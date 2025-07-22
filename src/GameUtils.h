//
// Created by Remus on 20/07/2025.
//


#pragma once
#include <array>
#include <cstdint>
#include <random>
#include <stdlib.h>

enum Direction : uint8_t
{
    UP          = 1 << 0,
    DOWN        = 1 << 1,
    LEFT        = 1 << 2,
    RIGHT       = 1 << 3,

    UP_LEFT     = UP | LEFT,
    UP_RIGHT    = UP | RIGHT,
    DOWN_LEFT   = DOWN | LEFT,
    DOWN_RIGHT  = DOWN | RIGHT,
};

enum class CellType : uint8_t
{
    Null    = 0,
    Sand    = 1,
    Water   = 2
};

struct Cell
{
    CellType cellType;
};

struct CellMovement
{
    virtual ~CellMovement() = default;
    [[nodiscard]] virtual std::vector<Direction> GetMovementDirections() const = 0;
};

struct SandMovement final : CellMovement
{
    [[nodiscard]] std::vector<Direction> GetMovementDirections() const override
    {
        std::array<Direction, 3> directions = {
            Direction::DOWN,
            Direction::DOWN_LEFT,
            Direction::DOWN_RIGHT
        };

        static thread_local std::mt19937 rng(std::random_device{}());

        if (rng() % 2 == 0)
            std::swap(directions[1], directions[2]);

        return std::vector<Direction>{directions.begin(), directions.end()};
    }
};


struct WaterMovement final : CellMovement
{
    [[nodiscard]] std::vector<Direction> GetMovementDirections() const  override
    {
        std::array<Direction, 5> directions = {
            Direction::DOWN,
            Direction::DOWN_LEFT,
            Direction::DOWN_RIGHT,
            Direction::LEFT,
            Direction::RIGHT,
        };

        static thread_local std::mt19937 rng(std::random_device{}());

        if (rng() % 2 == 0)
        {
            std::swap(directions[1], directions[2]);
            std::swap(directions[3], directions[4]);
        }

        // Return as vector
        return std::vector<Direction>{directions.begin(), directions.end()};
    }
};


// todo: this doesn't have much use
struct Vector2i
{
    int x;
    int y;
};

class GameUtils
{
public:

    static Vector2i GetDirection(const Direction& direction)
    {
        switch (direction)
        {
            case UP: return {0, -1};
            case DOWN: return {0, 1};
            case LEFT: return {-1, 0};
            case RIGHT: return {1, 0};
            case UP_LEFT: return {-1, -1};
            case UP_RIGHT: return {1, -1};
            case DOWN_LEFT: return {-1, 1};
            case DOWN_RIGHT: return {1, 1};
            default: return {0, 0}; // fallback
        }
    }

    static bool GetNeighbourIndex(const int32_t& originIndex, const Direction& direction, const Vector2i& extents, int32_t& outIndex)
    {
        const auto [dx, dy] = GetDirection(direction);
        int x = originIndex % extents.x;
        int y = originIndex / extents.x;

        x += dx;
        y += dy;

        if (x < 0 || x > extents.x - 1 || y < 0 || y > extents.y - 1)
        {
            outIndex = -1;
            return false;
        }

        outIndex = y * extents.x + x;
        return true;
    }

    static std::vector<int32_t> GetNeighbouringCells(int32_t originIndex, const Vector2i& extents, int32_t distance = 1)
    {
        std::vector<int32_t> neighbours;
        neighbours.reserve((2 * distance + 1) * (2 * distance + 1) - 1); // exclude center

        const int originX = originIndex % extents.x;
        const int originY = originIndex / extents.x;

        for (int dy = -distance; dy <= distance; ++dy)
        {
            const int y = originY + dy;
            if (y >= extents.y) continue;

            for (int dx = -distance; dx <= distance; ++dx)
            {
                const int x = originX + dx;
                if (dx == 0 && dy == 0) continue;
                if (x >= extents.x) continue;

                neighbours.push_back(y * extents.x + x);
            }
        }

        return neighbours;
    }

    static uint8_t CellTypeBit(const CellType& type)
    {
        return 1 << static_cast<uint8_t>(type);
    }

    static bool CanDisplace(const CellType& from, const CellType& to)
    {
        const uint8_t DisplacementMask[] = {
            /* Null  */ 0,
            /* Sand  */ static_cast<uint8_t>(CellTypeBit(CellType::Null) | CellTypeBit(CellType::Water)),
            /* Water */ static_cast<uint8_t>(CellTypeBit(CellType::Null))
        };

        return (DisplacementMask[static_cast<uint8_t>(from)] & CellTypeBit(to)) != 0;
    }
};
