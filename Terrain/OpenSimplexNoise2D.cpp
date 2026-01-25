#include "OpenSimplexNoise2D.h"

#include <algorithm>
#include <random>

const double OpenSimplexNoise2D::F2 = 0.366025403784439,
			 OpenSimplexNoise2D::G2 = 0.211324865405187;

const double OpenSimplexNoise2D::grad2[24][2] = {
	{ 1.000000000000000,  0.000000000000000 },
	{ 0.965925826289068,  0.258819045102521 },
	{ 0.866025403784439,  0.500000000000000 },
	{ 0.707106781186548,  0.707106781186548 },
	{ 0.500000000000000,  0.866025403784439 },
	{ 0.258819045102521,  0.965925826289068 },

	{  0.000000000000000,  1.000000000000000 },
	{ -0.258819045102521,  0.965925826289068 },
	{ -0.500000000000000,  0.866025403784439 },
	{ -0.707106781186548,  0.707106781186548 },
	{ -0.866025403784439,  0.500000000000000 },
	{ -0.965925826289068,  0.258819045102521 },

	{ -1.000000000000000,  0.000000000000000 },
	{ -0.965925826289068, -0.258819045102521 },
	{ -0.866025403784439, -0.500000000000000 },
	{ -0.707106781186548, -0.707106781186548 },
	{ -0.500000000000000, -0.866025403784439 },
	{ -0.258819045102521, -0.965925826289068 },

	{ 0.000000000000000, -1.000000000000000 },
	{ 0.258819045102521, -0.965925826289068 },
	{ 0.500000000000000, -0.866025403784439 },
	{ 0.707106781186548, -0.707106781186548 },
	{ 0.866025403784439, -0.500000000000000 },
	{ 0.965925826289068, -0.258819045102521 }
};




OpenSimplexNoise2D::OpenSimplexNoise2D(int s) {
	// Initialize permutation vector with the reference values
	for (int i = 0; i < 256; i++) p[256 + i] = p[i] = permutation[i];
	// Apply seed
	std::shuffle(p, p + 256, std::mt19937(s));
	// Copy the new seeded permutation
	for (int i = 0; i < 256; i++) p[256 + i] = p[i];
}

double OpenSimplexNoise2D::get(double i, double j) {

	// Skew
	double s = (i + j) * F2;
	int u = static_cast<int>(fastFloor(i + s));
	int v = static_cast<int>(fastFloor(j + s));

	// Compute position in triangle
	double t = (u + v) * G2;
	double x = u - t, y = v - t;

	// Relative x, y in triangle
	double dx0 = i - x;
	double dy0 = j - y;

	// Determine which triangle we are in
	int i1, j1;
	// Upper triangle
	if (dx0 > dy0) {  i1 = 1; j1 = 0; } 
	// Lower triangle
	else { i1 = 0; j1 = 1; } 

	// Offsets for corners
	double dx1 = dx0 - i1 + G2;
	double dy1 = dy0 - j1 + G2;
	double dx2 = dx0 - 1.0 + 2.0 * G2;
	double dy2 = dy0 - 1.0 + 2.0 * G2;

	// Hashed gradient indices of the three corners
	int gi0 = p[(u + p[v & 255]) & 255] % 24;
	int gi1 = p[(u + i1 + p[(v + j1) & 255]) & 255] % 24;
	int gi2 = p[(u + 1 + p[(v + 1) & 255]) & 255] % 24;

	// Contributions from the three corners
	double n0, n1, n2;

	double t0 = 0.5 - dx0 * dx0 - dy0 * dy0;
	if (t0 < 0) n0 = 0.0;
	else {
		t0 *= t0;
		n0 = t0 * t0 * dot(grad2[gi0], dx0, dy0);
	}

	double t1 = 0.5 - dx1 * dx1 - dy1 * dy1;
	if (t1 < 0) n1 = 0.0;
	else {
		t1 *= t1;
		n1 = t1 * t1 * dot(grad2[gi1], dx1, dy1);
	}

	double t2 = 0.5 - dx2 * dx2 - dy2 * dy2;
	if (t2 < 0) n2 = 0.0;
	else {
		t2 *= t2;
		n2 = t2 * t2 * dot(grad2[gi2], dx2, dy2);
	}

	return 70.0 * (n0 + n1 + n2);
}
