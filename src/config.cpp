#include "config.hpp"

#include "ncurses.h"

#include <map>
#include <vector>

namespace cfg {
	std::map<int, bind> bindings;

	int init() {
		return loadConfig();
	}

	void deinit() {

	}

	int loadBindings() {
		// TODO: Offload this to a file at some point.
		bindings['h'] = bind::GAME_LEFT;
		bindings['k'] = bind::GAME_RIGHT;
		bindings['j'] = bind::GAME_DOWN;
		bindings[' '] = bind::GAME_DROP;
		bindings['r'] = bind::GAME_ROTATE;
		bindings['q'] = bind::GAME_QUIT;
		bindings[ERR] = bind::GAME_NO_ACTION;

		return 0;
	}

	int loadConfig() {
		int ret = 0;
		ret += loadBindings();

		return ret;
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
