#include "ui/input.hpp"

#include <queue>

#include <ncurses.h>

namespace ui::input {
	std::queue<code> inputBuffer;
	bool isInit = false;

	int init() {
		if (isInit) {
			return 1;
		}

		nodelay(stdscr, true);

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
		return !inputBuffer.empty();
	}

	int fetch() {
		int inputSequences = 0;

		int input = -1;

		while ((input = getch()) != ERR) {
			inputBuffer.push(input);
			inputSequences++;
		}

		return inputSequences;
	}

	code getNext() {
		if (!hasInput()) {
			return -1;
		}

		code inputCode = inputBuffer.front();
		inputBuffer.pop();

		return inputCode;
	}
}
