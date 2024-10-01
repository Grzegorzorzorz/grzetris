#include "ui.hpp"
#include "colour.hpp"

#include <ncurses.h>

namespace ui {
	WINDOW* stdscr = NULL;

	int init() {
		int ret = 0;
		if ((stdscr = initscr()) == NULL) {ret++; return ret;}
		cbreak();
		noecho();
		curs_set(0);

		if (!has_colors()) {
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
		if (endwin() == ERR) {ret++;}
		delwin(stdscr);

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
		// Create a window for the playfield.
		// Width is doubled to make each block more square.
		WINDOW* playfieldWin = newwin(
				2 * p->h + 2,
				(4 * p->w) + 2,
				LINES / 2 - ((p->h) + 1), // Center vertically
				COLS / 2 - (2 * p->w + 1) // Center horizontally.
		);
		refresh();
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
		delwin(playfieldWin);
		return 0;
	}
}
