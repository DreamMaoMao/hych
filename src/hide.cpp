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

void Hide::refocusToSourceWorkspaceAfterMove(int workspaceID) {
    for (auto &w : g_pCompositor->m_vWindows)
    {
		CWindow *pWindow = w.get();
        if ((g_pCompositor->m_pLastMonitor->specialWorkspaceID != 0 && !g_pCompositor->isWorkspaceSpecial(pWindow->m_iWorkspaceID)) || pWindow->m_iWorkspaceID != workspaceID || pWindow->isHidden() || !pWindow->m_bIsMapped || pWindow->m_bFadingOut || pWindow->m_bIsFullscreen)
            continue;
        g_pCompositor->focusWindow(pWindow);
        return;
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

void Hide::leaveSpecialWorkspace() {
	const auto pMonitor = g_pCompositor->m_pLastMonitor;
	pMonitor->setSpecialWorkspace(nullptr);

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

    CWorkspace *pWorkspace;
    auto pNode = getNodeFromWindow(pWindow);
    
    if(!pNode) 
        return;

    pNode->isMinimized = false;

    if(g_hych_restore_to_old_workspace) {
        pWorkspace = g_pCompositor->getWorkspaceByID(pNode->hibk_workspaceID);
        if (!pWorkspace){
            hych_log(LOG,"source workspace no exist");
            pWorkspace = g_pCompositor->createNewWorkspace(pNode->hibk_workspaceID, pNode->pWindow->m_iMonitorID,pNode->hibk_workspaceName);
        }
    } else {
        pWorkspace =  g_pCompositor->getWorkspaceByID(g_pCompositor->m_pLastMonitor->activeWorkspace);
    }

    auto pMonitor = g_pCompositor->getMonitorFromID(pWorkspace->m_iMonitorID);
    g_pCompositor->moveWindowToWorkspaceSafe(pWindow, pWorkspace);
    pMonitor->changeWorkspace(pWorkspace);


    wlr_foreign_toplevel_handle_v1_set_minimized(pWindow->m_phForeignToplevel, false);
    g_pCompositor->focusWindow(pWindow);
}