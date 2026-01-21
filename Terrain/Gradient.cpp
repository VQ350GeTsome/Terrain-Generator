#include "Gradient.h"

void Gradient::updatePalette() {
    for (int i = 0; i < gradientSize; i++) {
        double t = (double)i / (gradientSize - 1);

        int leftIndex = 0;
        int rightIndex = colorsCount - 1;

        for (int j = 0; j < colorsCount - 1; j++) {
            if (positions[j] <= t && t <= positions[j + 1]) {
                leftIndex = j;
                rightIndex = j + 1;
                break;
            }
        }

        double localT =
            (t - positions[leftIndex]) /
            (positions[rightIndex] - positions[leftIndex]);

        uint8_t r1 = (colors[leftIndex] >> 16) & 0xFF;
        uint8_t g1 = (colors[leftIndex] >> 8) & 0xFF;
        uint8_t b1 = colors[leftIndex] & 0xFF;

        uint8_t r2 = (colors[rightIndex] >> 16) & 0xFF;
        uint8_t g2 = (colors[rightIndex] >> 8) & 0xFF;
        uint8_t b2 = colors[rightIndex] & 0xFF;

        uint8_t r = r1 + (r2 - r1) * localT;
        uint8_t g = g1 + (g2 - g1) * localT;
        uint8_t b = b1 + (b2 - b1) * localT;

        palette[i] = (r << 16) | (g << 8) | b;
    }
}