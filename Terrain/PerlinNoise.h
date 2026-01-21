#pragma once

#include <cmath>

class PerlinNoise {
public:

	PerlinNoise(int s = 0);
	inline double noise(double x, double y) {
		// Top Left corner coordinates, then wrapped to 256
		// to be used as index of permutation array
		int X = fastFloor(x) & 255, Y = fastFloor(y) & 255;

		// Relative x and y in cell
		x -= fastFloor(x);
		y -= fastFloor(y);

		// Fade curves for x and y
		double u = fade_c(x), v = fade_c(y);

		// Hash coordinates of the 4 square corners
		int aa = p[p[X] + Y];
		int ab = p[p[X] + Y + 1];
		int ba = p[p[X + 1] + Y];
		int bb = p[p[X + 1] + Y + 1];

		// Blend results from 4 corners of the square
		double x1 = lerp(
			u,
			grad(aa, x, y),
			grad(ba, x - 1, y)
		);
		double x2 = lerp(
			u,
			grad(ab, x, y - 1),
			grad(bb, x - 1, y - 1)
		);

		// [-1, 1]
		return lerp(v, x1, x2);
	}

private:

	int p[512];
	static const int permutation[256];
	static const double grad2[16][2];

	// Classic Perlin fade function
    inline double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
	// Cheaper fade function
	inline double fade_c(double t) { return t * t * (3 - 2 * t); }
	// Linear interpolation
	inline double lerp(double t, double a, double b) { return a + t * (b - a); }
	// Get a gradient based on hash value
    inline double grad(int hash, double x, double y) {
        int h = hash & 15; 
        return grad2[h][0] * x + grad2[h][1] * y; 
	}
	// Quicker floor function ( skips some checks )
	inline int fastFloor(double x) {
		int i = (int) x; 
		return (x < i) ? (i - 1) : i;
	}
};