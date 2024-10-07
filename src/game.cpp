#include "game.hpp"

#include "ui.hpp"

#include <sys/time.h>

namespace game {
	void run() {
		ngin::init();

		// Basic setup.
		ngin::playfield p = ngin::initPlayfield(ngin::PLAYER_ONE, 20, 10);
		shp::polyomino* shape = ngin::drawTetro();
		shp::polyomino* nextShape = ngin::drawTetro();
		ngin::setPolynoPos(&p, shape, {0,0});

		bool loop = true;
		int currentCols = COLS;
		int currentLines = LINES;
		int timeoutMax = 500;
		int timeout = timeoutMax;
		while (loop) {
			if (currentCols != COLS || currentLines != LINES) {
				currentCols = COLS;
				currentLines = LINES;
				clear();
			}
			ui::drawPlayfield(&p);
			ui::drawNextShape(nextShape);

			timeval startTime, endTime;
			gettimeofday(&startTime, NULL);

			timeout(timeout);
			char input = getch();

			gettimeofday(&endTime, NULL);

			// check if we timed out.
			if (input == ERR) {
				timeout = timeoutMax;

				// otherwise calculate the new timeout
			} else { 
				timeval deltaTime;
				deltaTime.tv_sec = endTime.tv_sec - startTime.tv_sec;
				deltaTime.tv_usec = endTime.tv_usec - startTime.tv_usec;

				// carry second if micro is negative
				if (deltaTime.tv_usec < 0) {
					deltaTime.tv_usec += 1000000;
					deltaTime.tv_sec--;
				}

				if ((timeout -= deltaTime.tv_usec / 1000) <= 0) {
					timeout = timeoutMax;
				}
			}

			switch (input) {
				case 'q':
					loop = false;
					break;

				case 'h':
					ngin::movePolyno(&p, shape, {-1,0});
					break;
				case 'r':
					ngin::rotate(&p, &shape);
					break;

				case 'k':
					ngin::movePolyno(&p, shape, {1,0});
					break;

				case ' ':
					ngin::dropPolyno(&p, shape);
				case 'j':
				case ERR:
					if (ngin::movePolyno(&p, shape, {0,1})) {break;}
					shp::deinitPolyomino(&shape);

					if (ngin::hasFilledRow(&p)) {

						std::vector<bool> filledRows = ngin::checkFilledRows(&p);

						timeout = timeoutMax;

						ngin::clearFilledRows(&p, filledRows);
					}

					shape = nextShape;
					nextShape = ngin::drawTetro();
					ngin::setPolynoPos(&p, shape, {0,0});

			}
		}

		shp::deinitPolyomino(&shape);

		ngin::deinit();
	}
}
