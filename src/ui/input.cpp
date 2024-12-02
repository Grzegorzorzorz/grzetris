#include "ui/input.hpp"


#include <string>
#include <queue>

#include <ncurses.h>

namespace ui::input {
	std::queue<code> inputBuffer;
	bindMap bindings;
	map currentMap = map::NONE;

	bool isInit = false;

	int init() {
		if (isInit) {
			return 1;
		}

		nodelay(stdscr, TRUE);
		keypad(stdscr, TRUE);

		isInit = true;
		return 0;
	}

	int deinit() {
		if (!isInit) {
			return 1;
		}

		nodelay(stdscr, false);

		while (hasInput()) {
			inputBuffer.pop();
		}

		isInit = false;
		return 0;
	};

	bool hasInput() {
		return inputBuffer.size() != 0;
	}

	code ncursesToLocalCode(int ncursesCode) {
		switch (ncursesCode) {
			case 'a':
				return code::KEY_A;
			case 'b':
				return code::KEY_B;
			case 'c':
				return code::KEY_C;
			case 'd':
				return code::KEY_D;
			case 'e':
				return code::KEY_E;
			case 'f':
				return code::KEY_F;
			case 'g':
				return code::KEY_G;
			case 'h':
				return code::KEY_H;
			case 'i':
				return code::KEY_I;
			case 'j':
				return code::KEY_J;
			case 'k':
				return code::KEY_K;
			case 'l':
				return code::KEY_L;
			case 'm':
				return code::KEY_M;
			case 'n':
				return code::KEY_N;
			case 'o':
				return code::KEY_O;
			case 'p':
				return code::KEY_P;
			case 'q':
				return code::KEY_Q;
			case 'r':
				return code::KEY_R;
			case 's':
				return code::KEY_S;
			case 't':
				return code::KEY_T;
			case 'u':
				return code::KEY_U;
			case 'v':
				return code::KEY_V;
			case 'w':
				return code::KEY_W;
			case 'x':
				return code::KEY_X;
			case 'y':
				return code::KEY_Y;
			case 'z':
				return code::KEY_Z;
			case ' ':
				return code::KEY_SPACE;
			case '\n':
			case KEY_ENTER:
				return code::KEY_RETURN;
			case KEY_UP:
				return code::KEY_ARROW_UP;
			case KEY_DOWN:
				return code::KEY_ARROW_DOWN;
			case KEY_LEFT:
				return code::KEY_ARROW_LEFT;
			case KEY_RIGHT:
				return code::KEY_ARROW_RIGHT;
			case ERR:
				return code::ERROR;
			default:
				return code::NOT_IMPLEMENTED;
		}
	}

	int fetch() {
		int inputSequences = 0;

		int input = -1;

		while ((input = getch()) != ERR) {
			inputBuffer.push(ncursesToLocalCode(input));
			inputSequences++;
		}

		return inputSequences;
	}

	bind getNext() {
		if (!hasInput()) {
			return bind::NONE;
		}

		code inputCode = inputBuffer.front();
		inputBuffer.pop();

		return getBind(inputCode);
	}

	void setCurrentMap(map map) {
		currentMap = map;
	}

	map getCurrentMap() {
		return currentMap;
	}

	bindMap getBindMap() {
		return bindings;
	}

	void setBind(bind bind, code code) {
		if (currentMap == map::NONE) {
			return;
		}

		bindings[currentMap][code] = bind;
	}

	bind getBind(code code) {
		if (currentMap == map::NONE) {
			return bind::NONE;
		}

		if (!bindings.contains(currentMap)) {
			return bind::NONE;
		}

		if (!bindings.at(currentMap).contains(code)) {
			return bind::NONE;
		}

		return bindings.at(currentMap).at(code);
	}

	std::vector<code> getReverseBind(bind bind) {
		std::vector<code> keys;

		for (auto& [code, binding]: bindings.at(currentMap)) {
			if (binding == bind) {
				keys.push_back(code);
			}
		}

		return keys;
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

	code stringToCode(std::string name) {
		if (name == "KEY_A") {
			return code::KEY_A;
		} else if (name == "KEY_B") {
			return code::KEY_B;
		} else if (name == "KEY_C") {
			return code::KEY_C;
		} else if (name == "KEY_D") {
			return code::KEY_D;
		} else if (name == "KEY_E") {
			return code::KEY_E;
		} else if (name == "KEY_F") {
			return code::KEY_F;
		} else if (name == "KEY_G") {
			return code::KEY_G;
		} else if (name == "KEY_H") {
			return code::KEY_H;
		} else if (name == "KEY_I") {
			return code::KEY_I;
		} else if (name == "KEY_J") {
			return code::KEY_J;
		} else if (name == "KEY_K") {
			return code::KEY_K;
		} else if (name == "KEY_L") {
			return code::KEY_L;
		} else if (name == "KEY_M") {
			return code::KEY_M;
		} else if (name == "KEY_N") {
			return code::KEY_N;
		} else if (name == "KEY_O") {
			return code::KEY_O;
		} else if (name == "KEY_P") {
			return code::KEY_P;
		} else if (name == "KEY_Q") {
			return code::KEY_Q;
		} else if (name == "KEY_R") {
			return code::KEY_R;
		} else if (name == "KEY_S") {
			return code::KEY_S;
		} else if (name == "KEY_T") {
			return code::KEY_T;
		} else if (name == "KEY_U") {
			return code::KEY_U;
		} else if (name == "KEY_V") {
			return code::KEY_V;
		} else if (name == "KEY_W") {
			return code::KEY_W;
		} else if (name == "KEY_X") {
			return code::KEY_X;
		} else if (name == "KEY_Y") {
			return code::KEY_Y;
		} else if (name == "KEY_Z") {
			return code::KEY_Z;
		} else if (name == "KEY_SPACE") {
			return code::KEY_SPACE;
		} else if (name == "KEY_RETURN") {
			return code::KEY_RETURN;
		} else if (name == "KEY_ARROW_UP") {
			return code::KEY_ARROW_UP;
		} else if (name == "KEY_ARROW_DOWN") {
			return code::KEY_ARROW_DOWN;
		} else if (name == "KEY_ARROW_LEFT") {
			return code::KEY_ARROW_LEFT;
		} else if (name == "KEY_ARROW_RIGHT") {
			return code::KEY_ARROW_RIGHT;
		} else if (name == "ERROR") {
			return code::ERROR;
		} else {
			return code::NOT_IMPLEMENTED;
		}
	}

	std::string codeToString(code code) {
		switch (code) {
			case code::KEY_A:
				return "KEY_A";
			case code::KEY_B:
				return "KEY_B";
			case code::KEY_C:
				return "KEY_C";
			case code::KEY_D:
				return "KEY_D";
			case code::KEY_E:
				return "KEY_E";
			case code::KEY_F:
				return "KEY_F";
			case code::KEY_G:
				return "KEY_G";
			case code::KEY_H:
				return "KEY_H";
			case code::KEY_I:
				return "KEY_I";
			case code::KEY_J:
				return "KEY_J";
			case code::KEY_K:
				return "KEY_K";
			case code::KEY_L:
				return "KEY_L";
			case code::KEY_M:
				return "KEY_M";
			case code::KEY_N:
				return "KEY_N";
			case code::KEY_O:
				return "KEY_O";
			case code::KEY_P:
				return "KEY_P";
			case code::KEY_Q:
				return "KEY_Q";
			case code::KEY_R:
				return "KEY_R";
			case code::KEY_S:
				return "KEY_S";
			case code::KEY_T:
				return "KEY_T";
			case code::KEY_U:
				return "KEY_U";
			case code::KEY_V:
				return "KEY_V";
			case code::KEY_W:
				return "KEY_W";
			case code::KEY_X:
				return "KEY_X";
			case code::KEY_Y:
				return "KEY_Y";
			case code::KEY_Z:
				return "KEY_Z";
			case code::KEY_SPACE:
				return "KEY_SPACE";
			case code::KEY_RETURN:
				return "KEY_RETURN";
			case code::KEY_ARROW_UP:
				return "KEY_ARROW_UP";
			case code::KEY_ARROW_DOWN:
				return "KEY_ARROW_DOWN";
			case code::KEY_ARROW_LEFT:
				return "KEY_ARROW_LEFT";
			case code::KEY_ARROW_RIGHT:
				return "KEY_ARROW_RIGHT";
			case code::ERROR:
				return "ERROR";
			case code::NOT_IMPLEMENTED:
			default:
				return "NOT_IMPLEMENTED";
		}
	}

	std::string codeToFriendlyString(code code) {
		switch (code) {
			case code::KEY_A:
				return "A";
			case code::KEY_B:
				return "B";
			case code::KEY_C:
				return "C";
			case code::KEY_D:
				return "D";
			case code::KEY_E:
				return "E";
			case code::KEY_F:
				return "F";
			case code::KEY_G:
				return "G";
			case code::KEY_H:
				return "H";
			case code::KEY_I:
				return "I";
			case code::KEY_J:
				return "J";
			case code::KEY_K:
				return "K";
			case code::KEY_L:
				return "L";
			case code::KEY_M:
				return "M";
			case code::KEY_N:
				return "N";
			case code::KEY_O:
				return "O";
			case code::KEY_P:
				return "P";
			case code::KEY_Q:
				return "Q";
			case code::KEY_R:
				return "R";
			case code::KEY_S:
				return "S";
			case code::KEY_T:
				return "T";
			case code::KEY_U:
				return "U";
			case code::KEY_V:
				return "V";
			case code::KEY_W:
				return "W";
			case code::KEY_X:
				return "X";
			case code::KEY_Y:
				return "Y";
			case code::KEY_Z:
				return "Z";
			case code::KEY_SPACE:
				return "Space";
			case code::KEY_RETURN:
				return "Return";
			case code::KEY_ARROW_UP:
				return "Up";
			case code::KEY_ARROW_DOWN:
				return "Down";
			case code::KEY_ARROW_LEFT:
				return "Left";
			case code::KEY_ARROW_RIGHT:
				return "Right";
			case code::ERROR:
			case code::NOT_IMPLEMENTED:
			default:
				return "(unknown)";
		}
	}

	map stringToMap(std::string name) {
		if (name == "GAME") {
			return map::GAME;
		} else if (name == "MENU") {
			return map::MENU;
		} else {
			return map::NONE;
		}
	}

	std::string mapToString(map map) {
		switch (map) {
			case map::GAME:
				return "GAME";
			case map::MENU:
				return "MENU";
			case map::NONE:
			default:
				return "NONE";
		}
	}
}
