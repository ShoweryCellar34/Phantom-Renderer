cmake_minimum_required(VERSION 3.20)

set(POSITION_INDEPENDENT_CODE true)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION true)

project(Phantom-Renderer DESCRIPTION "A Basic OpenGL renderer" LANGUAGES C HOMEPAGE_URL "https://github.com/ShoweryCellar34/Phantom-Renderer")
file(GLOB PR_SOURCE source/*.c)
add_library(Phantom-Renderer STATIC ${PR_SOURCE})
set_property(TARGET Phantom-Renderer PROPERTY C_STANDARD 99)
target_include_directories(Phantom-Renderer PUBLIC headers)

set(CGLM_SHARED OFF)
set(CGLM_STATIC ON)
add_subdirectory(vendors/cglm)

set(GLAD_LIBRARY_TYPE "STATIC")
add_subdirectory(vendors/glad)

set(GLFW_USE_HYBRID_HPG true CACHE BOOL "Force use of high-performance GPU on hybrid systems")
set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
add_subdirectory(vendors/glfw)

set(STB_LIBRARY_TYPE "STATIC")
add_subdirectory(vendors/stb)

target_link_libraries(Phantom-Renderer cglm glad glfw stb)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /MP")
    add_compile_definitions(/arch:AVX2)
endif()
if(GCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic -fsanitize=address,undefined")
endif()

option(PR_BUILD_EXAMPLE "Build the example program" true)

if(${PR_BUILD_EXAMPLE})
    add_subdirectory(example)
endif()
