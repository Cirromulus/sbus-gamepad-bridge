#pragma once

#include <inttypes.h>

struct SbusFrame
{
    uint8_t start;   // should be always 0xF0
    uint8_t data[22];
    uint8_t flag;
    uint8_t end;     // should be always zero
};

struct SbusChannels
{
    uint16_t rc[16];
    uint8_t data;

    constexpr
    bool getChannel17() const
    {
        return data & 0b0001;
    }

    constexpr
    bool getChannel18() const
    {
        return data & 0b0010;
    }

    constexpr
    bool isFailsafeOn() const
    {
        return data & 0b0100;
    }

    constexpr
    bool wasFrameLost() const
    {
        return data & 0b1000;
    }

};