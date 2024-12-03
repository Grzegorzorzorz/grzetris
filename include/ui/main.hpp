#ifndef __UI_MAIN_HPP__
#define __UI_MAIN_HPP__

#include "engine.hpp"

#include <ncurses.h>

namespace ui {
	int init();
	int deinit();

	inline int centre(int parentSize, int size) {
		return (parentSize - size) / 2;
	}

	WINDOW* initLogo();

	bool hasResized();

	int drawGame(const ngin::playfield* p, const shp::polyomino* shape);

	int drawPlayfield(const ngin::playfield* p);
	int drawNextShape(const shp::polyomino* shape);
	int drawControls();
}

#endif
