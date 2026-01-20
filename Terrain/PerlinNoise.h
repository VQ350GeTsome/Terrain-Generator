#pragma once
class PerlinNoise {
public:

	PerlinNoise(int s = 0);
	double noise(double x, double y);

private:

	int p[512];
	static const int permutation[256];
	static const int grad2[8][2];

    inline double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
	inline double lerp(double t, double a, double b) { return a + t * (b - a); }
    inline double grad(int hash, double x, double y) {
        int h = hash & 7; 
        return grad2[h][0] * x + grad2[h][1] * y; }
};