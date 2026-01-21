#pragma once

#include "PerlinNoise.h"
#include "Gradient.h"

#include <cstdint>
#include <cstdlib>
#include <random>
#include <string>

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

	// Gets raw humidity at given coordinates
	inline double getHumidity(int x, int y) { return humidity[y * w + x]; }
	// Gets raw temperature at given coordinates
	inline double getTemperature(int x, int y) { return temperature[y * w + x]; }
	// Gets raw elevation at given coordinates
	inline double getElevation(int x, int y) { return elevation[y * w + x];  }

	// Gets the actual terrain color
	inline uint32_t get(int x, int y) {
		// Get terrain features and normalize to [0, 1]
		double h = getHumidity(x, y), t = getTemperature(x, y), e = getElevation(x, y);
		h = (h + 1) / 2;
		t = (t + 1) / 2;
		e = (e + 1) / 2;
		
		// Get base color from terrain features
		uint32_t color = parseColor(h, t, e);

		// Get randomness scale
		double s = ((color >> 24) & 0xFF) / 255.0;

		// Add some variation to brightness based on hash
		double r = hash(x, y, h + t + e);
		double range = 1.0 * s;
		double b = (1 - (range / 2)) + (r * range);

		return scale(color, b);
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

	// Get info string about terrain features at given coordinates
	inline std::string getInfo(int x, int y) {
		// Get terrain features and normalize to [0, 1]
		double h = getHumidity(x, y), t = getTemperature(x, y), e = getElevation(x, y);
		h = (h + 1) / 2;
		t = (t + 1) / 2;
		e = (e + 1) / 2;

		std::string info = "Coordinates:\t(" + std::to_string(x + ox) + ", " + std::to_string(y + oy) + ")\n" +
				           "Humidity:\t"    + std::to_string(h) + "\n" +
						   "Temperature:\t" + std::to_string(t) + "\n" +
						   "Elevation:\t"   + std::to_string(e) + "\n";

		return info;
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
		globalScale =  2;

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

		     if (e > 0.85)	{ return 0x33EEEEEE; } // Snow caps
		else if (e > 0.75)	{ return 0x22AAAAFF; } // Mountain peaks
		else if (e > 0.70)	{ return 0x22888888; } // Mountain highs
		else if (e > 0.65)	{ return 0x22888866; } // Mountain sides

		else if (e > 0.60)	{ return 0x33228822; } // High lands
		else if (e > 0.55)	{ return 0x3344AA44; } // Low lands

		else if (e > 0.53)	{ return 0x2288AA88; } // Beach
		else if (e > 0.50)	{ return 0x22AAAA22; } // Sand	

		else if (e > 0.48)	{ return 0x2200AAFF; } // Shallow water
		else if (e > 0.46)	{ return 0x226699FF; } // Ocean
		else if (e > 0.44)	{ return 0x223366FF; } // Deep water
		else				{ return 0x55000099; } // Deep ocean
	}

};

