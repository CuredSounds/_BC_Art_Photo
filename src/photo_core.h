#pragma once

#include <cstdint>
#include <vector>

namespace bc_art_photo {

struct Image {
    int width;
    int height;
    int channels;
    std::vector<std::uint8_t> pixels;

    bool hasValidBuffer() const;
};

void applyBrightness(Image& image, int delta);
void applyContrast(Image& image, float factor);
void applyGrayscale(Image& image);
void applyInvert(Image& image);

void applyBrightnessWithHalideFallback(Image& image, int delta);

}  // namespace bc_art_photo
