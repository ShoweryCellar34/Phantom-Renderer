cmake_minimum_required(VERSION 4.0.0)

project(GLAD LANGUAGES C)

file(GLOB glad_sources glad/src/*.c)
add_library(glad ${GLAD_LIBRARY_TYPE} ${glad_sources})
target_include_directories(glad PUBLIC glad/include)

if(MSVC)
    target_compile_definitions(glad PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
if(GCC)
    target_compile_options(glad PRIVATE -Wall -Wextra -Wpedantic -fsanitize=address,undefined)
endif()
