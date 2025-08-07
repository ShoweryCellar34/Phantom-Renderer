cmake_minimum_required(VERSION 3.20)

project(Example LANGUAGES C)

file(GLOB SOURCE source/*.c)
add_executable(Example source/example.c ${SOURCE})
set_property(TARGET Example PROPERTY C_STANDARD 99)
target_include_directories(Example PUBLIC headers)

target_link_libraries(Example Phantom-Renderer)

add_subdirectory(vendors/tinyobjloader-c)
target_link_libraries(Example tinyobjloader-c)

if(MSVC)
    set_target_properties(Example PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/$<CONFIG>/")
    add_custom_target(delete_resources_msvc ALL COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/$<CONFIG>/res COMMENT "Deleting old resources in binary directory")
    add_custom_target(copy_resources_msvc ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${PROJECT_BINARY_DIR}/$<CONFIG>/res COMMENT "Copying resources into binary directory" DEPENDS delete_resources_msvc)
else()
    add_custom_target(delete_resources ALL COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/res COMMENT "Deleting old resources in binary directory")
    add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${PROJECT_BINARY_DIR}/res COMMENT "Copying resources into binary directory" DEPENDS delete_resources)
endif()

option(PR_BUILD_EXAMPLE_INSTALLER "Build the example program's installer" true)

if(PR_BUILD_EXAMPLE_INSTALLER)
    set(CPACK_PACKAGE_NAME "Phantom Renderer Example")
    set(CPACK_PACKAGE_VERSION "0.0.0")
    set(CPACK_PACKAGE_VENDOR "ShoweryCellar34")

    set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/packages")

    include(InstallRequiredSystemLibraries)
    install(TARGETS Example RUNTIME DESTINATION bin)
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION .)
    include(CPack)
endif()
