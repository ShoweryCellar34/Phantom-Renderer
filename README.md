# DEV BRANCH, EXPECT ERRORS THIS BRANCH IS FOR BIG CHANGES AND IS NOT NOT BE STABLE

# What is Phantom Renderer?

Phantom Renderer basic OpenGL based renderer using GLFW written in C, It is designed in a way that you are expected to manage the window (some heelper functions provided) because this being purely a renderer it only needs a glad opengl context so using SDL as a GLFW replacement would be very simple (just make sure to update the opengl viewport size). This is mostly just for use in my other projects.

## Platform support

Resource Manager just uses GLFW and [CMake](https://cmake.org/), and is tested on Arch linux with Hyprland and Windows 11/10.

## How to clone

Run this command with [git](https://git-scm.com/) installed to clone this repository: `git clone --recursive https://github.com/ShoweryCellar34/Phantom-Renderer.git`

## How to build \(CMake required\)

### [CMake](https://cmake.org/)
1. Move into the cloned directory `cd Phantom-Renderer`.
2. Make the build directory `mkdir build` and move into it `cd build`.
3. Generate the project file `cmake ..`.
4. Build with `cmake --build .`
5. Run the produced `Phantom-Renderer` file \(if example is build\).
