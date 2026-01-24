#pragma once
class Noise {
public:
	virtual double get(double x, double y) = 0;

protected:

	static const int permutation[256];

	// Quicker floor function ( skips some checks )
	inline int fastFloor(double x) {
		int i = (int)x;
		return (x < i) ? (i - 1) : i;
	}
};

