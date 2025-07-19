add_subdirectory(raylib)

add_library(vendored INTERFACE)
target_include_directories(raylib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/raylib/src)
target_link_libraries(vendored INTERFACE raylib)
