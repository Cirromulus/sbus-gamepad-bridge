#pragma once

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

class WS2812LED
{
    PIO pio;
    uint sm;
    int offset;

public:
    struct Color
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        constexpr
        operator uint32_t() const
        {
            return  ((uint32_t) (red) << 8) |
                    ((uint32_t) (green) << 16) |
                    (uint32_t) (blue);
        }

        static constexpr
        uint8_t scaleByte(uint8_t value, uint8_t factor255)
        {
            return (static_cast<uint16_t>(value) * factor255) / 255;
        }

        constexpr
        Color
        operator*(uint8_t lightness255) const
        {
            return Color{
                    scaleByte(red, lightness255),
                    scaleByte(green, lightness255),
                    scaleByte(blue, lightness255)
                    };
        }
    };

    inline
    WS2812LED(unsigned pin, PIO pio = pio0, uint sm = 0) :
        pio{pio},
        sm{sm},
        offset{pio_add_program(pio, &ws2812_program)}
    {
        ws2812_program_init(pio, sm, offset, pin, 800000, false /* not rgbw */);
    }

    inline
    void put_pixel(uint32_t pixel_grb) {
        pio_sm_put_blocking(pio, sm, pixel_grb << 8u);
    }
};