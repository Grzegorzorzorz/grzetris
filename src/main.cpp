#include "config.hpp"
#include "game.hpp"
#include "ui.hpp"

int main(int argc, const char** argv) {
	cfg::init();
	ui::init();

	game::run();

	ui::deinit();
	cfg::deinit();

	return 0;
}
