#include "SimplexNoise2D.h"

#include <algorithm>
#include <random>

const double SimplexNoise2D::F2 = 0.366025403784439,
		     SimplexNoise2D::G2 = 0.211324865405187;

SimplexNoise2D::SimplexNoise2D(int s) {
	// Initialize permutation vector with the reference values
	for (int i = 0; i < 256; i++) p[256 + i] = p[i] = permutation[i];
	// Apply seed
	std::shuffle(p, p + 256, std::mt19937(s));
	// Copy the new seeded permutation
	for (int i = 0; i < 256; i++) p[256 + i] = p[i];
}

double SimplexNoise2D::get(double i, double j) {

	// Skew
	double s = (i + j) * F2;
	int u = static_cast<int>(fastFloor(i + s)),
		v = static_cast<int>(fastFloor(j + s));

	// Compute position in triangle
	double t = (u + v) * G2;
	double x = u + t, y = v + t;


	return 0.0;
}
