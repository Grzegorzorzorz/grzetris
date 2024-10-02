#ifndef __SHAPE_HPP__
#define __SHAPE_HPP__

#include "maths.hpp"
#include "colour.hpp"

#include <vector>

namespace shp {
	struct block;
	struct polyomino;

	enum class rotationDirection {
		ANTICLOCKWISE = -1,
		CLOCKWISE = 1
	};
	
	struct block {
		mth::vect2D pos;
		colour c;
	};

	struct polyomino {
		mth::vect2D pos;
		mth::vect2D pivot;
		int h, w;
		std::vector<block*> blocks;
	};

	polyomino* initPolyomino(mth::vect2D pos, int h, int w);
	void deinitPolyomino(polyomino** p);
	polyomino* copyPolyomino(const polyomino* src);
	void delPolynominoBlock(polyomino* p, block* newBlock);
	void rotatePolyomino(polyomino* p, rotationDirection direction, int amount);
}

#endif
