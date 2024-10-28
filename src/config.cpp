#include "config.hpp"

#include <fstream>
#include <map>
#include <ncurses.h>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace cfg {
	using json = nlohmann::json;

	const std::string CONFIG_FILE = "./config.json";

	std::map<int, bind> bindings;

	int init() {
		return loadConfig();
	}

	void deinit() {}

	int loadBindings(const json* config) {
		// Fallback bindings
		if (config == nullptr || !config->contains("bindings")) {
			bindings['h'] = bind::GAME_LEFT;
			bindings['k'] = bind::GAME_RIGHT;
			bindings['j'] = bind::GAME_DOWN;
			bindings[' '] = bind::GAME_DROP;
			bindings['r'] = bind::GAME_ROTATE;
			bindings['q'] = bind::GAME_QUIT;

			bindings[ERR] = bind::GAME_NO_ACTION;

			return 1;
		}

		json bindJson = config->at("bindings");

		for (auto& [action, key]: bindJson.items()) {
			bindings[key] = stringToBind(action);
		}

		bindings[ERR] = bind::GAME_NO_ACTION;

		return 0;
	}

	int loadConfig() {
		json config = nullptr;

		std::ifstream cfgFile;
		cfgFile.open(CONFIG_FILE);

		if (cfgFile.peek() != std::ifstream::traits_type::eof()) {
			config = json::parse(cfgFile);
		}

		int ret = 0;
		ret += loadBindings(&config);

		return ret;
	}

	int saveBindings(json* config) {
		json bindJson;
		
		for (auto& [key, action]: bindings) {
			if (bindings.at(key) == bind::GAME_NO_ACTION
					|| bindings.at(key) == bind::NONE)
			{
				continue;
			}
			bindJson[bindToString(action)] = key;
		}

		(*config)["bindings"] = bindJson;

		return 0;
	}

	int saveConfig() {
		json config;
		saveBindings(&config);

		std::ofstream configFile;
		configFile.open(CONFIG_FILE);
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
