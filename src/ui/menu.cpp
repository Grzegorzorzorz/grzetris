#include "ui/menu.hpp"

#include "ui/input.hpp"
#include "ui/main.hpp"

#include "game.hpp"

#include <chrono>
#include <thread>

#include <ncurses.h>

namespace ui::menu {
	signal doNothing() {
		return SIGCONT;
	}

	int linkNodes(menu* m, bool isCircular=true) {
		for (int i = 0; i < m->nodes.size(); i++) {
			node* n = &m->nodes.at(i);

			if (i != 0) {
				n->prev = &m->nodes.at(i - 1);
			} else if (isCircular) {
				n->prev = &m->nodes.at(m->nodes.size() - 1);
			} else {
				n->prev = nullptr;
			}

			if (i != m->nodes.size() - 1) {
				n->next = &m->nodes.at(i + 1);
			} else if (isCircular) {
				n->next = &m->nodes.at(0);
			} else {
				n->next = nullptr;
			}
		}

		return 0;
	}

	node initNode() {
		node ret;

		ret.text = "NONE";
		ret.next = nullptr;
		ret.prev = nullptr;

		ret.onSelect = doNothing;
		ret.onFocus = doNothing;
		ret.onFocusLost = doNothing;

		return ret;
	}

	menu initMain() {
		menu main;

		main.nodes.push_back(initNode());
		main.nodes.push_back(initNode());
		main.nodes.push_back(initNode());
		main.nodes.push_back(initNode());


		main.nodes[0].text = "Play";
		main.nodes[0].onSelect = []{game::run(); return SIGCONT;};

		main.nodes[1].text = "Settings";

		main.nodes[2].text = "Info";

		main.nodes[3].text = "Quit";
		main.nodes[3].onSelect = []{return SIGQUIT;};


		linkNodes(&main);

		main.selected = &main.nodes[0];

		return main;
	}

	int run(menu* m, std::function<int(menu*)> renderer) {
		bool doRun = true;
		input::setCurrentMap(input::map::MENU);

		renderer(m);
		while (doRun) {
			input::fetch();
			while (input::hasInput()) {
				int ret = inputDriver(m);
				doRun = ret != 1;
				if (!doRun) {
					break;
				}
				renderer(m);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		return 0;
	}

	int inputDriver(menu *m) {
		input::bind bind = input::getNext();

		signal sig = NONE;

		switch (bind) {
			case input::bind::MENU_NEXT:
				if (m->selected->next != nullptr) {
					sig = m->selected->onFocusLost();
					m->selected = m->selected->next;
					sig = m->selected->onFocus();
				}
				break;
			case input::bind::MENU_PREV:
				if (m->selected->prev != nullptr) {
					sig = m->selected->onFocusLost();
					m->selected = m->selected->prev;
					sig = m->selected->onFocus();
				}
				break;
			case input::bind::MENU_SELECT:
				sig = m->selected->onSelect();
				break;
			default:
				sig = NONE;
		}

		switch (sig) {
			case SIGCONT:
				return 0;
			case SIGQUIT:
				return 1;
			case NONE:
			default:
				return -1;
		}
	}

	int rendererMain(menu* main) {
		const int WIDTH = 16;
		const int HEIGHT = 10;
		WINDOW* optionsWin
			= newwin(HEIGHT, WIDTH, 1.5 * centre(LINES, HEIGHT), centre(COLS, WIDTH));

		clear();
		refresh();

		WINDOW* logoWin = initLogo();
		mvwin(logoWin, centre(LINES, 9) * 0.5, centre(COLS, 2 * 38));
		wrefresh(logoWin);

		for (int i = 0; i < main->nodes.size(); i++) {
			node* n = &main->nodes.at(i);

			if (main->selected == n) {
				wattron(optionsWin, A_STANDOUT);
			}

			mvwaddstr(
					optionsWin,
					2*i, centre(WIDTH, n->text.size()),
					(n->text).c_str());

			if (main->selected == n) {
				wattroff(optionsWin, A_STANDOUT);
			}
		}

		wrefresh(optionsWin);
		delwin(optionsWin);
		delwin(logoWin);
		return 0;

	}
}
