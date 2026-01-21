#pragma once
#include <cstdint>
#include <cstring>

class Gradient {

public:

    Gradient() {
        colorsCount = 8;

        uint32_t defaultColors[8] = {
            0x000764, 0x206BCB, 0xEDFFFF, 0xFFAA00,
            0x000200, 0x000764, 0x206BCB, 0xEDFFFF
        };

        float defaultPositions[8] = {
            0.000f, 0.125f, 0.250f, 0.375f,
            0.500f, 0.625f, 0.750f, 1.000f
        };

        colors = new uint32_t[colorsCount];
        positions = new float[colorsCount];

        std::memcpy(colors, defaultColors, sizeof(defaultColors));
        std::memcpy(positions, defaultPositions, sizeof(defaultPositions));

        palette = new uint32_t[gradientSize];
        updatePalette();
    }
    Gradient(int colorsAmount, uint32_t* colorsArr, float* positionsArr)
        : colorsCount(colorsAmount) {
        colors = new uint32_t[colorsCount];
        positions = new float[colorsCount];

        std::memcpy(colors, colorsArr, colorsCount * sizeof(uint32_t));
        std::memcpy(positions, positionsArr, colorsCount * sizeof(float));

        palette = new uint32_t[gradientSize];
        updatePalette();
    }
    ~Gradient() {
        delete[] colors;
        delete[] positions;
        delete[] palette;
    }

    inline uint32_t getColor(int pos) const {
        if (pos == err) return insideColor;
        return palette[(pos % gradientSize + gradientSize) % gradientSize];
    }

    inline void setErrorColor(uint32_t color, int error) {
        insideColor = color;
        err = error;
    }

    void updatePalette();

    inline void setSize(int delta) {
        gradientSize += delta;
        if (gradientSize < 2) gradientSize = 2;
        delete[] palette;
        palette = new uint32_t[gradientSize];
        updatePalette();
    }
    inline int getSize() const {
		return gradientSize;
	}

private:
    int gradientSize = 32;

    int colorsCount = 0;
    uint32_t* colors = nullptr;
    float* positions = nullptr;

    uint32_t* palette = nullptr;

    uint32_t insideColor = 0x000000;
    int err = -1;
};
