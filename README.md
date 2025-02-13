
# What is Phantom Renderer?

Phantom Renderer basic file OpenGL based renderer using GLFW. This is mostly just for use in my other projects. It is designed in a way that everywhere a class/stuct has no private methods/properties for easy access to internal data \(all properties/methods that would be private are prefixed with `i_`\)

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
5. Run the produced `Phantom-Renderer` file.
