#include "photo_core.h"

#if __has_include(<Halide.h>)
#include <Halide.h>
#include <cstring>
#endif

namespace bc_art_photo {

void applyBrightnessWithHalideFallback(Image& image, int delta) {
    if (!image.hasValidBuffer()) {
        return;
    }

#if __has_include(<Halide.h>)
    // Halide buffers are indexed as (channel, x, y), so the dimensions map to
    // (channels, width, height) even though Image stores width/height/channels.
    Halide::Buffer<std::uint8_t> input(image.pixels.data(), image.channels, image.width, image.height);
    Halide::Buffer<std::uint8_t> output(image.channels, image.width, image.height);

    Halide::Var c;
    Halide::Var x;
    Halide::Var y;

    Halide::Func brighten;
    brighten(c, x, y) = Halide::cast<std::uint8_t>(
        Halide::clamp(Halide::cast<int>(input(c, x, y)) + delta, 0, 255));

    brighten.realize(output);

    std::memcpy(image.pixels.data(), output.data(),
                static_cast<std::size_t>(image.width * image.height * image.channels));
#else
    applyBrightness(image, delta);
#endif
}

}  // namespace bc_art_photo
