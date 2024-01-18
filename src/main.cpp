#include "dispatchers.hpp"
#include "globaleventhook.hpp"

APICALL EXPORT std::string PLUGIN_API_VERSION() { return HYPRLAND_API_VERSION; }

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle)
{
	PHANDLE = handle;

#define CONF(NAME, TYPE, VALUE) \
	// HyprlandAPI::addConfigValue(PHANDLE, "plugin:hych:" NAME, SConfigValue{.TYPE##Value = VALUE})
	// CONF("enable_xxx", int, 1);
#undef CONF

	g_Hide = std::make_unique<Hide>();

	registerGlobalEventHook();
	registerDispatchers();
	
	return {"hych", "hyprland grocery Store", "DreamMaoMao", "0.1"};
}

APICALL EXPORT void PLUGIN_EXIT() {}
