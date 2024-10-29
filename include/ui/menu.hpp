#ifndef __UI_MENU_HPP__
#define __UI_MENU_HPP__

#include <functional>
#include <string>
#include <vector>

namespace ui::menu {
	enum signal {
		SIGCONT,
		SIGQUIT,
		NONE
	};

	struct node {
		std::string text;
		node* next;
		node* prev;
		std::function<signal()> onSelect;
		std::function<signal()> onFocus;
		std::function<signal()> onFocusLost;
	};

	struct menu {
		node* selected;
		std::vector<node> nodes;
	};

	node initNode();

	menu initMain();

	int runMenu(menu* m);

	int drawMain(menu* main);
}

#endif
