#include <memory>

#include "Game.h"


int main()
{
    std::unique_ptr<Game> game = std::make_unique<Game>();
    return 0;
}