#include "game.hpp"

#include "colour.hpp"
#include "maths.hpp"
#include "shape.hpp"

#include <cassert>
#include <list>
#include <map>
#include <random>

namespace game {
	enum tetroShapes {
		TETRO_SHAPE_FIRST,
		TETRO_SHAPE_I,
		TETRO_SHAPE_O,
		TETRO_SHAPE_T,
		TETRO_SHAPE_L,
		TETRO_SHAPE_J,
		TETRO_SHAPE_S,
		TETRO_SHAPE_Z,
		TETRO_SHAPE_LAST
	};

	std::map<tetroShapes, shp::polyomino*> tetrominoes;
	std::list<playfield*> playfields;
	std::vector<tetroShapes> tetroBag;
	std::mt19937 randomGenerator;

	int loadTetros();

	int init() {
		loadTetros();

		std::random_device seed;
		randomGenerator.seed(seed());

		return 0;
	}

	int deinit() {
		for (playfield* p: playfields) {
			for (int y = 0; y < p->h; y++) {
				for (int x = 0; x < p->w; x++) {
					shp::block* currentBlock = p->blocks[y][x];
					if (currentBlock != nullptr) {
						delete currentBlock;
						p->blocks[y][x] = nullptr;
					}
				}
			}
		}

		for (auto& [key, value] : tetrominoes) {
			shp::deinitPolyomino(&value);
		}
		tetrominoes.clear();
		return 0;
	}

	int loadTetros() {
		tetrominoes.clear();
		// Messy, but it is what it is.
		// I'll probably move this to a json or something later.
		
		tetrominoes[TETRO_SHAPE_I] = shp::initPolyomino({-32,-32}, 4, 4);
		tetrominoes[TETRO_SHAPE_I]->blocks = {
			new shp::block({0,1}, CYAN),
			new shp::block({1,1}, CYAN),
			new shp::block({2,1}, CYAN),
			new shp::block({3,1}, CYAN)
		};

		tetrominoes[TETRO_SHAPE_O] = shp::initPolyomino({-32,-32}, 2, 2);
		tetrominoes[TETRO_SHAPE_O]->blocks = {
			new shp::block({0,0}, YELLOW),
			new shp::block({1,0}, YELLOW),
			new shp::block({0,1}, YELLOW),
			new shp::block({1,1}, YELLOW)
		};

		tetrominoes[TETRO_SHAPE_T] = shp::initPolyomino({-32,-32}, 3, 3);
		tetrominoes[TETRO_SHAPE_T]->blocks = {
			new shp::block({1,0}, MAGENTA),
			new shp::block({0,1}, MAGENTA),
			new shp::block({1,1}, MAGENTA),
			new shp::block({2,1}, MAGENTA)
		};

		tetrominoes[TETRO_SHAPE_L] = shp::initPolyomino({-32,-32}, 3, 3);
		tetrominoes[TETRO_SHAPE_L]->blocks = {
			new shp::block({2,0}, WHITE),
			new shp::block({0,1}, WHITE),
			new shp::block({1,1}, WHITE),
			new shp::block({2,1}, WHITE)
		};

		tetrominoes[TETRO_SHAPE_J] = shp::initPolyomino({-32,-32}, 3, 3);
		tetrominoes[TETRO_SHAPE_J]->blocks = {
			new shp::block({0,0}, BLUE),
			new shp::block({0,1}, BLUE),
			new shp::block({1,1}, BLUE),
			new shp::block({2,1}, BLUE)
		};

		tetrominoes[TETRO_SHAPE_S] = shp::initPolyomino({-32,-32}, 3, 3);
		tetrominoes[TETRO_SHAPE_S]->blocks = {
			new shp::block({1,0}, GREEN),
			new shp::block({2,0}, GREEN),
			new shp::block({1,1}, GREEN),
			new shp::block({0,1}, GREEN)
		};

		tetrominoes[TETRO_SHAPE_Z] = shp::initPolyomino({-32,-32}, 3, 3);
		tetrominoes[TETRO_SHAPE_Z]->blocks = {
			new shp::block({0,0}, RED),
			new shp::block({1,0}, RED),
			new shp::block({1,1}, RED),
			new shp::block({2,1}, RED)
		};

		return 0;
	}

	void shakeTetroBag() {
		tetroBag.clear();

		std::vector<tetroShapes> undrawnTetros = {
			TETRO_SHAPE_I,
			TETRO_SHAPE_O,
			TETRO_SHAPE_T,
			TETRO_SHAPE_L,
			TETRO_SHAPE_J,
			TETRO_SHAPE_S,
			TETRO_SHAPE_Z
		};

		while (tetroBag.size() < 7) {
			std::uniform_int_distribution<> tetroRange(0, undrawnTetros.size() - 1);
			int tetroIndex = tetroRange(randomGenerator);

			tetroBag.push_back(undrawnTetros.at(tetroIndex));
			undrawnTetros.erase(undrawnTetros.begin() + tetroIndex);
		}
	}

	shp::polyomino* drawTetro() {
		if (tetroBag.empty()) {
			shakeTetroBag();
		}

		shp::polyomino* ret
			= shp::copyPolyomino(tetrominoes.at(tetroBag.back()));
		tetroBag.pop_back();

		return ret;
	}

	bool polynoMoveCheck(
			const playfield *p,
			const shp::polyomino* shape,
			mth::vect2D position,
			bool ignoreCurrentPos
	) {
		if (p == nullptr) {return false;}
		// We ignore these blocks because they are the previous position of
		// the shape, so it's fine if we collide with them.
		std::vector<mth::vect2D> ignoredBlocks(shape->blocks.size());
		if (ignoreCurrentPos) {
			if (shape->pos.x != -32 && shape->pos.y != -32) {
				for (shp::block* block : shape->blocks) {
					// Don't ignore a block if it falls out of bounds.
					if (block->pos.y + position.y >= p->h
							|| block->pos.x + position.x >= p->w
							|| block->pos.y + position.y < 0
							|| block->pos.x + position.x < 0) {
						continue;
					}
					ignoredBlocks.push_back(block->pos + shape->pos);
				}
			}
		}

		for (shp::block* block : shape->blocks) {
			mth::vect2D newPos = block->pos + position;

			for (mth::vect2D ignored : ignoredBlocks) {
				if (ignored == newPos) {
					goto ignored;
				}
			}

			// Bounds checking.
			if (block->pos.y + position.y >= p->h
					|| block->pos.x + position.x >= p->w
					|| block->pos.y + position.y < 0
					|| block->pos.x + position.x < 0
			) {
				return false;
			}

			if (p->blocks[newPos.y][newPos.x] != nullptr) {
				return false;
			}

			continue;
			ignored:
			continue;
		}
		return true;
	}

	void deleteBlock(playfield* p, mth::vect2D pos) {
		shp::block* block = p->blocks[pos.y][pos.x];
		if (block != nullptr) {
			delete block;
			p->blocks[pos.y][pos.x] = nullptr;
		}
	}

	void deletePolynoBlocks(playfield *p, shp::polyomino *shape) {
		if (shape == nullptr) {return;}
		for (shp::block* block : shape->blocks) {
			// Clean up after the previous position.
			if (shape->pos.x != -32 || shape->pos.y != -32) {
				deleteBlock(p, shape->pos + block->pos);
			}
		}
	}

	void setPolynoPos(
			playfield* p, shp::polyomino* shape, mth::vect2D position) {

		if (p == nullptr) {return;}

		deletePolynoBlocks(p, shape);

		for (shp::block* block : shape->blocks) {
			// Blot the blocks in their new positions.
			mth::vect2D blockPosAbsolute = block->pos + position;
			if (p->blocks[blockPosAbsolute.y][blockPosAbsolute.x] != nullptr) {
				deleteBlock(p, blockPosAbsolute);
			}

			p->blocks[blockPosAbsolute.y][blockPosAbsolute.x]
				= &(*(new shp::block) = *block);
		}

		shape->pos = position;
	}

	bool movePolyno(
			playfield* p, shp::polyomino* shape, mth::vect2D displacement) {

		if (shape == nullptr) {return false;}

		if (polynoMoveCheck(p, shape, displacement + shape->pos)) {
			setPolynoPos(p, shape, displacement + shape->pos);
			return true;
		} else {
			return false;
		}
	}

	void dropPolyno(playfield *p, shp::polyomino *shape) {
		while (movePolyno(p, shape, {0, 1})) {;}
	}

	void move(playfield* p, mth::vect2D displacement) {
		movePolyno(p, p->currentShape, displacement);
	}

	playfield initPlayfield(playerID owner, int height, int width) {
		playfield ret;

		ret.owner = owner;

		ret.h = height;
		ret.w = width;
		ret.currentShape = nullptr;

		for (int y = 0; y < ret.h; y++) {
			ret.blocks.push_back({});
			for (int x = 0; x < ret.w; x++) {
				ret.blocks[y].push_back(nullptr);
			}
		}

		playfields.push_back(&ret);

		return ret;
	}

	bool hasFilledRow(const playfield *p) {
		bool hasFullRow = false;
		for (std::vector<shp::block*> row: p->blocks) {
			bool hasNullBlock = false;
			for (shp::block* block : row) {
				if (block == nullptr) {hasNullBlock = true;}
				break;
			}

			if (!hasNullBlock) {
				hasFullRow = true;
				break;
			}
		}

		return hasFullRow;
	}

	std::vector<bool> checkFilledRows(const playfield *p) {
		std::vector<bool> ret;
		ret.reserve(p->h);

		for (std::vector<shp::block*> row: p->blocks) {
			bool isRowFilled = true;
			for (shp::block* block: row) {
				if (block == nullptr) {
					isRowFilled = false;
					break;
				}
			}

			ret.push_back(isRowFilled);
		}
		
		return ret;
	}

	// Score based on the number of rows 
	int scoreFilledRows(std::vector<bool> rows) {
	}

	int clearFilledRows(playfield *p, std::vector<bool> rows) {
		int numCleared = 0;

		for (int i = 0; i < p->h; i++) {
			if (rows.at(i) == true) {
				clearRow(p, i);
				numCleared++;
			}
		}

		return numCleared;
	}

	int clearRow(playfield* p, int index) {
		for (int y = index; y >= 1; y--) {
			// Delete the row about to be replaced.
			for (int x = 0; x < p->w; x++) {
				deleteBlock(p, {x, y});
			}

			// Swap the rows.
			p->blocks[y] = p->blocks[y - 1];

			// Mark the row which got swapped down as empty.
			for (int x = 0; x < p->w; x++) {
				p->blocks[y - 1][x] = nullptr;
			}
		}
		
		return 0;
	}

	int bleachRow(playfield* p, int rowID) {
		for (shp::block* b: p->blocks[rowID]) {
			assert(b != nullptr);
			b->c = NONE;
		}
	}

	int rotate(playfield* p, shp::polyomino** shape) {
		if (shape == nullptr || *shape == nullptr) {return -1;}

		shp::polyomino* rotatedShape = shp::copyPolyomino(*shape);
		shp::rotatePolyomino(
				rotatedShape, shp::rotationDirection::CLOCKWISE, 1);

		deletePolynoBlocks(p, *shape);
		if (polynoMoveCheck(p, rotatedShape, rotatedShape->pos, false)) {
			shp::deinitPolyomino(shape);
			setPolynoPos(p, rotatedShape, rotatedShape->pos);
			*shape = rotatedShape;
			return  0;
		} else {
			shp::deinitPolyomino(&rotatedShape);
			setPolynoPos(p, *shape, (*shape)->pos);
			return 1;
		}
	}
}
