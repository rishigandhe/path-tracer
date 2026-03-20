# PathTracer (C++)

Simple C++ path tracer that outputs a **PPM (P3)** image to stdout.

## Build

```bash
cd /Users/rishigandhe/PathTracer
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/main > output.ppm
python3 -c "from PIL import Image; Image.open('output.ppm').save('output.png')"
open output.png
```

## Camera

Edit these values in `raytracer/main.cpp`:

- `lookfrom` (camera position)
- `lookat` (what you’re pointing at)
- `kVerticalFov` (field of view)
- `kAperture` (depth of field; set to `0.0` for easy camera tests)

