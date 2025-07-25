//
// Created by Remus on 20/07/2025.
//


#pragma once
#include <cstdint>
#include <random>

#include "raylib.h"

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
    Null        = 0,
    Wall        = 1,
    Sand        = 2,
    Water       = 3,
    Steam       = 4,
    Fire        = 5,
    Methane     = 6,

    END
};

struct Cell
{
    CellType cellType;
    int32_t cellLifetime = -1;
    int32_t cellMaxLifetime = -1;
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
            if (y < 0 || y >= extents.y) continue;

            for (int dx = -distance; dx <= distance; ++dx)
            {
                const int x = originX + dx;
                if (dx == 0 && dy == 0) continue;
                if (x >= extents.x) continue;

                if (int idx = y * extents.x + x; idx >= 0)
                {
                    neighbours.push_back(y * extents.x + x);
                }
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
            /* Wall  */ 0,
            /* Sand  */     static_cast<uint8_t>(CellTypeBit(CellType::Null) | CellTypeBit(CellType::Water)),
            /* Water */     static_cast<uint8_t>(CellTypeBit(CellType::Null) | CellTypeBit(CellType::Fire)),
            /* Steam  */    static_cast<uint8_t>(CellTypeBit(CellType::Null) | CellTypeBit(CellType::Water)),
            /* Fire */      static_cast<uint8_t>(CellTypeBit(CellType::Null) | CellTypeBit(CellType::Steam) | CellTypeBit(CellType::Water) | CellTypeBit(CellType::Methane)),
            /* Methane */   static_cast<uint8_t>(CellTypeBit(CellType::Null) | CellTypeBit(CellType::Water) | CellTypeBit(CellType::Fire))
        };

        return (DisplacementMask[static_cast<uint8_t>(from)] & CellTypeBit(to)) != 0;
    }

    static Color Lerp(const Color& a, const Color& b, const float& t)
    {
        Color result;
        result.r = static_cast<unsigned char>(a.r + (b.r - a.r) * t);
        result.g = static_cast<unsigned char>(a.g + (b.g - a.g) * t);
        result.b = static_cast<unsigned char>(a.b + (b.b - a.b) * t);
        result.a = static_cast<unsigned char>(a.a + (b.a - a.a) * t);
        return result;
    }

    static Color GetCellTypeColor(const Cell& cell)
    {
        switch (cell.cellType)
        {
            case CellType::Wall:
            {
                return CLITERAL(Color){ 55, 55, 55, 255 };
            }
            case CellType::Sand:
            {
                return CLITERAL(Color){ 225,191,146, 255 };
            }
            case CellType::Water:
            {
                return CLITERAL(Color){ 111, 122, 252, 255 };
            }
            case CellType::Steam:
            {
                return CLITERAL(Color){ 0, 0, 55, 255 };
            }
            case CellType::Fire:
            {
                return Lerp({255,0, 0, 255}, {255,154, 0, 255}, static_cast<float>(cell.cellLifetime) / cell.cellMaxLifetime);
            }
            case CellType::Methane:
            {
                return CLITERAL(Color){ 0, 55, 55, 255 };
            }
            default: return BLACK;
        }
    }

};
