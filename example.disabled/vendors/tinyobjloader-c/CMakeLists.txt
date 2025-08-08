cmake_minimum_required(VERSION 3.10)
add_library(tinyobjloader-c ${TINYOBJLOADER_C_LIBRARY_TYPE} tinyobjloader-c.c)
target_include_directories(tinyobjloader-c PUBLIC tinyobjloader-c)
