#include "globaleventhook.hpp"
#include <hyprland/src/SharedDefs.hpp>
#include "hide.hpp"

typedef void (*origIHyprLayout_requestFocusForWindow)(void* , CWindow* pWindow);
typedef void (*origCWindow_moveToWorkspace)(void*,int workspaceID);


void openWindowHook(void* self, SCallbackInfo &info, std::any data) {
    auto* const pWindow = std::any_cast<CWindow*>(data);
    
    if (pWindow->isHidden() || !pWindow->m_bIsMapped || pWindow->m_bFadingOut || pWindow->m_bX11DoesntWantBorders)
      return;

    const auto pNode = &g_Hide->m_lHideNodesData.emplace_back(); // make a new node in list back
    const auto pWindowOriWorkspace = g_pCompositor->getWorkspaceByID(pWindow->m_iWorkspaceID);

    pNode->pWindow = pWindow;

    if(!g_pCompositor->isWorkspaceSpecial(pWindow->m_iWorkspaceID)) { 
      pNode->hibk_workspaceID = pWindow->m_iWorkspaceID;
      pNode->hibk_workspaceName = pWindowOriWorkspace->m_szName;
      pNode->isMinimized = false;
    } else {
      pNode->hibk_workspaceID = 1;
      pNode->hibk_workspaceName = "";
      pNode->isMinimized = true;
      wlr_foreign_toplevel_handle_v1_set_minimized(pWindow->m_phForeignToplevel, true);
    }

    
    hych_log(LOG,"bind a node to window{}",pNode->pWindow);
  
}

void closeWindowHook(void* self, SCallbackInfo &info, std::any data) {
    auto* const pWindow = std::any_cast<CWindow*>(data);
    
    auto pNode = g_Hide->getNodeFromWindow(pWindow);

    if (!pNode)
        return;

    g_Hide->m_lHideNodesData.remove(*pNode);    
  
}


void workspaceHook(void* self, SCallbackInfo &info, std::any data) {
  auto* const pWorkspace = std::any_cast<CWorkspace*>(data);
    
  auto pNode = g_Hide->getNodeFromWindow(g_pCompositor->m_pLastWindow);

  if (pNode && !pNode->isMinimized && !g_pCompositor->isWorkspaceSpecial(pWorkspace->m_iID)) {
     pNode->hibk_workspaceID = pWorkspace->m_iID;
     pNode->hibk_workspaceName =pWorkspace->m_szName;
  }
}


void hkIHyprLayout_requestFocusForWindow(void* thisptr,CWindow* pWindow) {

  auto pNode = g_Hide->getNodeFromWindow(pWindow);
  if (pNode && pNode->isMinimized) {
    g_Hide->restoreWindowFromSpecial(pWindow);
    return;
  }

  if(pNode && !pNode->isMinimized && pWindow == g_pCompositor->m_pLastWindow) {
    g_Hide->hideWindowToSpecial(pWindow);
    return;
  }

  (*(origIHyprLayout_requestFocusForWindow)g_pIHyprLayout_requestFocusForWindowHook->m_pOriginal)(thisptr, pWindow);
}

void hkCWindow_moveToWorkspace(void* thisptr,int workspaceID) {
  auto pWindow = g_pCompositor->m_pLastWindow;
  auto pNode = g_Hide->getNodeFromWindow(pWindow);

  if (pNode && g_pCompositor->isWorkspaceSpecial(workspaceID)) {
    pNode->isMinimized = true;
    wlr_foreign_toplevel_handle_v1_set_minimized(pWindow->m_phForeignToplevel, true);
  } else if(pNode && g_Hide->isInSpecialWorkspace()) {
      pNode->isMinimized = false;
      wlr_foreign_toplevel_handle_v1_set_minimized(pWindow->m_phForeignToplevel, false);  
  }
  
  (*(origCWindow_moveToWorkspace)g_pCWindow_moveToWorkspaceHook->m_pOriginal)(thisptr, workspaceID);

}

void hkEvents_listener_requestMinimize(void* thisptr,void* owner, void* data) {
    auto pWindow = g_pCompositor->m_pLastWindow;
    auto pNode = g_Hide->getNodeFromWindow(pWindow);

    if(pNode && !pNode->isMinimized) {
      g_Hide->hideWindowToSpecial(pWindow);
    }
}

void registerGlobalEventHook() {

  HyprlandAPI::registerCallbackDynamic(PHANDLE, "openWindow", [&](void* self, SCallbackInfo& info, std::any data) { openWindowHook(self, info, data); });
  HyprlandAPI::registerCallbackDynamic(PHANDLE, "closeWindow", [&](void* self, SCallbackInfo& info, std::any data) { closeWindowHook(self, info, data); });
  HyprlandAPI::registerCallbackDynamic(PHANDLE, "workspace", [&](void* self, SCallbackInfo& info, std::any data) { workspaceHook(self, info, data); });

  g_pIHyprLayout_requestFocusForWindowHook = HyprlandAPI::createFunctionHook(PHANDLE, (void*)&IHyprLayout::requestFocusForWindow, (void*)&hkIHyprLayout_requestFocusForWindow);
  g_pIHyprLayout_requestFocusForWindowHook->hook();

  g_pCWindow_moveToWorkspaceHook = HyprlandAPI::createFunctionHook(PHANDLE, (void*)&CWindow::moveToWorkspace, (void*)&hkCWindow_moveToWorkspace);
  g_pCWindow_moveToWorkspaceHook->hook();

  g_pEvents_listener_requestMinimizeHook = HyprlandAPI::createFunctionHook(PHANDLE, (void*)&Events::listener_requestMinimize, (void*)&hkEvents_listener_requestMinimize);
  g_pEvents_listener_requestMinimizeHook->hook();
}
