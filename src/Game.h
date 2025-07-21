//
// Created by Remus on 19/07/2025.
//
#pragma once
#include <memory>
#include <string>

#include "raylib.h"
#include "ECS/HelloECS.h"
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
    void Update();
    void Shutdown();

    void DrawSim();

private:
    WindowSettings windowSettings;
    std::unique_ptr<SystemManager> systemManager;
    FallingSandsSystem* fallingSandsSystem;

    // Rendering
    std::vector<Color> cellPixels;
    Texture2D gridTexture;
    Shader shader;
};
