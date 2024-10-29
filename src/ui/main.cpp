#include "ui/main.hpp"

#include "colour.hpp"
#include "config.hpp"
#include "engine.hpp"
#include "maths.hpp"

#include <ncurses.h>
#include <map>
#include <sstream>

namespace ui {
	enum class windowTypes {
		PLAYFIELD,
		NEXT_SHAPE,
		STATISTICS,
		CONTROLS
	};

	WINDOW* stdscr = NULL;
	std::map<windowTypes, WINDOW*> windows;

	int init() {
		int ret = 0;
		if ((stdscr = initscr()) == NULL) {ret++; return ret;}
		cbreak();
		noecho();
		curs_set(0);
		keypad(stdscr, TRUE);

		if (has_colors()) {
			start_color();

			init_pair(RED, COLOR_RED, COLOR_RED);
			init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);
			init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);
			init_pair(CYAN, COLOR_CYAN, COLOR_CYAN);
			init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);
			init_pair(MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
			init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);
		}


		return ret;
	}
	
	int deinit() {
		int ret = 0;

		delwin(stdscr);
		for (auto& [key, win]: windows) {
			delwin(win);
			windows.erase(key);
		}

		if (endwin() == ERR) {ret++;}

		return ret;
	}

	int drawStats() {
		WINDOW *statsWin = newwin(15, 17, 2, 10);
		refresh();
		box(statsWin, 0, 0);
		wrefresh(statsWin);

		getch();

		return 0;
	}

	WINDOW* initLogo() {
		WINDOW* logoWin = newwin(9, 2 * 38, 0, 0);

		// G
		mth::vect2D origin = {3,2};
		wattron(logoWin, COLOR_PAIR(CYAN));
		mvwaddstr(logoWin, origin.y, origin.x + 2, "XXXXXX");
		mvwaddstr(logoWin, origin.y + 1, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 2, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 2, origin.x + 4, "XXXX");
		mvwaddstr(logoWin, origin.y + 3, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 3, origin.x + 6, "XX");
		mvwaddstr(logoWin, origin.y + 4, origin.x + 2, "XXXX");
		wattroff(logoWin, COLOR_PAIR(CYAN));

		// R
		origin.x += 10;
		wattron(logoWin, COLOR_PAIR(BLUE));
		mvwaddstr(logoWin, origin.y, origin.x, "XXXXXX");
		mvwaddstr(logoWin, origin.y + 1, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 1, origin.x + 6, "XX");
		mvwaddstr(logoWin, origin.y + 2, origin.x, "XXXXXX");
		mvwaddstr(logoWin, origin.y + 3, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 3, origin.x + 4, "XX");
		mvwaddstr(logoWin, origin.y + 4, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 4, origin.x + 6, "XX");
		wattroff(logoWin, COLOR_PAIR(BLUE));

		// Z
		origin.x += 10;
		wattron(logoWin, COLOR_PAIR(MAGENTA));
		mvwaddstr(logoWin, origin.y, origin.x, "XXXXXX");
		mvwaddstr(logoWin, origin.y + 1, origin.x + 4, "XX");
		mvwaddstr(logoWin, origin.y + 2, origin.x + 2, "XX");
		mvwaddstr(logoWin, origin.y + 3, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 4, origin.x, "XXXXXX");
		wattroff(logoWin, COLOR_PAIR(MAGENTA));
		
		// E
		origin.x += 8;
		wattron(logoWin, COLOR_PAIR(RED));
		mvwaddstr(logoWin, origin.y, origin.x, "XXXXXX");
		mvwaddstr(logoWin, origin.y + 1, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 2, origin.x, "XXXX");
		mvwaddstr(logoWin, origin.y + 3, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 4, origin.x, "XXXXXX");
		wattroff(logoWin, COLOR_PAIR(RED));

		// T
		origin.x += 8;
		wattron(logoWin, COLOR_PAIR(YELLOW));
		mvwaddstr(logoWin, origin.y, origin.x, "XXXXXX");
		mvwaddstr(logoWin, origin.y + 1, origin.x + 2, "XX");
		mvwaddstr(logoWin, origin.y + 2, origin.x + 2, "XX");
		mvwaddstr(logoWin, origin.y + 3, origin.x + 2, "XX");
		mvwaddstr(logoWin, origin.y + 4, origin.x + 2, "XX");
		wattroff(logoWin, COLOR_PAIR(YELLOW));

		// R
		origin.x += 8;
		wattron(logoWin, COLOR_PAIR(GREEN));
		mvwaddstr(logoWin, origin.y, origin.x, "XXXXXX");
		mvwaddstr(logoWin, origin.y + 1, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 1, origin.x + 6, "XX");
		mvwaddstr(logoWin, origin.y + 2, origin.x, "XXXXXX");
		mvwaddstr(logoWin, origin.y + 3, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 3, origin.x + 4, "XX");
		mvwaddstr(logoWin, origin.y + 4, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 4, origin.x + 6, "XX");
		wattroff(logoWin, COLOR_PAIR(GREEN));

		// I
		origin.x += 10;
		wattron(logoWin, COLOR_PAIR(BLUE));
		mvwaddstr(logoWin, origin.y, origin.x, "XXXXXX");
		mvwaddstr(logoWin, origin.y + 1, origin.x + 2, "XX");
		mvwaddstr(logoWin, origin.y + 2, origin.x + 2, "XX");
		mvwaddstr(logoWin, origin.y + 3, origin.x + 2, "XX");
		mvwaddstr(logoWin, origin.y + 4, origin.x, "XXXXXX");
		wattroff(logoWin, COLOR_PAIR(BLUE));

		// S
		origin.x += 8;
		wattron(logoWin, COLOR_PAIR(MAGENTA));
		mvwaddstr(logoWin, origin.y, origin.x + 2, "XXXXXX");
		mvwaddstr(logoWin, origin.y + 1, origin.x, "XX");
		mvwaddstr(logoWin, origin.y + 2, origin.x + 2, "XXXX");
		mvwaddstr(logoWin, origin.y + 3, origin.x + 6, "XX");
		mvwaddstr(logoWin, origin.y + 4, origin.x, "XXXXXX");
		wattroff(logoWin, COLOR_PAIR(MAGENTA));

		return logoWin;
	}

	int drawPlayfield(const ngin::playfield *p) {
		if (!windows.contains(windowTypes::PLAYFIELD)) {
			// Create a window for the playfield.
			// Width is doubled to make each block more square.
			windows[windowTypes::PLAYFIELD] = newwin(
					2 * p->h + 2,
					2 * (2 * p->w) + 2,
					LINES / 2 - ((p->h) + 1), // Center vertically
					COLS / 2 - (2 * p->w + 1) // Center horizontally.
			);
		}
		WINDOW* playfieldWin = windows.at(windowTypes::PLAYFIELD);
		// Center the window
		mvwin(playfieldWin,
				LINES / 2 - ((p->h) + 1),
				COLS / 2 - (2 * p->w + 1));
		werase(playfieldWin);
		wborder(playfieldWin, 0, 0, 0, '^', 0, 0, '^', '^');

		// Print the playfield, block by block.
		for (int y = 0; y < p->h; y++) {
			for (int x = 0; x < p->w; x++) {
				const shp::block *currentBlock = p->blocks.at(y).at(x);
				if (currentBlock == nullptr) {
					continue;
				}

				if (has_colors()) {
					wattron(playfieldWin, COLOR_PAIR(currentBlock->c));
				}
				mvwaddstr(playfieldWin, (2 * y) + 1, (x * 4) + 1, "XXXX");
				mvwaddstr(playfieldWin, (2 * y) + 1 + 1, (x * 4) + 1, "XXXX");
				if (has_colors()) {
					wattroff(playfieldWin, COLOR_PAIR(currentBlock->c));
				}
			}
		}
		wrefresh(playfieldWin);
		return 0;
	}

	int drawNextShape(const shp::polyomino *shape) {
		const int HEIGHT = 8;
		const int WIDTH = 14;
		int nextX = 0, nextY = 0;
		if (windows.contains(windowTypes::PLAYFIELD)) {
			nextX = getmaxx(windows.at(windowTypes::PLAYFIELD))
				+ getbegx(windows.at(windowTypes::PLAYFIELD));
			nextY = getbegy(windows.at(windowTypes::PLAYFIELD));
		}

		if (!windows.contains(windowTypes::NEXT_SHAPE)) {
			windows[windowTypes::NEXT_SHAPE]
				= newwin(HEIGHT, WIDTH, nextY, nextX);
		}

		WINDOW* nextWin = windows.at(windowTypes::NEXT_SHAPE);
		mvwin(nextWin, nextY, nextX);
		werase(nextWin);
		box(nextWin, 0, 0);

		mvwaddstr(nextWin, 2, centre(WIDTH, 4), "NEXT");

		for (const shp::block* block: shape->blocks) {
			if (has_colors()) {
				wattron(nextWin, COLOR_PAIR(block->c));
			}
			mvwaddstr(
					nextWin,
					HEIGHT / 2 + (block->pos.y),
					WIDTH / 2 - shape->w + (2 * block->pos.x),
					"XX"
			);
			if (has_colors()) {
				wattroff(nextWin, COLOR_PAIR(block->c));
			}
		}

		wrefresh(nextWin);
		return 0;
	}

	std::string getBindChar(cfg::bind bind) {
		std::vector<int> chars = cfg::getReverseBind(bind);

		if (!chars.empty()) {
			std::stringstream bindChar;
			bindChar << "[" << (char)chars.at(0) << "]";
			return bindChar.str();
		} else {
			return "(none)";
		}
	}

	int drawControls() {
		const int HEIGHT = 15;
		const int WIDTH = 20;
		int controlsX = 0, controlsY = 0;
		if (windows.contains(windowTypes::PLAYFIELD)) {
			controlsX = getbegx(windows.at(windowTypes::PLAYFIELD)) - WIDTH;
			controlsY = getbegy(windows.at(windowTypes::PLAYFIELD));
				//+ getmaxy(windows.at(windowTypes::PLAYFIELD)) - HEIGHT;
		}

		if (!windows.contains(windowTypes::CONTROLS)) {
			windows[windowTypes::CONTROLS] = newwin(
					HEIGHT, WIDTH, controlsY, controlsX);
		}

		WINDOW* controlsWin = windows.at(windowTypes::CONTROLS);
		mvwin(controlsWin, controlsY, controlsX);
		werase(controlsWin);
		box(controlsWin, 0, 0);

		int cursorLine = 2;
		mvwaddstr(controlsWin, cursorLine, centre(WIDTH, 8), "CONTROLS");

		cursorLine += 1;

		// List of binds to render
		cfg::bind gameBinds[] = {
			cfg::bind::GAME_LEFT,
			cfg::bind::GAME_RIGHT,
			cfg::bind::GAME_DOWN,
			cfg::bind::NONE,
			cfg::bind::GAME_DROP,
			cfg::bind::GAME_ROTATE,
			cfg::bind::NONE,
			cfg::bind::GAME_PAUSE,
			cfg::bind::GAME_QUIT,
		};

		for (cfg::bind bind: gameBinds) {
			if (bind == cfg::bind::NONE) {
				cursorLine++;
				continue;
			}

			mvwaddstr(
					controlsWin,
					++cursorLine,
					3,
					cfg::bindToFriendlyString(bind).c_str());
			mvwaddstr(
					controlsWin,
					cursorLine,
					WIDTH - getBindChar(bind).length() - 3,
					getBindChar(bind).c_str());
		}

		

		wrefresh(controlsWin);
		return 0;
	}
}
