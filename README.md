# BC Art Photo

A lightweight Photoshop-style prototype with core drawing and photo manipulation tools.

## Tech stack

- **C++**: image manipulation core (`src/photo_core.cpp`)
- **Halide**: optional acceleration path (`src/halide_bridge.cpp`) used automatically when `Halide.h` is available
- **HTML/CSS/JavaScript**: browser UI (`web/`) for drawing and live adjustments

## Features

- Brush and eraser drawing tools
- Line and rectangle tools
- Image upload and PNG export
- Brightness, contrast, saturation sliders
- Grayscale and invert operations

## Build and test C++ core

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Run web UI

Open `/home/runner/work/_BC_Art_Photo/_BC_Art_Photo/web/index.html` in a browser.
