#include "PerlinNoise2D.h"

#include <algorithm>
#include <random>

const double PerlinNoise2D::grad2[16][2] = {
	{ 1, 0}, { 0.9239, 0.3827}, { 0.7071, 0.7071}, { 0.3827, 0.9239},
	{ 0, 1}, {-0.3827, 0.9239}, {-0.7071, 0.7071}, {-0.9239, 0.3827},
	{-1, 0}, {-0.9239,-0.3827}, {-0.7071,-0.7071}, {-0.3827,-0.9239},
	{ 0,-1}, { 0.3827,-0.9239}, { 0.7071,-0.7071}, { 0.9239,-0.3827}
};


PerlinNoise2D::PerlinNoise2D(int s) {
	// Initialize permutation vector with the reference values
	for (int i = 0; i < 256; i++) p[256 + i] = p[i] = permutation[i];
	// Apply seed
	std::shuffle(p, p + 256, std::mt19937(s));
	// Copy the new seeded permutation
	for (int i = 0; i < 256; i++) p[256 + i] = p[i];
}
