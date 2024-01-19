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

void Hide::hideWindowToSpecial(CWindow *pWindow) {

    auto pNode = getNodeFromWindow(pWindow);
    int workspaceID = pWindow->m_iWorkspaceID;
    if(!pNode) 
        return;

    pNode->isMinimized = true;

    g_pKeybindManager->moveActiveToWorkspaceSilent("special");
    refocusToSourceWorkspaceAfterMove(workspaceID);
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