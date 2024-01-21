#pragma once

#include "log.hpp"
#include <hyprland/src/includes.hpp>
#include <any>

#define private public
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/managers/KeybindManager.hpp>
#undef private

#include <boost/algorithm/string.hpp>

#include "hide.hpp"

inline HANDLE PHANDLE = nullptr;
inline CFunctionHook *g_hych_pIHyprLayout_requestFocusForWindowHook = nullptr;
inline CFunctionHook *g_hych_pCWindow_moveToWorkspaceHook = nullptr;
inline CFunctionHook *g_hych_pEvents_listener_requestMinimizeHook = nullptr;
inline CFunctionHook* g_hych_pOnKeyboardKeyHook = nullptr;

inline int g_hych_enable_alt_release_exit;
inline std::string g_hych_alt_replace_key;

inline void errorNotif()
{
	HyprlandAPI::addNotificationV2(
		PHANDLE,
		{
			{"text", "Something has gone very wrong. Check the log for details."},
			{"time", (uint64_t)10000},
			{"color", CColor(1.0, 0.0, 0.0, 1.0)},
			{"icon", ICON_ERROR},
		});
}

