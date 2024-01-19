#include "hide.hpp"

SHideNodeData *Hide::getNodeFromWindow(CWindow *pWindow)
{
    for (auto &nd : m_lHideNodesData)
    {
        if (nd.pWindow == pWindow)
            return &nd;
    }

    return nullptr;
}

bool Hide::isInSpecialWorkspace() {
    static auto* const PFOLLOWMOUSE = &g_pConfigManager->getConfigValuePtr("input:follow_mouse")->intValue;
    std::string workspaceName = "";
    int workspaceID   = getWorkspaceIDFromString("special:",workspaceName);
    bool requestedWorkspaceIsAlreadyOpen = false;
    const auto PMONITOR = *PFOLLOWMOUSE == 1 ? g_pCompositor->getMonitorFromCursor() : g_pCompositor->m_pLastMonitor;
    int specialOpenOnMonitor = PMONITOR->specialWorkspaceID;


    for (auto& m : g_pCompositor->m_vMonitors) {
        if (m->specialWorkspaceID == workspaceID) {
            requestedWorkspaceIsAlreadyOpen = true;
            break;
        }
    }
    if (requestedWorkspaceIsAlreadyOpen && specialOpenOnMonitor == workspaceID) {
      return true;
    }
  return false;
}

void Hide::refocusToSourceWorkspaceAfterMove(int workspaceID) {
    if (g_pCompositor->m_pLastWindow->m_iWorkspaceID == workspaceID)
        return;
    
    for (auto &w : g_pCompositor->m_vWindows) {
        CWindow *pWindow = w.get();
        if (pWindow->m_iWorkspaceID == workspaceID) {
            g_pCompositor->focusWindow(pWindow);
            return;
        }
    }
    g_pCompositor->focusWindow(nullptr);
}

void Hide::moveWindowToSpecialWorlspace(CWindow *pWindow) {
    std::string workspaceName = "";

    const int   WORKSPACEID = getWorkspaceIDFromString("special", workspaceName);

    if (WORKSPACEID == INT_MAX) {
        Debug::log(ERR, "Error in moveActiveToWorkspaceSilent, invalid value");
        return;
    }

    if (WORKSPACEID == pWindow->m_iWorkspaceID)
        return;

    g_pHyprRenderer->damageWindow(pWindow);

    auto       pWorkspace = g_pCompositor->getWorkspaceByID(WORKSPACEID);

    if (pWorkspace) {
        g_pCompositor->moveWindowToWorkspaceSafe(pWindow, pWorkspace);
    } else {
        pWorkspace = g_pCompositor->createNewWorkspace(WORKSPACEID, pWindow->m_iMonitorID, workspaceName);
        g_pCompositor->moveWindowToWorkspaceSafe(pWindow, pWorkspace);
    }
}


void Hide::hideWindowToSpecial(CWindow *pWindow) {

    auto pNode = getNodeFromWindow(pWindow);
    int workspaceID = pWindow->m_iWorkspaceID;
    if(!pNode) 
        return;

    pNode->isMinimized = true;

    moveWindowToSpecialWorlspace(pWindow);
    refocusToSourceWorkspaceAfterMove(workspaceID);
    wlr_foreign_toplevel_handle_v1_set_activated(pWindow->m_phForeignToplevel, false);
}


void Hide::restoreWindowFromSpecial(CWindow *pWindow) {

    auto pNode = getNodeFromWindow(pWindow);
    
    if(!pNode) 
        return;

    pNode->isMinimized = false;
    
    auto pWorkspace = g_pCompositor->getWorkspaceByID(pNode->hibk_workspaceID);
    if (!pWorkspace){
        hych_log(LOG,"source workspace no exist");
        pWorkspace = g_pCompositor->createNewWorkspace(pNode->hibk_workspaceID, pNode->pWindow->m_iMonitorID,pNode->hibk_workspaceName);
    }

    auto pMonitor = g_pCompositor->getMonitorFromID(pWorkspace->m_iMonitorID);
    g_pCompositor->moveWindowToWorkspaceSafe(pWindow, pWorkspace);
    pMonitor->changeWorkspace(pWorkspace);


    wlr_foreign_toplevel_handle_v1_set_minimized(pWindow->m_phForeignToplevel, false);
    g_pCompositor->focusWindow(pWindow);
}