//
// Created by Remus on 22/07/2025.
//


#pragma once
#include "GameUtils.h"

struct CellBehaviour
{
    virtual ~CellBehaviour() = default;
    [[nodiscard]] virtual std::vector<Direction> GetMovementDirections() { return {}; }
    [[nodiscard]] virtual CellType Mix(const CellType& original, const CellType& other) { return original; }
    [[nodiscard]] virtual int32_t GetLifetime() { return -1; }

protected:
    std::vector<Direction> movementDirections;
};

struct PowderBehaviour final : CellBehaviour
{
    PowderBehaviour()
    {
        movementDirections = {
            Direction::DOWN,
            Direction::DOWN_LEFT,
            Direction::DOWN_RIGHT
        };
    }

    [[nodiscard]] std::vector<Direction> GetMovementDirections() override
    {
        thread_local std::mt19937 rng(std::random_device{}());

        if (rng() % 2 == 0)
            std::swap(movementDirections[1], movementDirections[2]);

        return std::vector<Direction>{movementDirections.begin(), movementDirections.end()};
    }
};

struct LiquidBehaviour  : CellBehaviour
{
    LiquidBehaviour()
    {
        movementDirections = {
            Direction::DOWN,
            Direction::DOWN_LEFT,
            Direction::DOWN_RIGHT,
            Direction::LEFT,
            Direction::RIGHT,
        };
    }

    [[nodiscard]] std::vector<Direction> GetMovementDirections() override
    {
        thread_local std::mt19937 rng(std::random_device{}());

        if (rng() % 2 == 0)
        {
            std::swap(movementDirections[1], movementDirections[2]);
            std::swap(movementDirections[3], movementDirections[4]);
        }

        return std::vector<Direction>{movementDirections.begin(), movementDirections.end()};
    }

    [[nodiscard]] CellType Mix(const CellType& original, const CellType& other) override
    {
        switch (other)
        {
            case CellType::Fire:
            {
                return CellType::Steam;
            }
            default: return original;
        }
    }
};

struct GasBehaviour : CellBehaviour
{
    GasBehaviour()
    {
        movementDirections = {
            Direction::UP,
            Direction::UP,
            Direction::UP_LEFT,
            Direction::UP_RIGHT,
            Direction::LEFT,
            Direction::RIGHT,
        };
    }

    [[nodiscard]] std::vector<Direction> GetMovementDirections() override
    {
        thread_local std::mt19937 rng(std::random_device{}());
        std::ranges::shuffle(movementDirections, rng);
        return movementDirections;
    }
};

struct SteamBehaviour final : GasBehaviour
{
    [[nodiscard]] int32_t GetLifetime() override { return 120; }
};

struct FireBehaviour : GasBehaviour
{
    [[nodiscard]] int32_t GetLifetime() override { return 10; }
};

struct MethaneBehaviour : GasBehaviour
{
    [[nodiscard]] CellType Mix(const CellType& original, const CellType& other) override
    {
        switch (other)
        {
            case CellType::Fire:
            {
                return CellType::Fire;
            }
            default: return original;
        }
    }
};
