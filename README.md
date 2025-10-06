# DEV BRANCH, EXPECT ERRORS THIS BRANCH IS FOR BIG CHANGES AND IS NOT NOT BE STABLE

# What is Phantom Renderer?

Phantom Renderer basic OpenGL based renderer using GLFW written in C, It is designed in a way that you are expected to manage the window (some heelper functions provided) because this being purely a renderer it only needs a glad OpenGL context so using SDL as a GLFW replacement would be simple (maybe, I have never tried it).

# Example application
There is an example application in the `example/` folder that is build by default but can be disabled in cmake (set `PR_BUILD_EXAMPLE` cmake variable to false). The example is primarily mean to show what can be done with Phantom Renderer, but I am too stupid to get the the more complicated stuff working, as a result of my mistakes don't use my code as an opengl tutorial, use something like [LearnOpenGL](https://learnopengl.com) 

## Platform support

Phantom-Renderer just uses GLFW and [CMake](https://cmake.org/), and is tested on Arch linux with Hyprland and Windows 11/10.

## How to clone

Run this command with [git](https://git-scm.com/) installed to clone this repository: `git clone --recursive --branch dev https://github.com/ShoweryCellar34/Phantom-Renderer.git Phantom-Renderer-dev`

## How to build example \(CMake required\)

### [CMake](https://cmake.org/)
1. Move into the cloned directory `cd Phantom-Renderer-dev`.
2. Make the build directory `mkdir build` and move into it `cd build`.
3. Generate the project file `cmake ..`.
4. Build with `cmake --build .`
5. Run the produced `Example` file \(may be in a `Debug` or `Release` directory if using MSVC\).
