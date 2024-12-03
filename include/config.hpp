#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

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
		MENU_NEXT,
		MENU_PREV,
		MENU_SELECT,
		NONE
	};

	int init();
	void deinit();

	int loadConfig();
	int saveConfig();
}

#endif
