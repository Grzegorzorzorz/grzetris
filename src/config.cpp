#include "config.hpp"

#include "ui/input.hpp"

#include <filesystem>
#include <fstream>
#include <map>
#include <ncurses.h>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace cfg {
	namespace sfs = std::filesystem;
	using json = nlohmann::json;

	const std::string CONFIG_FILE = "config.json";
	const sfs::path CONFIG_PATH	= sfs::path(getenv("HOME"))
#ifdef BUILD_RELEASE
	#if defined (PLAT_UNIX)
			.append(".config")
			.append("grzetris")
			.append(CONFIG_FILE);
	#elif defined (PLAT_APPLE)
	// TODO: Test this on an actual Mac :)
			.append("Library")
			.append("Application Support")
			.append("net.grzeg.grzetris")
			.append(CONFIG_FILE);
	#else
			.append(CONFIG_FILE)
	#endif
#else
	// Use the current dir for debugging the config
	= std::filesystem::current_path().append(CONFIG_FILE);
#endif

	std::map<int, bind> bindings;

	int init() {
		return loadConfig();
	}

	void deinit() {
		saveConfig();
	}

	void setDefaultBinds() {
		namespace ipt = ui::input;

		ipt::setCurrentMap(ipt::map::GAME);
		ipt::setBind(ipt::bind::GAME_LEFT, ipt::code::KEY_H);
		ipt::setBind(ipt::bind::GAME_DOWN, ipt::code::KEY_J);
		ipt::setBind(ipt::bind::GAME_RIGHT, ipt::code::KEY_K);
		ipt::setBind(ipt::bind::GAME_DROP, ipt::code::KEY_SPACE);
		ipt::setBind(ipt::bind::GAME_ROTATE, ipt::code::KEY_R);
		ipt::setBind(ipt::bind::GAME_QUIT, ipt::code::KEY_Q);
		ipt::setBind(ipt::bind::GAME_NO_ACTION, ipt::code::ERROR);

		ipt::setCurrentMap(ipt::map::MENU);
		ipt::setBind(ipt::bind::MENU_NEXT, ui::input::code::KEY_J);
		ipt::setBind(ipt::bind::MENU_PREV, ui::input::code::KEY_K);
		ipt::setBind(ipt::bind::MENU_SELECT, ui::input::code::KEY_RETURN);

		ipt::setCurrentMap(ipt::map::NONE);
	}

	int loadBindings(const json* config) {
		namespace ipt = ui::input;

		// Fallback bindings
		if (config == nullptr || !config->contains("bindings")) {
			setDefaultBinds();
			return 1;
		}

		json bindJson = json::parse(config->at("bindings").dump());

		for (auto& [map, bindings]: bindJson.items()) {
			if (ipt::stringToMap(map) == ipt::map::NONE) {
				continue;
			}

			ipt::setCurrentMap(ipt::stringToMap(map));
			for (auto& [bind, code]: bindings.items()) {
				ipt::setBind(ipt::stringToBind(bind), ipt::stringToCode(code));
			}
		}

		// Set default must have values.
		ipt::setCurrentMap(ipt::map::GAME);
		ipt::setBind(ipt::bind::GAME_NO_ACTION, ipt::code::ERROR);

		ipt::setCurrentMap(ipt::map::NONE);

		return 0;
	}

	int loadConfig() {
		json config = nullptr;

		std::ifstream cfgFile;
		cfgFile.open(CONFIG_PATH);

		if (cfgFile.peek() != std::ifstream::traits_type::eof()) {
			config = json::parse(cfgFile);
		}

		int ret = 0;
		ret += loadBindings(&config);

		return ret;
	}

	int saveBindings(json* config) {
		namespace ipt = ui::input;

		json bindJson;

		for (auto& [map, bindings]: ipt::getBindMap()) {
			if (map == ipt::map::NONE) {
				continue;
			}

			for (auto& [code, bind]: bindings) {
				if (code == ipt::code::ERROR
						|| code == ipt::code::NOT_IMPLEMENTED) {
					continue;
				}
				bindJson
					[ipt::mapToString(map)]
					[ipt::bindToString(bind)] = ipt::codeToString(code);
			}
		}
		//for (auto& [key, action]: bindings) {
		//	if (bindings.at(key) == bind::GAME_NO_ACTION
		//			|| bindings.at(key) == bind::NONE) {
		//		continue;
		//	}
		//	bindJson[bindToString(action)] = key;
		//}

		(*config)["bindings"] = bindJson;

		return 0;
	}

	int createConfigDir() {
		#ifdef BUILD_DEBUG
		return -1;
		#endif
		sfs::create_directories(CONFIG_PATH.parent_path());
		return 0;
	}

	int saveConfig() {
		json config;
		saveBindings(&config);

		createConfigDir();
		std::ofstream configFile;
		configFile.open(CONFIG_PATH);
		configFile << config.dump(4);
		configFile.close();

		return 0;
	}
}
