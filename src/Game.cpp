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

    int32_t worldSizeX = 800/3;
    int32_t worldSizeY = 448/3;

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

    fallingSandsSystem = systemManager->RegisterSystem<FallingSandsSystem>("falling sands", worldSizeX, worldSizeY);

    systemManager->Init();
}

Color GetCellTypeColor(const CellType& cellType)
{
    switch (cellType)
    {
        case CellType::Sand:
        {
            return CLITERAL(Color){ 225,191,146, 255 };
        }
        case CellType::Water:
        {
            return CLITERAL(Color){ 111, 122, 252, 255 };
        }
        default: return BLACK;
    }
}

void Game::Update()
{
    while (!WindowShouldClose())
    {
        Vector2 mousePos = GetMousePosition();

        // todo: Move to imgui pallet
        int mouseButtonInput = -1;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) mouseButtonInput = 0;
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) mouseButtonInput = 1;

        if (mouseButtonInput != -1)
        {
            CellType selectedCellType = mouseButtonInput > 0 ? CellType::Water : CellType::Sand;
            float cellWidth = static_cast<float>(windowSettings.windowWidth) /  static_cast<float>(fallingSandsSystem->GetWidth());
            float cellHeight = static_cast<float>(windowSettings.windowHeight) / static_cast<float>(fallingSandsSystem->GetHeight());

            int cellX = fallingSandsSystem->GetClampedWidth(static_cast<int>(mousePos.x / cellWidth));
            int cellY = fallingSandsSystem->GetClampedHeight(static_cast<int>(mousePos.y / cellHeight));

            auto& grid = fallingSandsSystem->GetGrid();
            grid[cellY * fallingSandsSystem->GetWidth() + cellX].cellType = selectedCellType;
        }

        systemManager->Update(GetFrameTime());

        BeginDrawing();
        rlImGuiBegin();

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
        cellPixels[i] = GetCellTypeColor(grid[i].cellType);
    }

    UpdateTexture(gridTexture, cellPixels.data());

    float resolution[2] = { static_cast<float>(width), static_cast<float>(height) };
    SetShaderValue(shader, GetShaderLocation(shader, "resolution"), resolution, SHADER_UNIFORM_VEC2);
    SetShaderValueTexture(shader, GetShaderLocation(shader, "cellTex"), gridTexture);

    Rectangle src = { 0, 0, static_cast<float>(width),              static_cast<float>(height) };
    Rectangle dst = { 0, 0, static_cast<float>(GetScreenWidth()),   static_cast<float>(GetScreenHeight()) };
    Vector2 origin = { 0, 0 };

    DrawTexturePro(gridTexture, src, dst, origin, 0.0f, WHITE);

    EndShaderMode();
}
