#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

namespace cfg {
	int init();
	void deinit();

	int loadConfig();
	int saveConfig();
}

#endif
