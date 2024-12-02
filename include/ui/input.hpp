#ifndef __INPUT_HPP__
#define __INPUT_HPP__

namespace ui::input {
	typedef int code;
	int init();
	int deinit();

	bool hasInput();
	int fetch();
	code getNext();
}

#endif
