#include "game.hpp"

#include "engine.hpp"
#include "ui/main.hpp"
#include "ui/input.hpp"

#include <chrono>
#include <thread>

namespace game {
	enum inputCodes {
		NOMINAL,
		TIMEOUT,
		QUIT
	};

	enum timerID {
		TIMER_FRAME,
		TIMER_TIMEOUT
	};

	struct timer {
		std::chrono::time_point<std::chrono::steady_clock> post;
		std::chrono::milliseconds delta;
	};

	int timeoutAction(
			ngin::playfield* p,
			shp::polyomino** shape,
			shp::polyomino** nextShape
	) {
		if (ngin::movePolyno(p, *shape, {0,1})) {
			return 0;
		}

		shp::deinitPolyomino(shape);

		if (ngin::hasFilledRow(p)) {

			std::vector<bool> filledRows = ngin::checkFilledRows(p);

			ngin::clearFilledRows(p, filledRows);
		}

		*shape = *nextShape;
		*nextShape = ngin::drawTetro();

		// We can't place the shape, game over!
		if (!ngin::polynoMoveCheck(p, *shape, {0,0})) {
			return 1;
		}

		ngin::setPolynoPos(p, *shape, {0,0});
		return 0;
	}

	void run() {
		namespace sc = std::chrono;
		ngin::init();


		// Basic setup.
		ngin::playfield p = ngin::initPlayfield(ngin::PLAYER_ONE, 20, 10);
		shp::polyomino* shape = ngin::drawTetro();
		shp::polyomino* nextShape = ngin::drawTetro();
		ngin::setPolynoPos(&p, shape, {0,0});

		bool loop = true;
		int currentCols = -1;
		int currentLines = -1;
		int timeoutMax = 400;
		int timeout = 0;

		std::map<timerID, timer> timers;

		sc::time_point<sc::steady_clock> pre = sc::steady_clock::now();
		timers[TIMER_FRAME] = {
			sc::steady_clock::now(),
			sc::milliseconds(0)
		};

		timers[TIMER_TIMEOUT] = {
			sc::steady_clock::now(),
			sc::duration_cast<sc::milliseconds>(pre.time_since_epoch())
		};

		ui::input::setCurrentMap(ui::input::map::GAME);
		ui::drawGame(&p, nextShape);
		while (loop) {
			pre = sc::steady_clock::now();
			for (auto [id, timer] : timers) {
				timers[id].delta = sc::duration_cast<sc::milliseconds>(
						pre.time_since_epoch()
						- timers[id].post.time_since_epoch());
			}

			if (ui::hasResized()) {
				ui::drawGame(&p, nextShape);
			}

			ui::input::fetch();
			while (ui::input::hasInput()) {
				namespace ipt = ui::input;
				ipt::setCurrentMap(ipt::map::GAME);
				ipt::bind bind = ipt::getNext();
				switch (bind) {
					case ipt::bind::GAME_QUIT:
						loop = false;
						break;
					case ipt::bind::GAME_LEFT:
						ngin::movePolyno(&p, shape, {-1,0});
						break;
					case ipt::bind::GAME_RIGHT:
						ngin::movePolyno(&p, shape, {1,0});
						break;

					case ipt::bind::GAME_ROTATE:
						ngin::rotate(&p, &shape);
						break;

					case ipt::bind::GAME_DROP:
						ngin::dropPolyno(&p, shape);
					case ipt::bind::GAME_DOWN:
					case ipt::bind::GAME_NO_ACTION:
						loop = timeoutAction(&p, &shape, &nextShape) != 1;
						break;
					default:
						break;
				}
				
				ui::drawGame(&p, nextShape);
			}

			if (timers.at(TIMER_TIMEOUT).delta > sc::milliseconds(timeoutMax)) {
				loop = timeoutAction(&p, &shape, &nextShape) != 1;
				timers[TIMER_TIMEOUT].post = sc::steady_clock::now();

				ui::drawGame(&p, nextShape);
			}

			std::this_thread::sleep_for(sc::milliseconds(1));
		}
		shp::deinitPolyomino(&shape);
		shp::deinitPolyomino(&nextShape);
		ngin::deinit();
	}
}
