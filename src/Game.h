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

enum GameState : int
{
    Uninitialized,
    Running,
    Exiting
};

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

    // will move to rendering class
    void TempMakeFallingSandsImage();

private:
    GameState currentGameState;
    WindowSettings windowSettings;

    std::unique_ptr<SystemManager> systemManager;
    FallingSandsSystem* fallingSandsSystem;


    // temp
    RenderTexture2D renderTexture;
};
