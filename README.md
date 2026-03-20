```
# PathTracer

A C++ path tracer built from scratch. Outputs a PPM image to stdout.

Supports diffuse, metal, and glass materials with soft shadows, depth of field, and antialiasing.

## Build
```bash
cmake -S . -B build
cmake --build build
```

## Run
```bash
./build/main > output.ppm
```

To convert to PNG:
```bash
python3 -c "from PIL import Image; Image.open('output.ppm').save('output.png')"
open output.png
```

## Camera
Edit these values in `raytracer/main.cpp`:

| Parameter | Description |
|-----------|-------------|
| `lookfrom` | Camera position |
| `lookat` | Target point |
| `kVerticalFov` | Field of view in degrees |
| `kAperture` | Depth of field blur (0.0 to disable) |
```
