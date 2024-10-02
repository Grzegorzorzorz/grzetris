#include "game.hpp"
#include "ui.hpp"

int main(int argc, const char** argv) {
	ui::init();

	game::run();

	ui::deinit();

	return 0;
}
