add_library(vendored INTERFACE)

# Raylib
add_subdirectory(raylib)
target_include_directories(raylib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/raylib/src)

# ImGui
add_library(imgui STATIC)

target_include_directories(imgui
        PUBLIC
        imgui
        imgui/backends
)

set(IMGUI_SOURCES
        "imgui/imgui.h"
        "imgui/imgui.cpp"
        "imgui/imgui_demo.cpp"
        "imgui/imgui_draw.cpp"
        "imgui/imgui_widgets.cpp"
        "imgui/imgui_tables.cpp"
)

target_sources(imgui PUBLIC ${IMGUI_SOURCES})

# rlImGui
add_library(rlimgui STATIC)

target_include_directories(rlimgui
        PUBLIC
        rlImGui
)

set(RLIMGUI_SOURCES
        rlImGui/rlImGui.h
        rlImGui/rlImGui.cpp
        rlImGui/imgui_impl_raylib.h
)

target_sources(rlimgui PUBLIC ${RLIMGUI_SOURCES})
target_link_libraries(rlimgui PUBLIC raylib imgui)
target_link_libraries(vendored INTERFACE raylib imgui rlimgui)