//
// Created by Remus on 19/07/2025.
//

#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>

#include "System.h"

enum class CellType
{
    Null,
    Sand
};

struct Cell
{
    CellType cellType;
};

class FallingSandsSystem : public System
{
public:
    FallingSandsSystem(const std::string_view& name, const int32_t& worldWidth, const int32_t& worldHeight);

    void Init() override;
    void Update(const float& deltaTime) override;
    void Shutdown() override;

    std::vector<Cell>& GetGrid() { return grid; }
    [[nodiscard]] int32_t GetWidth() const { return width; }
    [[nodiscard]] int32_t GetHeight() const { return height; }

    [[nodiscard]] int32_t GetClampedWidth(const int32_t& targetWidth) const;
    [[nodiscard]] int32_t GetClampedHeight(const int32_t& targetHeight) const;


private:
    int32_t width;
    int32_t height;
    std::vector<Cell> grid;
};
