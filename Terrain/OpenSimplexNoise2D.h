#pragma once

#include "Noise.h"

class OpenSimplexNoise2D : public Noise {
public:
	OpenSimplexNoise2D(int s);

	double get(double x, double y);

private:

	// F2 = (sqrt(3)-1)/2 , G2 = (3-sqrt(3))/6
	static const double F2, G2;
	int p[512];
	static const double grad2[24][2];

	inline double dot(const double *g, double x, double y) {
		return g[0] * x + g[1] * y;
	}

};

