#include "TerrainGenerator.h"

void TerrainGenerator::generateTerrain() {

	// Allocate space for terrain features
	humidity    = new double[w * h];
	temperature = new double[w * h];
	elevation   = new double[w * h];

	// Generate humidity
	for (int y = 0; h > y; y++) for (int x = 0; w > x; x++) {

		double sx = ((x + ox) / humiScale), sy = ((y + oy) / humiScale);

		humidity[y * w + x] = fractal(humiNoise, sx, sy, fractalOctaves);
	}
	
	// Generate temperature
	for (int y = 0; h > y; y++) for (int x = 0; w > x; x++) {

		double sx = ((x + ox) / tempScale), sy = ((y + oy) / tempScale);

		temperature[y * w + x] = fractal(tempNoise, sx, sy, fractalOctaves);
	}

	// Generate elevation
	for (int y = 0; h > y; y++) for (int x = 0; w > x; x++) {

		double sx = ((x + ox) / elevScale), sy = ((y + oy) / elevScale);

		double continent = elevNoise->noise(sx / 8.0, sy / 8.0);
		elevation[y * w + x] = fractal(elevNoise, sx, sy, fractalOctaves) * 0.50 + continent;
	}
}