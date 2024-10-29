#include "config.hpp"
#include "ui/main.hpp"
#include "ui/menu.hpp"

int main(int argc, const char** argv) {
	cfg::init();
	ui::init();

	ui::menu::menu main = ui::menu::initMain();

	bool doRun = true;
	while (doRun) {
		ui::menu::drawMain(&main);
		int ret = ui::menu::runMenu(&main);
		doRun = ret != 1;
	}

	ui::deinit();
	cfg::deinit();

	return 0;
}
