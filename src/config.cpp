#include "config.hpp"

#include "ncurses.h"

#include <map>

namespace cfg {
	std::map<char, bind> bindings;

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

	bind getBind(char input) {
		if (bindings.contains(input)) {
			return bindings.at(input);
		} else {
			return bind::NONE;
		}
	}
	
	void setBind(bind bind, char input) {
		bindings[input] = bind;
	}
}
