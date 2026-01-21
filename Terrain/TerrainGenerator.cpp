#include "TerrainGenerator.h"

void TerrainGenerator::generateTerrain() {

	// Allocate space for terrain features
	humidity    = new double[w * h];
	temperature = new double[w * h];
	elevation   = new double[w * h];

	// Generate humidity
	#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			double sx = ((x + ox) / (humiScale / globalScale)), sy = ((y + oy) / (humiScale / globalScale));
			humidity[y * w + x] = fractal(humiNoise, sx, sy, fractalOctaves);
		}
	}
	
	// Generate temperature
	#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			double sx = ((x + ox) / (tempScale / globalScale)), sy = ((y + oy) / (tempScale / globalScale));
			temperature[y * w + x] = fractal(tempNoise, sx, sy, fractalOctaves);
		}
	}

	// Generate elevation
	#pragma omp parallel for schedule(dynamic)
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			double sx = ((x + ox) / (elevScale / globalScale)), sy = ((y + oy) / (elevScale / globalScale));
			double continent = elevNoise->noise(sx / 8.0, sy / 8.0);
			elevation[y * w + x] = fractal(elevNoise, sx, sy, fractalOctaves) * 0.50 + continent;

		}
	}
}