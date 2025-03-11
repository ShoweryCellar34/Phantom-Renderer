set(POSITION_INDEPENDENT_CODE true)

cmake_minimum_required(VERSION 3.20)
add_executable(Test example.c)
target_link_libraries(Test Phantom-Renderer)
add_custom_target(delete_resources ALL COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_BINARY_DIR}/example/res COMMENT "Deleting old resources in binary directory")
add_custom_target(copy_resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/example/res ${PROJECT_BINARY_DIR}/example/res COMMENT "Copying resources into binary directory" DEPENDS delete_resources)
