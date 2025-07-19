//
// Created by Remus on 19/07/2025.
//
#pragma once
#include <string>

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
    GameState currentState;
    WindowSettings settings;
};
