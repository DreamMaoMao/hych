#include "dispatchers.hpp"
#include <hyprland/src/SharedDefs.hpp>
#include "hide.hpp"

typedef void (*origIHyprLayout_requestFocusForWindow)(void* , CWindow* pWindow);
typedef void (*origCWindow_moveToWorkspace)(void*,int workspaceID);
typedef void (*origOnKeyboardKey)(void*, wlr_keyboard_key_event* e, SKeyboard* pKeyboard);

void openWindowHook(void* self, SCallbackInfo &info, std::any data) {
    auto* const pWindow = std::any_cast<CWindow*>(data);
    
    if (pWindow->isHidden() || !pWindow->m_bIsMapped || pWindow->m_bFadingOut || pWindow->m_bX11DoesntWantBorders)
      return;

    const auto pNode = &g_hych_Hide->m_lHideNodesData.emplace_back(); // make a new node in list back
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

    
    hych_log(LOG,"bind a memory node to window:{}",pNode->pWindow);
  
}

void closeWindowHook(void* self, SCallbackInfo &info, std::any data) {
    auto* const pWindow = std::any_cast<CWindow*>(data);
    
    auto pNode = g_hych_Hide->getNodeFromWindow(pWindow);

    if (!pNode)
        return;

    g_hych_Hide->m_lHideNodesData.remove(*pNode);
    hych_log(LOG,"remove a memory node which is bind to window:{}",pNode->pWindow);    
  
}


void workspaceHook(void* self, SCallbackInfo &info, std::any data) {
  auto* const pWorkspace = std::any_cast<CWorkspace*>(data);
    
  auto pNode = g_hych_Hide->getNodeFromWindow(g_pCompositor->m_pLastWindow);

  if (pNode && !pNode->isMinimized && !g_pCompositor->isWorkspaceSpecial(pWorkspace->m_iID)) {
     pNode->hibk_workspaceID = pWorkspace->m_iID;
     pNode->hibk_workspaceName =pWorkspace->m_szName;
     hych_log(LOG,"update workspace memory,workspaceID:{},window:{}",pWorkspace->m_iID,pNode->pWindow);    
  }
}


void hkIHyprLayout_requestFocusForWindow(void* thisptr,CWindow* pWindow) {

  auto pNode = g_hych_Hide->getNodeFromWindow(pWindow);
  if (pNode && pNode->isMinimized) {
    g_hych_Hide->restoreWindowFromSpecial(pWindow);
    hych_log(LOG,"click waybar to restore window:{}",pWindow);    
    return;
  }

  if(pNode && !pNode->isMinimized && pWindow == g_pCompositor->m_pLastWindow) {
    g_hych_Hide->hideWindowToSpecial(pWindow);
    hych_log(LOG,"click waybar to minimize window:{}",pWindow);    
    return;
  }

  (*(origIHyprLayout_requestFocusForWindow)g_hych_pIHyprLayout_requestFocusForWindowHook->m_pOriginal)(thisptr, pWindow);
}

void hkCWindow_moveToWorkspace(void* thisptr,int workspaceID) {
  auto pWindow = g_pCompositor->m_pLastWindow;
  auto pNode = g_hych_Hide->getNodeFromWindow(pWindow);

  if (pNode && g_pCompositor->isWorkspaceSpecial(workspaceID)) {
    pNode->isMinimized = true;
    wlr_foreign_toplevel_handle_v1_set_minimized(pWindow->m_phForeignToplevel, true);
    hych_log(LOG,"window enter special workspace,minimized:{},window:{}",pNode->isMinimized,pWindow); 
  } else if(pNode && g_hych_Hide->isInSpecialWorkspace()) {
      pNode->isMinimized = false;
      wlr_foreign_toplevel_handle_v1_set_minimized(pWindow->m_phForeignToplevel, false);  
      hych_log(LOG,"window leave special workspace,minimized:{},window:{}",pNode->isMinimized,pWindow); 
  }
  
  (*(origCWindow_moveToWorkspace)g_hych_pCWindow_moveToWorkspaceHook->m_pOriginal)(thisptr, workspaceID);

}

void hkEvents_listener_requestMinimize(void* thisptr,void* owner, void* data) {
    auto pWindow = g_pCompositor->m_pLastWindow;
    auto pNode = g_hych_Hide->getNodeFromWindow(pWindow);

    if(pNode && !pNode->isMinimized) {
      hych_log(LOG,"receive minimize request from client,window:{}",pWindow); 
      g_hych_Hide->hideWindowToSpecial(pWindow);
    }
}

std::string getKeynameFromKeycode(wlr_keyboard_key_event* e, SKeyboard* pKeyboard) {
  struct wlr_keyboard *keyboard =  (struct wlr_keyboard *)pKeyboard->keyboard;
  xkb_keycode_t keycode = e->keycode + 8;
  xkb_keysym_t keysym = xkb_state_key_get_one_sym(keyboard->xkb_state, keycode);
  char *tmp_keyname = new char[64];
  xkb_keysym_get_name(keysym, tmp_keyname, 64);
  std::string keyname = tmp_keyname;
  delete[] tmp_keyname;
  return keyname;
}

bool isKeyReleaseToggleExitOverviewHit(wlr_keyboard_key_event* e, SKeyboard* pKeyboard) {
  if (g_hych_alt_replace_key == "")
    return false;

  if (isNumber(g_hych_alt_replace_key) && std::stoi(g_hych_alt_replace_key) > 9 && std::stoi(g_hych_alt_replace_key) == (e->keycode + 8)) {
    return true;
  } else if (g_hych_alt_replace_key.find("code:") == 0 && isNumber(g_hych_alt_replace_key.substr(5)) && std::stoi(g_hych_alt_replace_key.substr(5)) == (e->keycode + 8)) {
    return true;
  } else {
    std::string keyname = getKeynameFromKeycode(e,pKeyboard);
    if (keyname == g_hych_alt_replace_key) {
      return true;
    }
  }

  return false;
}

static void hkOnKeyboardKey(void* thisptr,wlr_keyboard_key_event* e, SKeyboard* pKeyboard) {

  (*(origOnKeyboardKey)g_hych_pOnKeyboardKeyHook->m_pOriginal)(thisptr, e, pKeyboard);
  // hycov_log(LOG,"alt key,keycode:{}",e->keycode);
  if(g_hych_enable_alt_release_exit && g_pCompositor->m_pLastMonitor->specialWorkspaceID != 0 && e->state == WL_KEYBOARD_KEY_STATE_RELEASED) {
    if (!isKeyReleaseToggleExitOverviewHit(e,pKeyboard))
      return;
    restore_minimize_window("");
    g_hych_Hide->leaveSpecialWorkspace();
    hych_log(LOG,"alt key release toggle leave special workspace");
  }

}

void registerGlobalEventHook() {

  HyprlandAPI::registerCallbackDynamic(PHANDLE, "openWindow", [&](void* self, SCallbackInfo& info, std::any data) { openWindowHook(self, info, data); });
  HyprlandAPI::registerCallbackDynamic(PHANDLE, "closeWindow", [&](void* self, SCallbackInfo& info, std::any data) { closeWindowHook(self, info, data); });
  HyprlandAPI::registerCallbackDynamic(PHANDLE, "workspace", [&](void* self, SCallbackInfo& info, std::any data) { workspaceHook(self, info, data); });

  g_hych_pIHyprLayout_requestFocusForWindowHook = HyprlandAPI::createFunctionHook(PHANDLE, (void*)&IHyprLayout::requestFocusForWindow, (void*)&hkIHyprLayout_requestFocusForWindow);
  g_hych_pIHyprLayout_requestFocusForWindowHook->hook();

  g_hych_pCWindow_moveToWorkspaceHook = HyprlandAPI::createFunctionHook(PHANDLE, (void*)&CWindow::moveToWorkspace, (void*)&hkCWindow_moveToWorkspace);
  g_hych_pCWindow_moveToWorkspaceHook->hook();

  g_hych_pEvents_listener_requestMinimizeHook = HyprlandAPI::createFunctionHook(PHANDLE, (void*)&Events::listener_requestMinimize, (void*)&hkEvents_listener_requestMinimize);
  g_hych_pEvents_listener_requestMinimizeHook->hook();

  g_hych_pOnKeyboardKeyHook = HyprlandAPI::createFunctionHook(PHANDLE, (void*)&CInputManager::onKeyboardKey, (void*)&hkOnKeyboardKey);
  //apply hook OnKeyboardKey function
  if (g_hych_enable_alt_release_exit) {
      g_hych_pOnKeyboardKeyHook->hook();
  }
}
