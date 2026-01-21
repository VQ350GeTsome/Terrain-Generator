#pragma once

#include "PerlinNoise.h"
#include "Gradient.h"

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

		humiGrad->setSize(256);
		tempGrad->setSize(256);
		elevGrad->setSize(256);
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
		// Get terrain features and normalize to [0, 1]
		double h = getHumidity(x, y), t = getTemperature(x, y), e = getElevation(x, y);
		h = (h + 1) / 2;
		t = (t + 1) / 2;
		e = (e + 1) / 2;
		
		uint32_t color = parseColor(h, t, e);
		double s = (color >> 24) & 0xFF;
		s /= 255; s += 0.5;

		// Add some variation to brightness based on hash
		double b = 0.95 + (hash(x, y, e * h * t) / 10.0);

		return scale(color, b * s);
	}

	// Use the gradient to get color based on elevation
	inline uint32_t getGradient(int x, int y) {
		double h = getHumidity(x, y), t = getTemperature(x, y), e = getElevation(x, y);
		h = (h + 1) / 2;
		t = (t + 1) / 2;
		e = (e + 1) / 2;

		uint32_t humiColor = humiGrad->getColor(static_cast<int>(h * elevGrad->getSize())),
				 tempColor = tempGrad->getColor(static_cast<int>(t * tempGrad->getSize())),
				 elevColor = elevGrad->getColor(static_cast<int>(e * elevGrad->getSize()));

		uint8_t r = (uint8_t)((((humiColor >> 16) & 0xFF) +
							  ((tempColor >> 16) & 0xFF) +
							  ((elevColor >> 16) & 0xFF)) / 3);

		uint8_t g = (uint8_t)((((humiColor >> 8) & 0xFF) +
							  ((tempColor >> 8) & 0xFF) +
							  ((elevColor >> 8) & 0xFF)) / 3);

		uint8_t b = (uint8_t)(((humiColor & 0xFF) +
							  (tempColor & 0xFF) +
							  (elevColor & 0xFF)) / 3);

		return (0xFF << 24) | (r << 16) | (g << 8) | b;
	}

	// Simple RGB encoding of terrain features, humidity->R, temperature->G, elevation->B
	inline uint32_t getRGB(int x, int y) {
		double h = getHumidity(x, y), t = getTemperature(x, y), e = getElevation(x, y);

		int r = static_cast<uint8_t>(255 * h);
		int g = static_cast<uint8_t>(255 * t);
		int b = static_cast<uint8_t>(255 * e);
		return (0xFF << 24) | (r << 16) | (g << 8) | b;
	}

private:

	// Width, height
	int w, h;

	// Orgin coordinates
	int ox = 0, oy = 0;

	// Colors
	Gradient *humiGrad = new Gradient(),
			 *tempGrad = new Gradient(),
			 *elevGrad = new Gradient();

	// Noise object	
	PerlinNoise *humiNoise,
				*tempNoise,
				*elevNoise;

	// Scales for different terrain features
	float humiScale = 64,
		  tempScale = 64,
		  elevScale = 64,
		globalScale =  1;

	// Data arrays for terrain features
	double *humidity = nullptr,
		   *temperature = nullptr,
		   *elevation = nullptr;

	// Amount of octaves for fractal noise
	int fractalOctaves = 8;

	inline double fractal(PerlinNoise* noise, double x, double y, int times) {

		// Track total noise value, frequency and amplitude
		double total = 0, frequency = 1, amplitude = 1;
		// Used to normalize result to [0, 1]
		double maxValue = 0;  

		for (int i = 0; i < times; i++) {
			total += noise->noise(x * frequency, y * frequency) * amplitude;
			maxValue += amplitude;
			amplitude *= 0.5;
			frequency *= 2;
		}
		return total/ maxValue;
	}

	inline uint32_t scale(uint32_t c, double f) {
		uint8_t r = c >> 16 & 0xFF, g = c >> 8 & 0xFF, b = c & 0xFF;
		int nr = r * f, ng = g * f, nb = b * f;

		if (nr > 255) nr = 255;
		if (ng > 255) ng = 255;
		if (nb > 255) nb = 255;

		return (0xFF << 24) | (nr << 16) | (ng << 8) | nb;
	}

	// Hashing function so we get a brightness value
	inline double hash(int a, int b, double v) {
		uint64_t c;
		memcpy(&c, &v, sizeof(double));  

		uint32_t h = 0;
		h ^= mix((uint32_t)(a));
		h ^= mix((uint32_t)(b));
		h ^= mix((uint32_t)(c >> 32));
		h ^= mix((uint32_t)(c & 0xFFFFFFFF));

		return (mix(h) % 100000) / 100000.0;
	}

	// A mixing function for hashing
	inline uint32_t mix(uint32_t x) {
		x ^= x >> 16;
		x *= 0x51fba012;
		x ^= x >> 15;
		x *= 0x924da104;
		x ^= x >> 16;
		return x;
	}

	inline uint32_t parseColor(double h, double t, double e) {

		if (e > 0.75)      { return 0x88AAAAFF; }
		else if (e > 0.70) { return 0x88888888; }
		else if (e > 0.65) { return 0x88888866; }
		else if (e > 0.55) { return 0x8822AA22; }
		else if (e > 0.50) { return 0x88AAAA22; }
		else if (e > 0.10) { return 0x882222AA; }
		else			   { return 0x88000099; }
	}

};

