#include "config.hpp"
#include "ui/main.hpp"
#include "ui/menu.hpp"

int main(int argc, const char** argv) {
	cfg::init();
	ui::init();

	ui::menu::menu main = ui::menu::initMain();
	ui::menu::run(&main, ui::menu::rendererMain);

	ui::deinit();
	cfg::deinit();

	return 0;
}
