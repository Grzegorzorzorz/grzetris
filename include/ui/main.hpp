#ifndef __UI_MAIN_HPP__
#define __UI_MAIN_HPP__

#include "engine.hpp"

#include <ncurses.h>

namespace ui {
	int init();
	int deinit();

	int drawPlayfield(const ngin::playfield* p);
	int drawNextShape(const shp::polyomino* shape);
	int drawControls();
}

#endif
