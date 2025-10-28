# DEV BRANCH, EXPECT ERRORS THIS BRANCH IS FOR BIG CHANGES AND IS NOT NOT BE STABLE

# What is Phantom Renderer?

Phantom Renderer is a basic OpenGL toolbox written in C. It is designed in a way that you are expected to manage the window (some helper functions provided) because this is purely a toolbox; it only needs a GLAD OpenGL context, so using any windowing library that supports GLAD should work (I have confirmed it works with GLFW).

## Platform support

Phantom-Renderer just uses [CMake](https://cmake.org/) and GLAD and is tested on Arch Linux with Hyprland and Windows 11/10, but it might work on Apple devices, maybe possibly slightly if you do some code tweaks

## How to clone

Run this command with [git](https://git-scm.com/) installed to clone this repository: `git clone --recursive --branch dev https://github.com/ShoweryCellar34/Phantom-Renderer.git Phantom-Renderer-dev`
