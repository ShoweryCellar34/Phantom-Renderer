cmake_minimum_required(VERSION 3.10)

project(STB LANGUAGES C)

add_library(stb ${STB_LIBRARY_TYPE} stb.c)
target_include_directories(stb PUBLIC stb)

if(MSVC)
    target_compile_definitions(stb PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
if(GCC)
    target_compile_options(stb PRIVATE -Wall -Wextra -Wpedantic -fsanitize=address,undefined)
endif()
