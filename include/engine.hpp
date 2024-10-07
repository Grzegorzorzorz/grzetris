#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__
#include "shape.hpp"

#include <vector>

namespace ngin {
	enum playerID {
		PLAYER_ONE
	};

	struct playfield {
		playerID owner;
		int h, w;
		std::vector<std::vector<shp::block*>> blocks;
		shp::polyomino* currentShape;
		int score;
	};

	int init();
	int deinit();

	void shakeTetroBag();
	shp::polyomino* drawTetro();

	playfield initPlayfield(playerID owner, int height, int width);

	bool hasFilledRow(const playfield* p);
	std::vector<bool> checkFilledRows(const playfield* p);
	int scoreFilledRows(std::vector<bool> rows);
	int clearFilledRows(playfield* p, std::vector<bool> rows);
	int clearRow(playfield* p, int index);

	bool polynoMoveCheck(
			const playfield* p,
			const shp::polyomino* shape,
			mth::vect2D position,
			bool ignoreCurrentPos=true);
	bool movePolyno(
			playfield* p, shp::polyomino* shape, mth::vect2D displacement);
	void dropPolyno(
			playfield* p, shp::polyomino* shape);
	void deletePolynoBlocks(playfield* p, shp::polyomino* shape);
	void setPolynoPos(
			playfield* p, shp::polyomino* shape, mth::vect2D position);

	
	int rotate(playfield* p, shp::polyomino** shape);
}

#endif
