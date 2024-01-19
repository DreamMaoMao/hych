#include "dispatchers.hpp"

// void dispatch_example(std::string arg)
// {

// }

void minimize_window(std::string arg)
{
	auto pWindow =  g_pCompositor->m_pLastWindow;
	auto pNode = g_Hide->getNodeFromWindow(pWindow);
  	if(pNode && !pNode->isMinimized) {
  	  g_Hide->hideWindowToSpecial(pWindow);
  	}
}

void restore_minimize_window(std::string arg)
{
    for (auto &nd : g_Hide->m_lHideNodesData)
    {
        if(!nd.isMinimized) {
            continue;
        }
		g_Hide->restoreWindowFromSpecial(nd.pWindow);
        break;
	}	
}

void registerDispatchers()
{
	// HyprlandAPI::addDispatcher(PHANDLE, "hych:example", dispatch_example);
	HyprlandAPI::addDispatcher(PHANDLE, "hych:minimize", minimize_window);
	HyprlandAPI::addDispatcher(PHANDLE, "hych:restore_minimize", restore_minimize_window);

}
