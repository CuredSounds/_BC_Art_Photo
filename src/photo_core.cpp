#include "photo_core.h"

#include <algorithm>
#include <cmath>

namespace bc_art_photo {
namespace {

constexpr float kRedWeight = 0.299F;
constexpr float kGreenWeight = 0.587F;
constexpr float kBlueWeight = 0.114F;

std::uint8_t clampToByte(int value) {
    return static_cast<std::uint8_t>(std::clamp(value, 0, 255));
}

}  // namespace

bool Image::hasValidBuffer() const {
    return width > 0 && height > 0 && channels >= 3 &&
           pixels.size() == static_cast<std::size_t>(width * height * channels);
}

void applyBrightness(Image& image, int delta) {
    if (!image.hasValidBuffer()) {
        return;
    }

    for (std::size_t i = 0; i < image.pixels.size(); i += image.channels) {
        image.pixels[i] = clampToByte(static_cast<int>(image.pixels[i]) + delta);
        image.pixels[i + 1] = clampToByte(static_cast<int>(image.pixels[i + 1]) + delta);
        image.pixels[i + 2] = clampToByte(static_cast<int>(image.pixels[i + 2]) + delta);
    }
}

void applyContrast(Image& image, float factor) {
    if (!image.hasValidBuffer() || !std::isfinite(factor)) {
        return;
    }

    for (std::size_t i = 0; i < image.pixels.size(); i += image.channels) {
        image.pixels[i] = clampToByte(static_cast<int>((static_cast<float>(image.pixels[i]) - 128.0F) * factor + 128.0F));
        image.pixels[i + 1] = clampToByte(static_cast<int>((static_cast<float>(image.pixels[i + 1]) - 128.0F) * factor + 128.0F));
        image.pixels[i + 2] = clampToByte(static_cast<int>((static_cast<float>(image.pixels[i + 2]) - 128.0F) * factor + 128.0F));
    }
}

void applyGrayscale(Image& image) {
    if (!image.hasValidBuffer()) {
        return;
    }

    for (std::size_t i = 0; i < image.pixels.size(); i += image.channels) {
        const int gray = static_cast<int>(kRedWeight * image.pixels[i] + kGreenWeight * image.pixels[i + 1] +
                                          kBlueWeight * image.pixels[i + 2]);
        const auto grayByte = clampToByte(gray);
        image.pixels[i] = grayByte;
        image.pixels[i + 1] = grayByte;
        image.pixels[i + 2] = grayByte;
    }
}

void applyInvert(Image& image) {
    if (!image.hasValidBuffer()) {
        return;
    }

    for (std::size_t i = 0; i < image.pixels.size(); i += image.channels) {
        image.pixels[i] = static_cast<std::uint8_t>(255 - image.pixels[i]);
        image.pixels[i + 1] = static_cast<std::uint8_t>(255 - image.pixels[i + 1]);
        image.pixels[i + 2] = static_cast<std::uint8_t>(255 - image.pixels[i + 2]);
    }
}

}  // namespace bc_art_photo
