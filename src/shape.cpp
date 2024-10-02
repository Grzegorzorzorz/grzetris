#include "shape.hpp"

#include "maths.hpp"

#include <vector>

namespace shp {
	polyomino* initPolyomino(mth::vect2D pos, int h, int w) {
		polyomino* ret = new polyomino;
		ret->pos = pos;
		ret->h = h;
		ret->w = w;
		ret->pivot = {w / 2, h / 2};

		return ret;
	}

	void deinitPolyomino(polyomino** p) {
		if (p == nullptr || *p == nullptr) {
			return;
		}

		for (block* b : (*p)->blocks) {
			delete b;
		}
		(*p)->blocks.clear();
		delete *p;
		
		(*p) = nullptr;
	}

	polyomino* copyPolyomino(const polyomino* src) {
		polyomino* dest = new polyomino;
		*dest = *src;

		dest->blocks.clear();
		// Deep copy blocks.
		for (block* b: src->blocks) {
				dest->blocks.push_back(&(*(new block) = *b));
		}

		return dest;
	}

	void rotatePolyomino(
			polyomino* p, 
			rotationDirection direction,
			int amount
	) {
		mth::vect2D &pivot = p->pivot;
		mth::vect2D pivotTracker = pivot;
		// Flip each block diagonally, bottom left corner.
		for (block* b : p->blocks) {
			int oldY = b->pos.y;
			b->pos.y = b->pos.x + p->h;
			b->pos.x = oldY - p->h;
		}

		int swap = pivotTracker.y;
		pivotTracker.y = pivotTracker.x + p->h;
		pivotTracker.x = swap - p->h;


		// Flip each block horizontally, right side.
		for (block* b : p->blocks) {
			b->pos.x = (b->pos.x * -1) - 1;
		}

		pivotTracker.x = (pivotTracker.x * -1) -1;


		// Adjust the position of the piece so it remains within its own
		// bounds

		// Update height and width
		swap = p->h;
		p->h = p->w;
		p->w = swap;

		// This needs to be adjusted for pieces which rotate about a line
		// intersection, such as the I and O.
		bool isOutOfBounds = false;
		for (block* b : p->blocks) {
			b->pos = b->pos - (pivotTracker - pivot);
		}
		
	}
}
