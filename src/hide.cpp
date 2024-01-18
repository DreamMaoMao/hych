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

void Hide::hideWindowToSpecial(CWindow *pWindow) {

    auto pNode = getNodeFromWindow(pWindow);
    
    if(!pNode) 
        return;

    pNode->isMinimized = true;

    g_pKeybindManager->moveActiveToWorkspaceSilent("special");
    wlr_foreign_toplevel_handle_v1_set_activated(pWindow->m_phForeignToplevel, false);
    // wlr_foreign_toplevel_handle_v1_set_minimized(pWindow->m_phForeignToplevel, true);
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
    // g_pCompositor->m_pLastWindow =  pWindow;
    g_pCompositor->moveWindowToWorkspaceSafe(pWindow, pWorkspace);
    pMonitor->changeWorkspace(pWorkspace);


    wlr_foreign_toplevel_handle_v1_set_minimized(pWindow->m_phForeignToplevel, false);
    g_pCompositor->focusWindow(pWindow);
}