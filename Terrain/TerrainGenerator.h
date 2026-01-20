#pragma once

#include "PerlinNoise.h"

#include <cstdint>
#include <cstdlib>
#include <random>

class TerrainGenerator {
public:

	TerrainGenerator(int width, int height) : w(width), h(height) {
		// Initialize random number generator
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> dist(-50000, 50000);

		// Initialize Perlin noise generators with random seeds
		humiNoise = new PerlinNoise(dist(rng));
		tempNoise = new PerlinNoise(dist(rng));
		elevNoise = new PerlinNoise(dist(rng));
	}
	~TerrainGenerator() {
		delete humiNoise;
		delete tempNoise;
		delete elevNoise;
	}

	// Generates all the terrain features
	void generateTerrain();

	// Getters for terrain features
	inline double getHumidity(int x, int y) { return humidity[y * w + x]; }
	inline double getTemperature(int x, int y) { return temperature[y * w + x]; }
	inline double getElevation(int x, int y) { return elevation[y * w + x];  }

	inline uint32_t get(int x, int y) {
		double h = getHumidity(x, y);
		int gray = static_cast<uint8_t>(h * 255);
		return (0xFF << 24) | (gray << 16) | (gray << 8) | gray;
	}

private:

	// Width, height
	int w, h;

	// Noise object	
	PerlinNoise *humiNoise,
				*tempNoise,
				*elevNoise;

	// Scales for different terrain features
	float humiScale = 32,
		  tempScale = 32,
		  elevScale = 32;

	// Data arrays for terrain features
	double *humidity = nullptr,
		   *temperature = nullptr,
		   *elevation = nullptr;
};

