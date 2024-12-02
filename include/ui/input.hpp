#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include <map>
#include <string>
#include <vector>

namespace ui::input {
	enum class code {
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_SPACE,
		KEY_RETURN,
		KEY_ARROW_UP,
		KEY_ARROW_DOWN,
		KEY_ARROW_LEFT,
		KEY_ARROW_RIGHT,
		ERROR,
		NOT_IMPLEMENTED
	};

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

	enum class map {
		GAME,
		MENU,
		NONE
	};

	typedef std::map<map, std::map<code, bind>> bindMap;

	int init();
	int deinit();

	bool hasInput();
	int fetch();
	bind getNext();

	void setCurrentMap(map map);
	map getCurrentMap();

	bindMap getBindMap();

	void setBind(bind bind, code code);
	bind getBind(code code);
	std::vector<code> getReverseBind(bind bind);

	code stringToCode(std::string name);
	std::string codeToString(code code);
	std::string codeToFriendlyString(code code);

	bind stringToBind(std::string name);
	std::string bindToString(bind bind);
	std::string bindToFriendlyString(bind bind);

	map stringToMap(std::string name);
	std::string mapToString(map map);
}

#endif
