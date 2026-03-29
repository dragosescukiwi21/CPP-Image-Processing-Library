# C++ Image Processing & Convolution Toolkit (

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Build](https://img.shields.io/badge/build-CMake-green.svg)


A C++ library for Image Processing and Image Convolution built from scratch, meant to serve as a foundational layer for future CNN implementation.

## Features
- **Zero Dependencies**: Built from scratch using standard C++ (no OpenCV required).
- **Core Transformations**: Brightness, Contrast, and Gamma Correction logic.
- **Image Convolution**: Supports generic matrix convolutions with a dedicated `ImageConvolution` class. Includes pre-defined kernels for:
  - Identity
  - Mean Blur (3x3)
  - Gaussian Blur (3x3)
  - Sobel Edge Detection (Horizontal & Vertical)
- **Shape Drawing**: Basic line, rectangle, and circle drawing algorithms.
- **PGM Format Support**: Native parsing and generation for PGM images.

## Project Structure
The repository is professionally structured using standard C++ conventions:
- `include/`: Contains all public header `*.h` files.
- `src/`: Contains all source `*.cpp` files.
- `CMakeLists.txt`: Configures the project build across all platforms.

## Building the Project

This project uses **CMake** to generate cross-platform build files.

### Requirements:
- A modern C++17 compatible compiler (e.g., GCC, Clang, MSVC).
- CMake (version 3.10 or higher).

### Build Steps:

1. Clone the repository and navigate into the project directory:
   ```bash
   git clone https://github.com/yourusername/cnn-image-processing.git
   cd cnn-image-processing
   ```
2. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
3. Generate build files and compile:
   ```bash
   cmake ..
   cmake --build .
   ```
4. Run the executable:
   - On Linux/macOS: `./CNN_Image_Processing`
   - On Windows: `Debug\CNN_Image_Processing.exe` (or `Release`)

## Usage

Upon running the executable, you will be presented with an interactive console menu:

```text
1. Load 
2. Save 
3. Apply Brightness and Contrast
4. Apply Gamma Correction
5. Apply Image Convolution (Blur, Edge Detection)
6. Draw Circle
7. Draw Line
8. Draw Rectangle
9. Exit
```

This toolkit allows you to chain multiple operations natively on an active in-memory buffer before ultimately saving the resulting `.pgm` image.

## Extending to a Full CNN
Future extensions to introduce full Deep Learning forward-passes (Conv2D layers, ReLU activations, View/Flatten, and Dense layers) can extend the `ImageProcessing` abstract interface.
