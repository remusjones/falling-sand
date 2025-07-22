//
// Created by Remus on 19/07/2025.
//
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "GameUtils.h"
#include "raylib.h"
#include "systems/SystemManager.h"

class FallingSandsSystem;

struct WindowSettings
{
    int windowWidth;
    int windowHeight;
    std::string windowTitle;
};

class Game 
{
public:
    Game();

private:
    void Setup();

    void DrawUI();

    void Update();
    void Shutdown();

    void DrawSim();

private:
    WindowSettings windowSettings;
    std::unique_ptr<SystemManager> systemManager;
    FallingSandsSystem* fallingSandsSystem;
    int32_t reservedUIHeight = 80;

    CellType selectedCellType = CellType::Sand;
    // Rendering
    std::vector<Color> cellPixels;
    Texture2D gridTexture;
    Shader shader;
};
