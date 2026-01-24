#include "TerrainGenerator.h"

void TerrainGenerator::generateTerrain() {

	// Allocate space for terrain features
	humidity    = new double[w * h];
	temperature = new double[w * h];
	elevation   = new double[w * h];

	// Generate humidity
	#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < h; y++) {
		double wy = oy + y / globalScale, sy = wy / humiScale;
		double wx = ox, sx = wx / humiScale;
		double dx = 1.0 / (globalScale * humiScale);
		for (int x = 0; x < w; x++) {
			humidity[y * w + x] = fractal(humiNoise, sx, sy, fractalOctaves);
			sx += dx;
		}
	}

	
	// Generate temperature
	#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < h; y++) {
		double wy = oy + y / globalScale, sy = wy / tempScale;
		double wx = ox, sx = wx / tempScale;
		double dx = 1.0 / (globalScale * tempScale);
		for (int x = 0; x < w; x++) {
			temperature[y * w + x] = fractal(tempNoise, sx, sy, fractalOctaves);
			sx += dx;
		}
	}

	// Generate elevation
	#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < h; y++) {
		double wy = oy + y / globalScale, sy = wy / elevScale;
		double wx = ox, sx = ox / elevScale;
		double dx = 1.0 / (globalScale * elevScale);
		for (int x = 0; x < w; x++) {
			double continent = elevNoise->get(sx / 8.0, sy / 8.0);
			elevation[y * w + x] = fractal(elevNoise, sx, sy, fractalOctaves) * 0.50 + continent;
			sx += dx;
		}
	}
}