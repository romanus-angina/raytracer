# raytracer

A basic raytracer implementation in C++ that renders 3D scenes with realistic lighting, reflections, and refractions. This project follows the concepts from [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) and creates photorealistic images of spheres with different materials.

## Features

- **Realistic rendering**: Generates images with proper lighting, shadows, and reflections
- **Multiple materials**: 
  - Lambertian (matte/diffuse surfaces)
  - Metal (reflective surfaces with controllable fuzziness)
  - Dielectric (glass-like materials with refraction)
- **Anti-aliasing**: Samples multiple rays per pixel for smooth edges
- **Configurable camera**: Adjustable field of view, image resolution, and sample count
- **PPM image output**: Creates standard image files viewable in most image viewers

## Project Structure

```
raytracer/
├── src/
│   ├── main.cc              # Main program and scene setup
│   ├── camera.h             # Camera class with rendering logic
│   ├── material.h           # Material classes (lambertian, metal, dielectric)
│   ├── sphere.h             # Sphere geometry implementation
│   ├── hittable.h           # Base class for renderable objects
│   ├── hittable_list.h      # Container for multiple objects
│   ├── ray.h                # Ray class for ray-object intersection
│   ├── vec3.h               # 3D vector math utilities
│   ├── color.h              # Color handling and gamma correction
│   ├── interval.h           # Range/interval utilities
│   └── constants.h          # Mathematical constants and utilities
├── CMakeLists.txt           # Build configuration
└── README.md
```

## Prerequisites

- **C++ compiler** with C++11 support (GCC, Clang, or MSVC)
- **CMake** version 3.1 or higher
- **Command line tools** (Xcode Command Line Tools on macOS)

### Installing Prerequisites on macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake using Homebrew
brew install cmake
```

### Installing Prerequisites on Linux

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake

# Fedora/RHEL
sudo dnf install gcc-c++ cmake make
```

## Building the Project

1. **Clone or download the project files** to a directory called `raytracer`

2. **Navigate to the project directory**:
   ```bash
   cd raytracer
   ```

3. **Configure the build**:
   ```bash
   cmake -B build
   ```

4. **Compile the raytracer**:
   ```bash
   cmake --build build
   ```

## Running the Raytracer

Generate an image and open it:

```bash
# Render the scene to a PPM file
build/raytracer > image.ppm

# View the image (macOS)
open image.ppm

# View the image (Linux)
xdg-open image.ppm
```

### Converting to Common Formats

PPM files can be converted to more common formats:

```bash
# Convert to PNG using ImageMagick
convert image.ppm image.png

# Convert to PNG using GIMP (GUI)
gimp image.ppm

# Convert using online tools or other image viewers
```

## Customizing the Scene

Edit `src/main.cc` to modify the rendered scene:

### Camera Settings

```cpp
camera cam;
cam.aspect_ratio = 16.0 / 9.0;    // Image aspect ratio
cam.image_width = 400;            // Image width in pixels
cam.samples_per_pixel = 100;      // Anti-aliasing samples (higher = smoother)
cam.max_depth = 50;               // Maximum ray bounces
cam.vfov = 90.0;                  // Vertical field of view in degrees
```

### Adding Objects

```cpp
// Create materials
auto red_material = std::make_shared<lambertian>(color(0.7, 0.3, 0.3));
auto metal_material = std::make_shared<metal>(color(0.8, 0.8, 0.9), 0.1);
auto glass_material = std::make_shared<dielectric>(1.5);

// Add spheres to the scene
world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5, red_material));
world.add(std::make_shared<sphere>(point3(-1, 0, -1), 0.5, glass_material));
world.add(std::make_shared<sphere>(point3(1, 0, -1), 0.5, metal_material));
```

### Material Types

- **Lambertian**: `lambertian(color(r, g, b))` - Matte surfaces
- **Metal**: `metal(color(r, g, b), fuzziness)` - Reflective surfaces (fuzziness 0.0-1.0)
- **Dielectric**: `dielectric(refractive_index)` - Glass-like materials (typical values: 1.3-2.4)

## Render Quality vs Speed

The rendering time depends on several factors:

- **Image resolution**: Higher `image_width` increases render time quadratically
- **Samples per pixel**: More samples = smoother images but longer renders
- **Max depth**: Higher values allow more realistic lighting but slower renders
- **Scene complexity**: More objects = longer intersection calculations

### Recommended Settings

```cpp
cam.image_width = 400;
cam.samples_per_pixel = 100;
cam.max_depth = 50;
```

## Troubleshooting

### Build Issues

**"cmake: command not found"**
- Install CMake using your system's package manager

**Compilation errors**
- Ensure you have a C++11 compatible compiler
- Check that all header files are in the `src/` directory

### Runtime Issues

**Segmentation fault**
- Usually indicates missing material assignments
- Check that all spheres have valid materials

**Black or incorrect images**
- Verify camera settings and object positions
- Ensure objects are positioned in front of the camera (negative z-coordinates)

**Very long render times**
- Reduce `samples_per_pixel` or `image_width` for testing
- Lower `max_depth` if scenes have many reflective/refractive objects

## Technical Details

This raytracer implements:

- **Ray-sphere intersection** using quadratic equation solving
- **Lambertian reflection** for diffuse materials
- **Metal reflection** with Fresnel-like behavior
- **Snell's law refraction** for dielectric materials
- **Schlick's approximation** for realistic glass reflection
- **Gamma correction** for proper color display
- **Jittered sampling** for anti-aliasing

The rendering process traces rays from the camera through each pixel, calculating intersections with objects and recursively following reflected and refracted rays to determine the final pixel color.

## Next Steps

This raytracer can be extended with:
- Additional primitive shapes (planes, triangles, boxes)
- Texture mapping and procedural materials
- Area lights and more complex lighting models
- Acceleration structures for faster rendering
- Parallel processing for multi-core performance