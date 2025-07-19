//
// Created by Remus on 19/07/2025.
//

#include "Game.h"

#include <memory>
#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

Game::Game(): currentState(Uninitialized), settings()
{
    settings.windowWidth = 800;
    settings.windowHeight = 450;
    settings.windowTitle = "falling-sand";

    InitWindow(settings.windowWidth, settings.windowHeight, settings.windowTitle.c_str());

    rlImGuiSetup(true);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    currentState = Running;
    while (!WindowShouldClose() && currentState != Exiting)
    {
        BeginDrawing();
        rlImGuiBegin();

        ClearBackground(RAYWHITE);

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
}