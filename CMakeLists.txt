cmake_minimum_required(VERSION 3.20)

set(POSITION_INDEPENDENT_CODE true)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION true)

project(Phantom-Renderer DESCRIPTION "A Basic OpenGL renderer" LANGUAGES C HOMEPAGE_URL "https://github.com/ShoweryCellar34/Phantom-Renderer")
file(GLOB source source/*.c)
add_library(Phantom-Renderer STATIC ${source})
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
add_custom_target(delete_resources_msvc ALL COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/$<CONFIG>/res COMMENT "Deleting old resources in binary directory")
add_custom_target(copy_resources_msvc ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/example/res ${PROJECT_BINARY_DIR}/$<CONFIG>/res COMMENT "Copying resources into binary directory" DEPENDS delete_resources_msvc)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS /MP")
add_compile_definitions(/arch:AVX2)
endif()
if(GCC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -pedantic -fsanitize=address,undefined")
endif()

option(PR_BUILD_EXAMPLE "Build the example program" true)

if(${PR_BUILD_EXAMPLE})
add_executable(Example example/example.c)
add_executable(ExampleLite example/exampleLite.c)
target_link_libraries(Example Phantom-Renderer)
target_link_libraries(ExampleLite Phantom-Renderer)
add_custom_target(delete_resources ALL COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/res COMMENT "Deleting old resources in binary directory")
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/example/res ${PROJECT_BINARY_DIR}/res COMMENT "Copying resources into binary directory" DEPENDS delete_resources)

install(TARGETS Example RUNTIME DESTINATION bin)
install(DIRECTORY "${PROJECT_SOURCE_DIR}/example/res" DESTINATION "bin/res")
include(CPack)
endif()
