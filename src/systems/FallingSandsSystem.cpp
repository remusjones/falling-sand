//
// Created by Remus on 19/07/2025.
//
#include "FallingSandsSystem.h"
#include <algorithm>
#include <memory>

FallingSandsSystem::FallingSandsSystem(const std::string_view& name, const int32_t& worldWidth, const int32_t& worldHeight)
    : System(name), width(worldWidth), height(worldHeight), grid(worldWidth * worldHeight), backGrid(worldWidth * worldHeight)
{

}

void FallingSandsSystem::Init()
{
    cellBehaviourMap[CellType::Null]    = std::make_unique<CellBehaviour>();
    cellBehaviourMap[CellType::Wall]    = std::make_unique<CellBehaviour>();
    cellBehaviourMap[CellType::Sand]    = std::make_unique<PowderBehaviour>();
    cellBehaviourMap[CellType::Water]   = std::make_unique<LiquidBehaviour>();
    cellBehaviourMap[CellType::Steam]   = std::make_unique<GasBehaviour>();
    cellBehaviourMap[CellType::Fire]    = std::make_unique<FireBehaviour>();
    cellBehaviourMap[CellType::Methane] = std::make_unique<MethaneBehaviour>();
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

        if (!GameUtils::CanDisplace(cellFrom.cellType, cellTo.cellType) || cellTo.cellType == cellFrom.cellType)
            return false;

        for (int32_t index : GameUtils::GetNeighbouringCells(fromIndex, extents, 1))
        {
            newActiveCellsIndices.insert(index);
        }

        const CellType toNewType = cellBehaviourMap[cellFrom.cellType]->Mix(cellFrom.cellType, cellTo.cellType);
        const CellType fromNewType = cellBehaviourMap[cellTo.cellType]->Mix(cellTo.cellType, cellFrom.cellType);

        std::swap(cellTo, cellFrom);

        if (cellTo.cellType != toNewType)
        {
            cellTo.cellType = toNewType;
            cellTo.cellLifetime = cellBehaviourMap[toNewType]->GetLifetime();
        }
        if (cellFrom.cellType != fromNewType)
        {
            cellFrom.cellType = fromNewType;
            cellFrom.cellLifetime = cellBehaviourMap[fromNewType]->GetLifetime();
        }
        return true;
    };

    for (const int32_t& index : activeCellsIndices)
    {
        const Cell& cell = grid[index];

        if (cell.cellType == CellType::Null) continue;


        if (cell.cellLifetime >= 0)
        {
            if (grid[index].cellLifetime == 0)
            {
                backGrid[index].cellType = CellType::Null;

            }else
            {
                backGrid[index].cellLifetime--;
            }
            newActiveCellsIndices.insert(index);
        }
        std::vector<Direction> cellMovement = cellBehaviourMap[cell.cellType]->GetMovementDirections();

        for (Direction currentDirection : cellMovement)
        {
            int32_t neighborIdx;
            if (GameUtils::GetNeighbourIndex(index, currentDirection, extents, neighborIdx))
            {
                if (trySwapCell(neighborIdx, index)){ break; }
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
    grid[cellIndex].cellType = cellType;
    grid[cellIndex].cellLifetime = cellBehaviourMap[cellType]->GetLifetime();
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
