#include "PerlinNoise.h"

#include <cmath>

PerlinNoise::PerlinNoise() {
	for (int i = 0; i < 256; i++) p[256 + i] = p[i] = permutation[i];
}

double PerlinNoise::noise(double x, double y) {

	// Top Left corner coordinates, then wrapped to 256
	// to be used as index of permutation array
	int X = (int) floor(x) & 255, Y = (int) floor(y) & 255;

	// Relative x and y in cell
	x -= floor(x);
	y -= floor(y);

	// Fade curves for x and y
	double u = fade(x), v = fade(y);

	int aa = p[p[X] + Y];
	int ab = p[p[X] + Y + 1];
	int ba = p[p[X + 1] + Y];
	int bb = p[p[X + 1] + Y + 1];

	double res = (1 - u) * (1 - v) * (aa + (x * (ba - aa))) +
				 u * (1 - v) * (ba + ((x - 1) * (bb - ba))) +
				 (1 - u) * v * (ab + (x * (bb - ab))) +
				 u * v * (bb + ((x - 1) * (ab - bb)));

	// Scale to [0,1]
	return (res + 1.0) / 2.0;
}
