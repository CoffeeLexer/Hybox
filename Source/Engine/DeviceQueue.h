#pragma once

#include <optional>
#include <cstdint>

struct Carcass;

struct QueueFamilyIndices
{
    std::optional<uint32_t> transfer{};
    std::optional<uint32_t> graphics{};
    std::optional<uint32_t> compute{};
    bool isComplete()
    {
        return transfer && graphics && compute;
    }
};

QueueFamilyIndices FindQueueFamily(Carcass *carcass);
