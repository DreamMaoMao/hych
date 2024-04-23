#include "dispatchers.hpp"
#include "globaleventhook.hpp"

APICALL EXPORT std::string PLUGIN_API_VERSION() { return HYPRLAND_API_VERSION; }

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle)
{
	PHANDLE = handle;

#define CONF(NAME,VALUE) \
	HyprlandAPI::addConfigValue(PHANDLE, "plugin:hych:" NAME, {VALUE})
	CONF("enable_alt_release_exit", 0L);
	CONF("alt_replace_key", "Alt_L");
	CONF("restore_to_old_workspace", 0L);
#undef CONF

	HyprlandAPI::reloadConfig();
	g_pConfigManager->tick();

	static const auto *pEnable_alt_release_exit = (Hyprlang::STRING const*)(HyprlandAPI::getConfigValue(PHANDLE, "plugin:hych:enable_alt_release_exit")->getDataStaticPtr());
	static const auto *pAlt_replace_key = (Hyprlang::STRING const*)(HyprlandAPI::getConfigValue(PHANDLE, "plugin:hych:alt_replace_key")->getDataStaticPtr());
	static const auto *pRestore_to_old_workspace = (Hyprlang::STRING const*)(HyprlandAPI::getConfigValue(PHANDLE, "plugin:hych:restore_to_old_workspace")->getDataStaticPtr());

	g_hych_enable_alt_release_exit = **pEnable_alt_release_exit;
	g_hych_alt_replace_key = *pAlt_replace_key;
	g_hych_restore_to_old_workspace = **pRestore_to_old_workspace;



	g_hych_Hide = std::make_unique<Hide>();

	registerGlobalEventHook();
	registerDispatchers();
	
 	HyprlandAPI::reloadConfig();

	return {"hych", "Window minimization plugin", "DreamMaoMao", "0.1"};
}

APICALL EXPORT void PLUGIN_EXIT() {}
