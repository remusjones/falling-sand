//
// Created by Remus on 19/07/2025.
//

#pragma once
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CellBehaviours.h"
#include "GameUtils.h"
#include "System.h"


class FallingSandsSystem : public System
{
public:
    FallingSandsSystem(const std::string_view& name, const int32_t& worldWidth, const int32_t& worldHeight);

    void Init() override;
    void Update(const float& deltaTime) override;
    void Shutdown() override;

    void ModifyCell(const int32_t& cellIndex, const CellType& cellType);

    std::vector<Cell>& GetGrid() { return grid; }
    [[nodiscard]] int32_t GetWidth() const { return width; }
    [[nodiscard]] int32_t GetHeight() const { return height; }

    [[nodiscard]] int32_t GetClampedWidth(const int32_t& targetWidth) const;
    [[nodiscard]] int32_t GetClampedHeight(const int32_t& targetHeight) const;

private:
    int32_t width;
    int32_t height;

    std::vector<Cell> grid;
    std::vector<Cell> backGrid;

    std::unordered_set<int32_t> activeCellsIndices;
    std::unordered_map<CellType, std::unique_ptr<CellBehaviour>> cellBehaviourMap;
};
