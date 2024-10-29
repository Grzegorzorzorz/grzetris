#include "ui/menu.hpp"

#include "config.hpp"
#include "game.hpp"
#include "ui/main.hpp"

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
			}

			if (i != m->nodes.size() - 1) {
				n->next = &m->nodes.at(i + 1);
			} else if (isCircular) {
				n->next = &m->nodes.at(0);
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
		timeout(-1);
		bool doRun = true;
		while (doRun) {
			renderer(m);
			int ret = inputDriver(m);
			doRun = ret != 1;
		}

		return 0;
	}

	int inputDriver(menu *m) {
		int input = getch();

		cfg::bind bind = cfg::getBind(input);

		signal sig;

		switch (bind) {
			case cfg::bind::MENU_NEXT:
				sig = m->selected->onFocusLost();
				m->selected = m->selected->next;
				sig = m->selected->onFocus();
				break;
			case cfg::bind::MENU_PREV:
				sig = m->selected->onFocusLost();
				m->selected = m->selected->prev;
				sig = m->selected->onFocus();
				break;
			case cfg::bind::MENU_SELECT:
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
