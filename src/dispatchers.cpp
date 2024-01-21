#include "dispatchers.hpp"

CWindow *get_circle_next_window (std::string arg) {
	bool next_ready = false;
	CWindow *pTempClient =  g_pCompositor->m_pLastWindow;
    for (auto &w : g_pCompositor->m_vWindows)
    {
		CWindow *pWindow = w.get();
        if (pTempClient->m_iWorkspaceID !=pWindow->m_iWorkspaceID || pWindow->isHidden() || !pWindow->m_bIsMapped || pWindow->m_bFadingOut || pWindow->m_bIsFullscreen)
            continue;
		if (next_ready)
			return 	pWindow;
		if (pWindow == pTempClient)
			next_ready = true;	
    }

    for (auto &w : g_pCompositor->m_vWindows)
    {
		CWindow *pWindow = w.get();
        if (pTempClient->m_iWorkspaceID !=pWindow->m_iWorkspaceID || !g_pCompositor->isWorkspaceSpecial(pWindow->m_iWorkspaceID) || pWindow->isHidden() || !pWindow->m_bIsMapped || pWindow->m_bFadingOut || pWindow->m_bIsFullscreen)
            continue;
		return pWindow;
    }
	return nullptr;
}

void warpcursor_and_focus_to_window(CWindow *pWindow) {
	g_pCompositor->focusWindow(pWindow);
	g_pCompositor->warpCursorTo(pWindow->middle());
	g_pInputManager->m_pForcedFocus = pWindow;
    g_pInputManager->simulateMouseMovement();
    g_pInputManager->m_pForcedFocus = nullptr;
}

void dispatch_circle(std::string arg)
{
	CWindow *pWindow;
	pWindow = get_circle_next_window(arg);
	if(pWindow){
		warpcursor_and_focus_to_window(pWindow);
	}
}


void minimize_window(std::string arg)
{
	auto pWindow =  g_pCompositor->m_pLastWindow;
	auto pNode = g_Hide->getNodeFromWindow(pWindow);
  	if(pNode && !pNode->isMinimized) {
  	  g_Hide->hideWindowToSpecial(pWindow);
	  hych_log(LOG,"shortcut key toggle minimize window:{}",pWindow);
  	}
}

void restore_minimize_window(std::string arg)
{

	if(g_Hide->isInSpecialWorkspace()) {
		if(!g_pCompositor->isWorkspaceSpecial(g_pCompositor->m_pLastWindow->m_iWorkspaceID)) {
			g_Hide->leaveSpecialWorkspace();
			hych_log(LOG,"special workspace view,do noting");
			return;
		}
		g_Hide->restoreWindowFromSpecial(g_pCompositor->m_pLastWindow);
		g_Hide->leaveSpecialWorkspace();
		hych_log(LOG,"special workspace view,shortcut key toggle restore window:{}",g_pCompositor->m_pLastWindow);
		return;
	}

    for (auto &nd : g_Hide->m_lHideNodesData)
    {
        if(!nd.isMinimized) {
            continue;
        }
		g_Hide->restoreWindowFromSpecial(nd.pWindow);
		hych_log(LOG,"normal workspace view,shortcut key toggle restore window:{}",nd.pWindow);
        break;
	}	
}


void toggle_restore_window(std::string arg)
{
	if(g_pCompositor->m_pLastMonitor->specialWorkspaceID == 0) {
		g_pKeybindManager->toggleSpecialWorkspace("");
	} else {
		dispatch_circle("");
	}
}

void registerDispatchers()
{
	HyprlandAPI::addDispatcher(PHANDLE, "hych:minimize", minimize_window);
	HyprlandAPI::addDispatcher(PHANDLE, "hych:restore_minimize", restore_minimize_window);
	HyprlandAPI::addDispatcher(PHANDLE, "hych:toggle_restore_window", toggle_restore_window);

}
