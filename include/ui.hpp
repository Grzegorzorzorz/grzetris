#ifndef __UI_HPP__
#define __UI_HPP__

#include "game.hpp"

#include <ncurses.h>

namespace ui {
	int init();
	int deinit();

	int drawPlayfield(const game::playfield* p);
	int drawStats();
}

#endif
