set(POSITION_INDEPENDENT_CODE true)

cmake_minimum_required(VERSION 3.20)
project(Phantom-Renderer DESCRIPTION "A Basic File Resource Manager" LANGUAGES C HOMEPAGE_URL "https://github.com/ShoweryCellar34/Phantom-Renderer")
file(GLOB source source/*.c)
add_library(Phantom-Renderer ${source})
target_include_directories(Phantom-Renderer PUBLIC headers)

set(CGLM_SHARED ON)
set(CGLM_STATIC OFF)
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
endif()
if(GCC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic -fsanitize=address,undefined -static")
endif()

add_executable(Test example/example.c)
target_link_libraries(Test Phantom-Renderer)
add_custom_target(delete_resources ALL COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/res COMMENT "Deleting old resources in binary directory")
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${PROJECT_BINARY_DIR}/res COMMENT "Copying resources into binary directory" DEPENDS delete_resources)
