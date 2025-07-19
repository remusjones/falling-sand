//
// Created by Remus on 19/07/2025.
//

#include "FallingSandsSystem.h"

#include <algorithm>

FallingSandsSystem::FallingSandsSystem(const std::string_view& name, const int32_t& worldWidth, const int32_t& worldHeight)
    : System(name), width(worldWidth), height(worldHeight), grid(worldWidth * worldHeight)
{

}

void FallingSandsSystem::Init()
{
}

void FallingSandsSystem::Update(const float &deltaTime)
{
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            int32_t currentCellIndex = y * width + x;
            Cell& cell = grid[currentCellIndex];
            switch (cell.cellType)
            {
                case CellType::Sand:
                {
                    if (y + 1 >= height) break; // ignore cell if its on the floor

                    int32_t targetBelowY = GetClampedHeight(y + 1);

                    auto trySwapNull = [&](const int& nx)  {
                        int32_t targetCell = targetBelowY * width + GetClampedWidth(nx);
                        if (grid[targetCell].cellType == CellType::Null)
                        {
                            std::swap(grid[targetCell], grid[currentCellIndex]);
                            return true;
                        }
                        return false;
                    };

                    // Check below, and then left/right
                    if (trySwapNull(x)) break;
                    if (trySwapNull(x - 1)) break;
                    if (trySwapNull(x + 1)) break;

                    break;
                }
                default: ;
            }
        }
    }
}

void FallingSandsSystem::Shutdown()
{
}

int32_t FallingSandsSystem::GetClampedWidth(const int32_t& targetWidth) const
{
    return std::clamp<int32_t>(targetWidth, 0, width - 1);
}

int32_t FallingSandsSystem::GetClampedHeight(const int32_t& targetHeight) const
{
    return std::clamp<int32_t>(targetHeight, 0, height - 1);
}
