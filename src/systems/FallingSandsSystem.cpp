//
// Created by Remus on 19/07/2025.
//
#include "FallingSandsSystem.h"
#include <algorithm>

#include "GameUtils.h"

FallingSandsSystem::FallingSandsSystem(const std::string_view& name, const int32_t& worldWidth, const int32_t& worldHeight)
    : System(name), width(worldWidth), height(worldHeight), grid(worldWidth * worldHeight), backGrid(worldWidth * worldHeight)
{

}

void FallingSandsSystem::Init()
{
}

void FallingSandsSystem::Update(const float& deltaTime)
{
    const Vector2i extents = {width, height};
    backGrid = grid;


    auto trySwapCell = [&](const int32_t& targetIndex, const int& fromIndex)  {

        Cell& cellFrom = backGrid[fromIndex];
        Cell& cellTo = backGrid[targetIndex];

        if (cellTo.isDirty || cellFrom.isDirty) return false;

        if (GameUtils::CanDisplace(cellFrom.cellType, cellTo.cellType))
        {
            std::swap(cellFrom, cellTo);
            cellTo.isDirty = true;
            cellFrom.isDirty = true;

            return true;
        }
        return false;
    };

    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            int32_t currentCellIndex = y * width + x;
            Cell& cell = grid[currentCellIndex];
            backGrid[currentCellIndex].isDirty = false;

            switch (cell.cellType)
            {
                case CellType::Sand:
                {
                    constexpr Direction validMovementDirection[] = {
                        Direction::DOWN,
                        Direction::DOWN_LEFT,
                        Direction::DOWN_RIGHT
                    };

                    for (Direction currentDirection : validMovementDirection)
                    {
                        int32_t neighborIdx;
                        if (GameUtils::GetNeighbourIndex(currentCellIndex, currentDirection, extents, neighborIdx))
                        {
                            if (trySwapCell(neighborIdx, currentCellIndex))
                            {
                                break;
                            }
                        }
                    }

                    break;
                }

                case CellType::Water:
                {
                    constexpr Direction validMovementDirection[] = {
                        Direction::DOWN,
                        Direction::DOWN_LEFT,
                        Direction::DOWN_RIGHT,
                        Direction::RIGHT,
                        Direction::LEFT,
                    };

                    for (Direction currentDirection : validMovementDirection)
                    {
                        int32_t neighborIdx;
                        if (GameUtils::GetNeighbourIndex(currentCellIndex, currentDirection, extents, neighborIdx))
                        {
                            if (trySwapCell(neighborIdx, currentCellIndex))
                            {
                                break;
                            }
                        }
                    }

                    break;
                }
                default: ;
            }
        }
    }

    std::swap(backGrid, grid);
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
