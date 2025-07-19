#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

int main()
{
    InitWindow(800, 450, "raylib [core] example - basic window");

    rlImGuiSetup(true);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        rlImGuiBegin();

        ClearBackground(RAYWHITE);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}