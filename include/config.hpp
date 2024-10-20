#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <string>
#include <vector>

namespace cfg {
	enum class bind {
		GAME_LEFT,
		GAME_RIGHT,
		GAME_DOWN,
		GAME_DROP,
		GAME_ROTATE,
		GAME_PAUSE,
		GAME_QUIT,
		GAME_NO_ACTION,
		NONE
	};

	int init();
	void deinit();

	int loadConfig();

	bind getBind(int input);
	std::vector<int> getReverseBind(bind bind);

	void setBind(bind bind, int input);

	std::string bindToFriendlyString(bind bind);
}

#endif
