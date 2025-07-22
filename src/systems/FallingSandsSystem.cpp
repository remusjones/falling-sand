//
// Created by Remus on 19/07/2025.
//
#include "FallingSandsSystem.h"
#include <algorithm>

#include "raylib.h"

FallingSandsSystem::FallingSandsSystem(const std::string_view& name, const int32_t& worldWidth, const int32_t& worldHeight)
    : System(name), width(worldWidth), height(worldHeight), grid(worldWidth * worldHeight), backGrid(worldWidth * worldHeight)
{

}

void FallingSandsSystem::Init()
{
    static SandMovement sandMovement;
    static WaterMovement waterMovement;

    cellMovementMap[CellType::Sand] = &sandMovement;
    cellMovementMap[CellType::Water] = &waterMovement;
}

void FallingSandsSystem::Update(const float& deltaTime)
{
    const Vector2i extents = {width, height};
    std::unordered_set<int32_t> newActiveCellsIndices(width * height);

    backGrid = grid;

    auto trySwapCell = [&](const int32_t& targetIndex, const int32_t& fromIndex)
    {
        Cell& cellFrom = backGrid[fromIndex];
        Cell& cellTo   = backGrid[targetIndex];

        if (!GameUtils::CanDisplace(cellFrom.cellType, cellTo.cellType))
            return false;

        for (int32_t index : GameUtils::GetNeighbouringCells(fromIndex, extents, 1))
        {
            newActiveCellsIndices.insert(index);
        }

        std::swap(cellTo, cellFrom);

        return true;
    };


    for (const int32_t& index : activeCellsIndices)
    {
        const Cell& cell = grid[index];

        if (cell.cellType == CellType::Null) continue;

        std::vector<Direction> cellMovement = cellMovementMap[cell.cellType]->GetMovementDirections();

        for (Direction currentDirection : cellMovement)
        {
            int32_t neighborIdx;
            if (GameUtils::GetNeighbourIndex(index, currentDirection, extents, neighborIdx))
            {
                if (trySwapCell(neighborIdx, index)){}
            }
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
    if (backGrid[cellIndex].cellType != CellType::Null) return;

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
