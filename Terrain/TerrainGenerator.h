#pragma once

#include "PerlinNoise.h"

class TerrainGenerator {
public:
	TerrainGenerator();
	~TerrainGenerator();
	void generateTerrain(int width, int height);

private:

	PerlinNoise *perlinNoise = new PerlinNoise();

	// Scales for different terrain features
	float humidityScale = 1,
		temperatureScale = 1,
		elevationScale = 1;
};

