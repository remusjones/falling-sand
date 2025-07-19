//
// Created by Remus on 19/07/2025.
//

#include "Game.h"
#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"
#include "systems/FallingSandsSystem.h"

Game::Game(): currentGameState(Uninitialized), windowSettings()
{
    Setup();
    Update();
    Shutdown();
}

void Game::Setup()
{
    windowSettings.windowWidth = 800;
    windowSettings.windowHeight = 450;
    windowSettings.windowTitle = "falling-sand";

    InitWindow(windowSettings.windowWidth, windowSettings.windowHeight, windowSettings.windowTitle.c_str());

    rlImGuiSetup(true);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    currentGameState = Running;

    systemManager = std::make_unique<SystemManager>();

    int32_t worldSizeX = 200;
    int32_t worldSizeY = 112;

    fallingSandsSystem = systemManager->RegisterSystem<FallingSandsSystem>("falling sands", worldSizeX, worldSizeY);

    // Move to renderer
    renderTexture = LoadRenderTexture(worldSizeX, worldSizeY);
    BeginTextureMode(renderTexture);
    ClearBackground(RAYWHITE);
    EndTextureMode();

    systemManager->Init();
}

void Game::Update()
{
    while (!WindowShouldClose() && currentGameState != Exiting)
    {
        Vector2 mousePos = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) || (GetGestureDetected() == GESTURE_DRAG))
        {
            float cellWidth = static_cast<float>(windowSettings.windowWidth) /  static_cast<float>(fallingSandsSystem->GetWidth());
            float cellHeight = static_cast<float>(windowSettings.windowHeight) / static_cast<float>(fallingSandsSystem->GetHeight());

            int cellX = fallingSandsSystem->GetClampedWidth(static_cast<int>(mousePos.x / cellWidth));
            int cellY = fallingSandsSystem->GetClampedHeight(static_cast<int>(mousePos.y / cellHeight));

            auto& grid = fallingSandsSystem->GetGrid();
            grid[cellY * fallingSandsSystem->GetWidth() + cellX].cellType = CellType::Sand;
        }

        systemManager->Update(GetFrameTime());

        // todo: Move rendering funcs to renderer class
        BeginDrawing();
        rlImGuiBegin();

        ClearBackground(RAYWHITE);

        TempMakeFallingSandsImage();

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        Rectangle source = { 0, 0, static_cast<float>(renderTexture.texture.width), static_cast<float>(-renderTexture.texture.height) };
        Rectangle dest = { 0, 0, static_cast<float>(screenWidth), static_cast<float>(screenHeight) };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(renderTexture.texture, source, dest, origin, 0.0f, WHITE);

        rlImGuiEnd();
        EndDrawing();
    }
    UnloadRenderTexture(renderTexture);
}

void Game::Shutdown()
{
    systemManager->Shutdown();
    rlImGuiShutdown();
    CloseWindow();
}

Color GetCellTypeColor(const CellType& cellType)
{
    switch (cellType)
    {
        case CellType::Sand:
        {
            return CLITERAL(Color){ 225,191,146, 255 };
        }
        default: return BLACK;
    }
}

void Game::TempMakeFallingSandsImage()
{
    // todo: Temp function, move to renderer class
    BeginTextureMode(renderTexture);
    std::vector<Cell>& grid = fallingSandsSystem->GetGrid();
    const int32_t width = fallingSandsSystem->GetWidth();
    const int32_t height = fallingSandsSystem->GetHeight();

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Cell& cell = grid[y * width + x];
            Color cellColor = GetCellTypeColor(cell.cellType);

            // spatial hash
            int noise = ((x * 73856093) ^ (y * 19349663)) & 0xFF;
            int offset = noise % 11 - 10; // range -+ 10

            Color noisyColor = {
                static_cast<unsigned char>(std::clamp(cellColor.r + offset, 0, 255)),
                static_cast<unsigned char>(std::clamp(cellColor.g + offset, 0, 255)),
                static_cast<unsigned char>(std::clamp(cellColor.b + offset, 0, 255)),
                cellColor.a
            };

            DrawPixel(x, y, noisyColor);
        }
    }
    EndTextureMode();
}
