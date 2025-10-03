#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <array>

struct SbusChannels
{
    static constexpr size_t numAxis {16};

    std::array<uint16_t, numAxis> axis;
    uint8_t flags;

    constexpr
    bool getChannel17() const
    {
        return flags & 0b0001;
    }

    constexpr
    bool getChannel18() const
    {
        return flags & 0b0010;
    }

    constexpr
    bool isFailsafeOn() const
    {
        return flags & 0b0100;
    }

    constexpr
    bool wasFrameLost() const
    {
        return flags & 0b1000;
    }

    constexpr bool
    operator==(SbusChannels const& other) const
    {
        return axis == other.axis && flags == other.flags;
    }
};
