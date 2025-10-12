#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <array>

struct SbusChannels
{
    static constexpr size_t numAxis = 16;
    static constexpr uint8_t ch17Mask = 0b0001;
    static constexpr uint8_t ch18Mask = 0b0010;
    static constexpr uint8_t failsafeMask = 0b0100;
    static constexpr uint8_t frameLostMask = 0b1000;


    std::array<uint16_t, numAxis> axis;
    uint8_t flags;

    constexpr
    bool getChannel17() const
    {
        return flags & ch17Mask;
    }

    constexpr
    bool getChannel18() const
    {
        return flags & ch17Mask;
    }

    constexpr
    bool isFailsafeOn() const
    {
        return flags & failsafeMask;
    }

    constexpr
    bool wasFrameLost() const
    {
        return flags & frameLostMask;
    }

    constexpr bool
    operator==(SbusChannels const& other) const
    {
        return axis == other.axis && flags == other.flags;
    }
};
