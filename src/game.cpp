#include "game.hpp"

#include "config.hpp"
#include "ui/main.hpp"

#include <sys/time.h>

namespace game {
	enum inputCodes {
		NOMINAL,
		TIMEOUT,
		QUIT
	};

	int timeoutAction(
			ngin::playfield* p,
			shp::polyomino** shape,
			shp::polyomino** nextShape,
			int* timeout,
			int* timeoutMax
	) {
		if (ngin::movePolyno(p, *shape, {0,1})) {
			return 0;
		}

		shp::deinitPolyomino(shape);

		if (ngin::hasFilledRow(p)) {

			std::vector<bool> filledRows = ngin::checkFilledRows(p);

			*timeout = *timeoutMax;

			ngin::clearFilledRows(p, filledRows);
		}

		*shape = *nextShape;
		*nextShape = ngin::drawTetro();

		// We can't place the shape, game over!
		if (!ngin::polynoMoveCheck(p, *shape, {0,0})) {
			return 1;
		}

		ngin::setPolynoPos(p, *shape, {0,0});
		return 0;
	}

	int calculateTimeout(
			timeval startTime,
			timeval endTime,
			int timeout,
			int timeoutMax
	) {
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

		return timeout;
	}

	inputCodes handleInput(
			int input,
			ngin::playfield* p,
			shp::polyomino** shape
	) {
			switch (cfg::getBind(input)) {
				case cfg::bind::GAME_QUIT:
					return QUIT;

				case cfg::bind::GAME_LEFT:
					ngin::movePolyno(p, *shape, {-1,0});
					break;
				case cfg::bind::GAME_RIGHT:
					ngin::movePolyno(p, *shape, {1,0});
					break;

				case cfg::bind::GAME_ROTATE:
					ngin::rotate(p, shape);
					break;

				case cfg::bind::GAME_DROP:
					ngin::dropPolyno(p, *shape);
				case cfg::bind::GAME_DOWN:
				case cfg::bind::GAME_NO_ACTION:
					return TIMEOUT;

				default:
					break;
			}
			return NOMINAL;
	}

	void run() {
		ngin::init();

		// Basic setup.
		ngin::playfield p = ngin::initPlayfield(ngin::PLAYER_ONE, 20, 10);
		shp::polyomino* shape = ngin::drawTetro();
		shp::polyomino* nextShape = ngin::drawTetro();
		ngin::setPolynoPos(&p, shape, {0,0});

		bool loop = true;
		int currentCols = -1;
		int currentLines = -1;
		int timeoutMax = 500;
		int timeout = 0;

		clear();
		ui::drawPlayfield(&p);
		ui::drawControls();

		while (loop) {
			if (currentCols != COLS || currentLines != LINES) {
				currentCols = COLS;
				currentLines = LINES;
			}
			ui::drawPlayfield(&p);
			ui::drawNextShape(nextShape);
			ui::drawControls();

			timeval startTime, endTime;
			gettimeofday(&startTime, NULL);

			timeout(timeout);
			int input = getch();

			gettimeofday(&endTime, NULL);

			// check if we timed out.
			if (input == ERR) {
				timeout = timeoutMax;
			} else { 
				timeout = calculateTimeout(
						startTime, endTime, timeout, timeoutMax);
			}
			switch (handleInput(input, &p, &shape)) {
				case NOMINAL:
					break;

				case QUIT:
					loop = false;
					break;

				case TIMEOUT:
					int ret = timeoutAction(
							&p, &shape, &nextShape, &timeout, &timeoutMax);

					if (ret == 1) {
						loop = false;
					}
			}
		}
		shp::deinitPolyomino(&shape);
		shp::deinitPolyomino(&nextShape);
		ngin::deinit();
	}
}
