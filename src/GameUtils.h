//
// Created by Remus on 20/07/2025.
//


#pragma once
#include <cstdint>
#include "systems/FallingSandsSystem.h"

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
        Vector2i result{0, 0};
        if (direction & UP)        result.y -= 1;
        if (direction & DOWN)      result.y += 1;
        if (direction & LEFT)      result.x -= 1;
        if (direction & RIGHT)     result.x += 1;
        return result;
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
