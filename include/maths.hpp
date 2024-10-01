#ifndef __MATHS_HPP__
#define __MATHS_HPP__

namespace mth {
	struct vect2D {
		int x, y;

		inline bool const operator==(const vect2D &rhs) const {
			return (this->x == rhs.x && this->y == rhs.y);
		}

		inline vect2D const operator+(const vect2D &rhs) const {
			return vect2D(this->x + rhs.x, this->y + rhs.y);
		}

		inline vect2D const operator-(const vect2D &rhs) const {
			return vect2D(this->x - rhs.x, this->y - rhs.y);
		}
	};

	struct vect2DF {
		float x, y;

		inline bool const operator==(const vect2DF &rhs) const {
			return (this->x == rhs.x && this->y == rhs.y);
		}

		inline vect2DF const operator+(const vect2DF &rhs) const {
			return vect2DF(this->x + rhs.x, this->y + rhs.y);
		}

		inline vect2DF const operator-(const vect2DF &rhs) const {
			return vect2DF(this->x - rhs.x, this->y - rhs.y);
		}
	};

	struct box {
		int x, y, h, w;
	};

	inline bool checkBoxCollision(mth::box a, mth::box b) {
		return !((a.y + a.h <= b.y) || (a.y >= b.y + b.h)                      
				||   (a.x + a.w <= b.x) || (a.x >= b.x + b.w));
	}
}

#endif
