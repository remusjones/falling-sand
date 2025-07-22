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
    std::unordered_set<int32_t> newActiveCellsIndices(width * height);

    backGrid = grid;

    auto trySwapCell = [&](const int32_t& targetIndex, const int32_t& fromIndex) {

        Cell& cellFrom = backGrid[fromIndex];
        Cell& cellTo = backGrid[targetIndex];

        if (GameUtils::CanDisplace(cellFrom.cellType, cellTo.cellType))
        {
            for (int32_t& index : GameUtils::GetNeighbouringCells(fromIndex, extents, 1))
            {
                newActiveCellsIndices.insert(index);
            }

            std::swap(cellFrom, cellTo);
            return true;
        }

        return false;
    };

    for (const int32_t& index : activeCellsIndices)
    {
        const Cell& cell = grid[index];
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
                    if (GameUtils::GetNeighbourIndex(index, currentDirection, extents, neighborIdx))
                    {
                        if (trySwapCell(neighborIdx, index))
                            break;
                    }
                }

                break;
            }

            case CellType::Water:
            {
                constexpr Direction baseDirections[] = {
                    Direction::DOWN,
                    Direction::DOWN_LEFT,
                    Direction::DOWN_RIGHT
                };

                Direction validMovementDirection[5];

                for (int i = 0; i < 3; ++i)
                {
                    validMovementDirection[i] = baseDirections[i];
                }

                if (rand() % 2 == 0)
                {
                    validMovementDirection[3] = Direction::LEFT;
                    validMovementDirection[4] = Direction::RIGHT;
                }
                else
                {
                    validMovementDirection[3] = Direction::RIGHT;
                    validMovementDirection[4] = Direction::LEFT;
                }

                for (const Direction& movementDirection: validMovementDirection)
                {
                    int32_t neighborIdx;
                    if (GameUtils::GetNeighbourIndex(index, movementDirection, extents, neighborIdx))
                    {
                        if (trySwapCell(neighborIdx, index))
                            break;
                    }
                }
                break;
            }

            default: ;
        }
    }

    std::swap(backGrid, grid);
    activeCellsIndices = std::move(newActiveCellsIndices);
}

void FallingSandsSystem::Shutdown()
{
}

void FallingSandsSystem::ModifyCell(const int32_t& cellIndex, const CellType& cellType)
{
    grid[cellIndex].cellType = cellType;
    activeCellsIndices.insert(cellIndex);
}

int32_t FallingSandsSystem::GetClampedWidth(const int32_t& targetWidth) const
{
    return std::clamp<int32_t>(targetWidth, 0, width - 1);
}

int32_t FallingSandsSystem::GetClampedHeight(const int32_t& targetHeight) const
{
    return std::clamp<int32_t>(targetHeight, 0, height - 1);
}
