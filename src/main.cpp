#include "game.hpp"
#include "shape.hpp"
#include "ui.hpp"

#include <cassert>
#include <sys/time.h>

int main(int argc, const char** argv) {
	game::init();
	ui::init();

	game::playfield p = game::initPlayfield(game::PLAYER_ONE, 20, 10);

	game::drawTetro();


	shp::polyomino* shape = game::drawTetro();
	game::setPolynoPos(&p, shape, {0,0});
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
			case ' ':
				loop = false;
				break;

			case 'h':
				game::movePolyno(&p, shape, {-1,0});
				break;
			case 'k':
				game::movePolyno(&p, shape, {0,-1});
				break;
			case 'l':
				game::movePolyno(&p, shape, {1,0});
				break;
			case 'r':
				game::rotate(&p, &shape);
				break;

			case 'j':
			default:
				if (!game::movePolyno(&p, shape, {0,1})) {
					shp::deinitPolyomino(&shape);

					std::vector<bool> filledRows = game::checkFilledRows(&p);

					// Flash every row about to be cleared.
					for (int i = 0; i < p.h; i++) {
					}

					shape = game::drawTetro();
					game::setPolynoPos(&p, shape, {0,0});

				}
				break;
		}
	}

	shp::deinitPolyomino(&shape);
	ui::deinit();
	game::deinit();

	return 0;
}
