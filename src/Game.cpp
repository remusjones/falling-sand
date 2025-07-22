//
// Created by Remus on 19/07/2025.
//

#include "Game.h"
#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"
#include "systems/FallingSandsSystem.h"

Game::Game() : cellPixels(), gridTexture(), shader()
{
    windowSettings.windowWidth = 800;
    windowSettings.windowHeight = 450;
    windowSettings.windowTitle = "falling-sand";

    Setup();
    Update();
    Shutdown();
}

void Game::Setup()
{
    InitWindow(windowSettings.windowWidth, windowSettings.windowHeight, windowSettings.windowTitle.c_str());

    rlImGuiSetup(true);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    systemManager = std::make_unique<SystemManager>();

    int32_t worldSizeX = windowSettings.windowWidth / 3;
    int32_t worldSizeY = (windowSettings.windowHeight - reservedUIHeight) / 3;

    cellPixels = std::vector<Color>(worldSizeX * worldSizeY);

    Image image = {
        .data = cellPixels.data(),
        .width = worldSizeX,
        .height = worldSizeY,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };

    gridTexture = LoadTextureFromImage(image);
    shader = LoadShader(0, "resources/shaders/fallingSand_fs.glsl");
    SetTargetFPS(120);
    fallingSandsSystem = systemManager->RegisterSystem<FallingSandsSystem>("falling sands", worldSizeX, worldSizeY);

    systemManager->Init();
}

void Game::DrawUI()
{
    ImGui::SetNextWindowPos(ImVec2(0, static_cast<float>(windowSettings.windowHeight) - 80), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(windowSettings.windowWidth), 80), ImGuiCond_Always);

    ImGui::Begin("Palette", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

    ImGui::Text("Select Cell Type:");

    auto DrawSelectableButton = [&](const char* label, CellType type)
    {
        bool isSelected = (selectedCellType == type);

        if (isSelected)
        {
            ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.4f, 0.7f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.8f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.3f, 0.6f, 0.9f, 1.0f));
        }

        if (ImGui::Button(label))
        {
            selectedCellType = type;
        }

        if (isSelected)
        {
            ImGui::PopStyleColor(3);
        }

        ImGui::SameLine();
    };

    DrawSelectableButton("Wall",    CellType::Wall);
    DrawSelectableButton("Sand",    CellType::Sand);
    DrawSelectableButton("Water",   CellType::Water);
    DrawSelectableButton("Fire",    CellType::Fire);
    DrawSelectableButton("Steam",   CellType::Steam);
    DrawSelectableButton("Methane", CellType::Methane);

    ImGui::NewLine();
    ImGui::End();
}
void Game::Update()
{
    while (!WindowShouldClose())
    {
        const Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            const int gridWidth = fallingSandsSystem->GetWidth();
            const int gridHeight = fallingSandsSystem->GetHeight();
            float cellWidth = static_cast<float>(windowSettings.windowWidth) / static_cast<float>(gridWidth);
            float cellHeight = static_cast<float>(windowSettings.windowHeight - reservedUIHeight) / static_cast<float>(gridHeight);

            int cellX = static_cast<int>(mousePos.x / cellWidth);
            int cellY = static_cast<int>(mousePos.y / cellHeight);

            if (cellX >= 0 && cellX < gridWidth && cellY >= 0 && cellY < gridHeight)
            {
                int index = cellY * gridWidth + cellX;
                for (auto idx : GameUtils::GetNeighbouringCells(index, {gridWidth, gridHeight}, 1))
                {
                    fallingSandsSystem->ModifyCell(idx, selectedCellType);
                }
                fallingSandsSystem->ModifyCell(index, selectedCellType);
            }
        }

        systemManager->Update(GetFrameTime());

        BeginDrawing();
        rlImGuiBegin();

        DrawUI();
        ClearBackground(RAYWHITE);

        DrawSim();
        DrawFPS(0,0);

        rlImGuiEnd();
        EndDrawing();
    }
    UnloadTexture(gridTexture);
}

void Game::Shutdown()
{
    systemManager->Shutdown();
    rlImGuiShutdown();
    CloseWindow();
}

void Game::DrawSim()
{
    BeginShaderMode(shader);

    const int width = fallingSandsSystem->GetWidth();
    const int height = fallingSandsSystem->GetHeight();
    const std::vector<Cell>& grid = fallingSandsSystem->GetGrid();

    if (cellPixels.size() != width * height)
        cellPixels.resize(width * height);

    for (int i = 0; i < width * height; ++i)
    {
        cellPixels[i] = GameUtils::GetCellTypeColor(grid[i].cellType);
    }

    UpdateTexture(gridTexture, cellPixels.data());

    float resolution[2] = { static_cast<float>(width), static_cast<float>(height) };
    SetShaderValue(shader, GetShaderLocation(shader, "resolution"), resolution, SHADER_UNIFORM_VEC2);
    SetShaderValueTexture(shader, GetShaderLocation(shader, "cellTex"), gridTexture);

    Rectangle src = { 0, 0, static_cast<float>(width), static_cast<float>(height) };
    Rectangle dst = {
        0,
        0,
        static_cast<float>(GetScreenWidth()),
        static_cast<float>(GetScreenHeight() - reservedUIHeight)
    };
    DrawTexturePro(gridTexture, src, dst, { 0, 0 }, 0.0f, WHITE);

    EndShaderMode();
}
