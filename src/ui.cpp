#include "ui.hpp"
#include "colour.hpp"

#include <ncurses.h>
#include <map>

namespace ui {
	enum class windowTypes {
		PLAYFIELD,
		NEXT_SHAPE,
		STATISTICS
	};

	WINDOW* stdscr = NULL;
	std::map<windowTypes, WINDOW*> windows;

	int init() {
		int ret = 0;
		if ((stdscr = initscr()) == NULL) {ret++; return ret;}
		cbreak();
		noecho();
		curs_set(0);

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

	int drawPlayfield(const game::playfield *p) {
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
		if (!windows.contains(windowTypes::NEXT_SHAPE)) {
			int nextX, nextY = 0;
			if (windows.contains(windowTypes::PLAYFIELD)) {
				nextX = getmaxx(windows.at(windowTypes::PLAYFIELD))
						+ getbegx(windows.at(windowTypes::PLAYFIELD));
				nextY = getbegy(windows.at(windowTypes::PLAYFIELD));
			}
			windows[windowTypes::NEXT_SHAPE]
				= newwin(HEIGHT, WIDTH, nextY, nextX);
		}

		WINDOW* nextWin = windows.at(windowTypes::NEXT_SHAPE);
		werase(nextWin);
		box(nextWin, 0, 0);

		mvwaddstr(nextWin, 2, WIDTH / 2 - 2, "NEXT");

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
}
