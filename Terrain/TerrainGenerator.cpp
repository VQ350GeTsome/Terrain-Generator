#include "TerrainGenerator.h"

void TerrainGenerator::generateTerrain() {

	// Allocate space for terrain features
	humidity = new double[w * h];
	temperature = new double[w * h];
	elevation = new double[w * h];

	// Generate humidity
	for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
			double nx = (double) x / (double) w * humiScale;
			double ny = (double) y / (double) h * humiScale;
			humidity[y * w + x] = humiNoise->noise(nx, ny);
		}
	
	// Generate temperature
	for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
			double nx = (double) x / (double) w * tempScale;
			double ny = (double) y / (double) h * tempScale;
			temperature[y * w + x] = tempNoise->noise(nx, ny);
		}

	// Generate elevation
	for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
			double nx = (double) x / (double) w * elevScale;
			double ny = (double) y / (double) h * elevScale;
			elevation[y * w + x] = elevNoise->noise(nx, ny);
		}
}