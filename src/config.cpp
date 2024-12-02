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

	bind getBind(int input) {
		if (bindings.contains(input)) {
			return bindings.at(input);
		} else {
			return bind::NONE;
		}
	}

	std::vector<int> getReverseBind(bind bind) {
		std::vector<int> out;

		for (auto const& [input, binding]: bindings) {
			if (binding == bind) {
				out.push_back(input);
			}
		}

		return out;
	}
	
	void setBind(bind bind, int input) {
		bindings[input] = bind;
	}

	bind stringToBind(std::string name) {
		if (name == "GAME_LEFT") {
			return bind::GAME_LEFT;
		} else  if (name == "GAME_RIGHT") {
			return bind::GAME_RIGHT;
		} else if (name == "GAME_DOWN") {
			return bind::GAME_DOWN;
		} else if (name == "GAME_DROP") {
			return bind::GAME_DROP;
		} else if (name == "GAME_ROTATE") {
			return bind::GAME_ROTATE;
		} else if (name == "GAME_PAUSE") {
			return bind::GAME_PAUSE;
		} else if (name == "GAME_QUIT") {
			return bind::GAME_QUIT;
		} else if (name == "MENU_NEXT") {
			return bind::MENU_NEXT;
		} else if (name == "MENU_PREV") {
			return bind::MENU_PREV;
		} else if (name == "MENU_SELECT") {
			return bind::MENU_SELECT;
		} else {
			return bind::NONE;
		}
	}

	std::string bindToString(bind bind) {
		switch (bind) {
			case bind::GAME_LEFT:
				return "GAME_LEFT";
			case bind::GAME_RIGHT:
				return "GAME_RIGHT";
			case bind::GAME_DOWN:
				return "GAME_DOWN";
			case bind::GAME_DROP:
				return "GAME_DROP";
			case bind::GAME_ROTATE:
				return "GAME_ROTATE";
			case bind::GAME_PAUSE:
				return "GAME_PAUSE";
			case bind::GAME_QUIT:
				return "GAME_QUIT";
			case bind::MENU_NEXT:
				return "MENU_NEXT";
			case bind::MENU_PREV:
				return "MENU_PREV";
			case bind::MENU_SELECT:
				return "MENU_SELECT";
			case bind::NONE:
			default:
				return "NONE";
		}
	}

	std::string bindToFriendlyString(bind bind) {
		switch (bind) {
			case bind::GAME_LEFT:
				return "Left";
			case bind::GAME_RIGHT:
				return "Right";
			case bind::GAME_DOWN:
				return "Down";
			case bind::GAME_DROP:
				return "Drop";
			case bind::GAME_ROTATE:
				return "Rotate";
			case bind::GAME_PAUSE:
				return "Pause";
			case bind::GAME_QUIT:
				return "Quit";
			default:
				return "(unknown)";
		}
	}
}
