#include "photo_core.h"

#include <cassert>
#include <cstdint>
#include <vector>

using bc_art_photo::Image;

namespace {

Image makeRgbImage(const std::vector<std::uint8_t>& pixels) {
    return Image{1, static_cast<int>(pixels.size() / 3), 3, pixels};
}

void testBrightness() {
    auto image = makeRgbImage({10, 20, 30});
    bc_art_photo::applyBrightness(image, 20);
    assert((image.pixels == std::vector<std::uint8_t>{30, 40, 50}));
}

void testContrast() {
    auto image = makeRgbImage({100, 128, 200});
    bc_art_photo::applyContrast(image, 1.5F);
    assert(image.pixels[0] < 100);
    assert(image.pixels[1] == 128);
    assert(image.pixels[2] > 200);
}

void testGrayscale() {
    auto image = makeRgbImage({255, 0, 0});
    bc_art_photo::applyGrayscale(image);
    assert(image.pixels[0] == image.pixels[1]);
    assert(image.pixels[1] == image.pixels[2]);
}

void testInvert() {
    auto image = makeRgbImage({0, 100, 255});
    bc_art_photo::applyInvert(image);
    assert((image.pixels == std::vector<std::uint8_t>{255, 155, 0}));
}

void testHalideFallback() {
    auto image = makeRgbImage({1, 2, 3});
    bc_art_photo::applyBrightnessWithHalideFallback(image, 4);
    assert((image.pixels == std::vector<std::uint8_t>{5, 6, 7}));
}

}  // namespace

int main() {
    testBrightness();
    testContrast();
    testGrayscale();
    testInvert();
    testHalideFallback();
    return 0;
}
