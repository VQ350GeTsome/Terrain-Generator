#pragma once

#include "Noise.h"

class SimplexNoise2D : public Noise {
public:
	SimplexNoise2D(int s);

	double get(double x, double y);

private:

	// F2 = (sqrt(3)-1)/2 , G2 = (3-sqrt(3))/6
	static const double F2, G2;

	int p[512];

};

