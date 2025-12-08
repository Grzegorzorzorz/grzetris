#include "game.hpp"

#include "engine.hpp"
#include "ui/main.hpp"
#include "ui/input.hpp"

#include <chrono>
#include <thread>
#include <iostream>

namespace sc = std::chrono;

namespace game {
	enum inputCodes {
		NOMINAL,
		TIMEOUT,
		QUIT
	};

	enum timerID {
		TIMER_FRAME,
		TIMER_LOCKOUT,
		TIMER_TIMEOUT
	};

	struct timer {
		sc::time_point<std::chrono::steady_clock> post;
		sc::milliseconds delta;
	};

	std::map<timerID, timer> initTimers() {
		std::map<timerID, timer> timers;

		timers[TIMER_FRAME] = {
			sc::steady_clock::now(),
			sc::milliseconds(0)
		};

		timers[TIMER_LOCKOUT] = {
			sc::steady_clock::time_point::max(),
			sc::milliseconds(0)
		};

		timers[TIMER_TIMEOUT] = {
			sc::steady_clock::now(),
			sc::duration_cast<sc::milliseconds>(
					sc::steady_clock::now().time_since_epoch())
		};

		return timers;
	}

	std::map<timerID, timer> updateTimers(std::map<timerID, timer> timers) {
		sc::time_point<sc::steady_clock> pre = sc::steady_clock::now();
		for (auto [id, timer] : timers) {
			timers[id].delta = sc::duration_cast<sc::milliseconds>(
					pre.time_since_epoch()
					- timers[id].post.time_since_epoch());
		}
		return timers;
	}

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
		ngin::init();


		// Prepare all game elements needed.
		ngin::playfield p = ngin::initPlayfield(ngin::PLAYER_ONE, 20, 10);
		shp::polyomino* shape = ngin::drawTetro();
		shp::polyomino* nextShape = ngin::drawTetro();

		// Launch the first tetromino
		ngin::setPolynoPos(&p, shape, {0,0});

		// Max deltas for timers, in milliseconds.
		int timeoutMax = 400;
		int lockoutMax = 500;

		bool loop = true;
		bool isLockout = false;

		std::map<timerID, timer> timers = initTimers();

		ui::input::setCurrentMap(ui::input::map::GAME);
		ui::drawGame(&p, nextShape);

		while (loop) {
			timers = updateTimers(timers);

			if (ui::hasResized()) {
				ui::drawGame(&p, nextShape);
			}

			// Track if we've moved the current shape this loop.
			// Used by lockout logic to prevent premature locking.
			bool hasMoved = false;

			// Run through the input stack, and resolve all keys.
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
						hasMoved = hasMoved || ngin::movePolyno(&p, shape, {-1,0});
						break;
					case ipt::bind::GAME_RIGHT:
						hasMoved = hasMoved || ngin::movePolyno(&p, shape, {1,0});
						hasMoved = true;
						break;

					case ipt::bind::GAME_ROTATE:
						hasMoved = hasMoved || (ngin::rotate(&p, &shape) == 0);
						break;

					case ipt::bind::GAME_DROP:
						ngin::dropPolyno(&p, shape);
					case ipt::bind::GAME_DOWN:
						loop = timeoutAction(&p, &shape, &nextShape) != 1;
						hasMoved = true;
						// Reset the timeout timer, since we've already done
						// its job.
						timers[TIMER_TIMEOUT].post = sc::steady_clock::now();
						timers[TIMER_TIMEOUT].delta = sc::milliseconds(0);

						// If the user moves the piece down themselves, they
						// forfeit the lockout grace peroid.
						isLockout = false;

						break;
					default:
						break;
				}
				
				ui::drawGame(&p, nextShape);
			}

			if (isLockout) {
				// Reset the grace period when the shape is moved.
				if (hasMoved) {
					timers[TIMER_LOCKOUT].post = sc::steady_clock::now();
					timers[TIMER_LOCKOUT].delta = sc::milliseconds(0);
				}

				bool canShapeFall = ngin::polynoMoveCheck(
						&p, shape, mth::vect2D(0,1) + shape->pos);

				// Leave the lockout state once the grace period is over, or
				// if the shape can fall again.
				if (timers[TIMER_LOCKOUT].delta >= sc::milliseconds(lockoutMax)
					|| canShapeFall)
				{
					isLockout = false;
					timers[TIMER_LOCKOUT].post = sc::steady_clock::time_point::max();

					// Prime the timeout timer so the shape doesn't plummet.
					timers[TIMER_TIMEOUT].post = sc::steady_clock::now();
					timers[TIMER_TIMEOUT].delta = sc::milliseconds(0);

					if (!canShapeFall) {
						timeoutAction(&p, &shape, &nextShape);
					}
					ui::drawGame(&p, nextShape);
				}
			}

			if (timers.at(TIMER_TIMEOUT).delta >= sc::milliseconds(timeoutMax)) {
				// We've timed out, but we've hit the bottom. It's time to
				// trigger the lockout state.
				if (!ngin::polynoMoveCheck(&p, shape, mth::vect2D(0,1) + shape->pos)
						&& isLockout == false) {
					isLockout = true;
					// Prime the lockout timer.
					timers[TIMER_LOCKOUT].post = sc::steady_clock::now();
				}

				if (!isLockout) {
					loop = timeoutAction(&p, &shape, &nextShape) != 1;

					ui::drawGame(&p, nextShape);
				}
				timers[TIMER_TIMEOUT].post = sc::steady_clock::now();
			}

			std::this_thread::sleep_for(sc::milliseconds(1));
		}
		shp::deinitPolyomino(&shape);
		shp::deinitPolyomino(&nextShape);
		ngin::deinit();
	}
}
