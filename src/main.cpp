#include "dispatchers.hpp"
#include "globaleventhook.hpp"

APICALL EXPORT std::string PLUGIN_API_VERSION() { return HYPRLAND_API_VERSION; }

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle)
{
	PHANDLE = handle;

#define CONF(NAME, TYPE, VALUE) \
	HyprlandAPI::addConfigValue(PHANDLE, "plugin:hych:" NAME, SConfigValue{.TYPE##Value = VALUE})
	CONF("enable_alt_release_exit", int, 0);
	CONF("alt_replace_key", str, "Alt_L");
#undef CONF

	static const auto *pEnable_alt_release_exit = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hych:enable_alt_release_exit")->intValue;
	static const auto *pAlt_replace_key = &HyprlandAPI::getConfigValue(PHANDLE, "plugin:hych:alt_replace_key")->strValue;

	g_hych_enable_alt_release_exit = *pEnable_alt_release_exit;
	g_hych_alt_replace_key = *pAlt_replace_key;


	g_hych_Hide = std::make_unique<Hide>();

	registerGlobalEventHook();
	registerDispatchers();
	
 	HyprlandAPI::reloadConfig();

	return {"hych", "Window minimization plugin", "DreamMaoMao", "0.1"};
}

APICALL EXPORT void PLUGIN_EXIT() {}
