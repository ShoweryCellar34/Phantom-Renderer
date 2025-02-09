set(CMAKE_CXX_STANDARD 20)
set(POSITION_INDEPENDENT_CODE true)

cmake_minimum_required(VERSION 3.20)
project(Phantom-Renderer DESCRIPTION "A Basic File Resource Manager" LANGUAGES CXX C HOMEPAGE_URL "https://github.com/ShoweryCellar34/Phantom-Renderer")
file(GLOB source source/*.cpp)
add_library(Phantom-Renderer ${source})
target_include_directories(Phantom-Renderer PUBLIC headers)

set(GLAD_LIBRARY_TYPE "STATIC")
add_subdirectory(vendors/glad)

set(GLFW_USE_HYBRID_HPG true CACHE BOOL "Force use of high-performance GPU on hybrid systems")
set(GLFW_LIBRARY_TYPE "STATIC" CACHE STRING "Library type override for GLFW (SHARED, STATIC, OBJECT, or empty to follow BUILD_SHARED_LIBS)")
add_subdirectory(vendors/glfw)

target_link_libraries(Phantom-Renderer glad glfw)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /MP")
endif()
if(GCC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic -fsanitize=address,undefined")
endif()

add_executable(Test test.cpp)
target_link_libraries(Test Phantom-Renderer)
