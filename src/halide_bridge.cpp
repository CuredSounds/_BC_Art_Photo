#include "photo_core.h"

#if __has_include(<Halide.h>)
#include <Halide.h>
#endif

namespace bc_art_photo {

void applyBrightnessWithHalideFallback(Image& image, int delta) {
    if (!image.hasValidBuffer()) {
        return;
    }

#if __has_include(<Halide.h>)
    Halide::Buffer<std::uint8_t> input(image.pixels.data(), image.channels, image.width, image.height);
    Halide::Buffer<std::uint8_t> output(image.channels, image.width, image.height);

    Halide::Var c;
    Halide::Var x;
    Halide::Var y;

    Halide::Func brighten;
    brighten(c, x, y) = Halide::cast<std::uint8_t>(
        Halide::clamp(Halide::cast<int>(input(c, x, y)) + delta, 0, 255));

    brighten.realize(output);

    for (int yPos = 0; yPos < image.height; ++yPos) {
        for (int xPos = 0; xPos < image.width; ++xPos) {
            for (int channel = 0; channel < image.channels; ++channel) {
                image.pixels[static_cast<std::size_t>((yPos * image.width + xPos) * image.channels + channel)] =
                    output(channel, xPos, yPos);
            }
        }
    }
#else
    applyBrightness(image, delta);
#endif
}

}  // namespace bc_art_photo
