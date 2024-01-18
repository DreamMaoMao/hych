#pragma once
#include "globals.hpp"

struct SHideNodeData
{
  CWindow *pWindow = nullptr;

  bool isMinimized = false;

  int hibk_workspaceID = -1;
  std::string hibk_workspaceName = "";

  bool operator==(const SHideNodeData &rhs) const
  {
    return pWindow == rhs.pWindow;
  }
};

class Hide {
public:
  SHideNodeData *getNodeFromWindow(CWindow *pWindow); 
  void hideWindowToSpecial(CWindow *pWindow);
  void restoreWindowFromSpecial(CWindow *pWindow);
  std::list<SHideNodeData> m_lHideNodesData; 

};


inline std::unique_ptr<Hide> g_Hide;
