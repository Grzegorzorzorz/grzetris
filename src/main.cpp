#include "game.hpp"
#include "shape.hpp"
#include "ui.hpp"

#include <cassert>

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
	while (loop) {
		if (currentCols != COLS || currentLines != LINES) {
			currentCols = COLS;
			currentLines = LINES;
			clear();
		}
		ui::drawPlayfield(&p);
		timeout(500);
		char input = getch();

		switch (input) {
			case ' ':
				loop = false;
				break;

			case 'h':
				game::movePolyno(&p, shape, {-1,0});
				break;
			case 'j':
				game::movePolyno(&p, shape, {0,1});
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
			case 'n':
				shp::deinitPolyomino(&shape);
				shape = game::drawTetro();
				game::setPolynoPos(&p, shape, {0,0});
				break;

			default:
				if (!game::movePolyno(&p, shape, {0,1})) {
					shp::deinitPolyomino(&shape);
				}
				break;
		}
	}

	shp::deinitPolyomino(&shape);
	ui::deinit();
	game::deinit();

	return 0;
}
