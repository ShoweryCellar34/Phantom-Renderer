cmake_minimum_required(VERSION 3.20)

set(CGLM_SHARED  false CACHE BOOL "Shared build")
set(CGLM_STATIC  true  CACHE BOOL "Static build")
set(CGLM_USE_C99 false CACHE BOOL "Use C99")
add_subdirectory(vendors/cglm)

set(GLAD_LIBRARY_TYPE "STATIC")
add_subdirectory(vendors/glad)

set(STB_LIBRARY_TYPE "STATIC")
add_subdirectory(vendors/stb)

project(SH34-PR LANGUAGES C)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE PR_C_SOURCE "source/*.c")
add_library(PR STATIC ${PR_C_SOURCE})
target_link_libraries(PR PUBLIC cglm glad stb)
target_include_directories(PR PUBLIC headers)

if(MSVC)
    target_compile_definitions(PR PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
if(GCC)
    target_compile_options(PR PRIVATE -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined)
endif()
