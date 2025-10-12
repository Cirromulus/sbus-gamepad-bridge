#pragma once

#include <hardware/uart.h>


// template <uart_inst_t* uartType, size_t pin>
// pico sdk is not really template-friendly :F
class SbusUart
{
    static constexpr inline size_t baudrate = 100'000;
    static constexpr inline size_t dataBits = 8;
    static constexpr inline size_t stopBits = 2;
    static constexpr inline uart_parity_t parity = UART_PARITY_EVEN;

public:

    void
    initializeSbusUart()
    {
        // Big thanks to
        // https://github.com/GitJer/Some_RPI-Pico_stuff/blob/main/SBUS/gpio_invert/SBUS.cpp

        uart_init(mUart, baudrate);
        // set the gpio function to uart
        gpio_set_function(mPin, GPIO_FUNC_UART);
        // set data bits, stop_bits and the parity of the uart
        uart_set_format(mUart, dataBits, stopBits, parity);
        // set the input gpio pin to inverted
        gpio_set_inover(mPin, GPIO_OVERRIDE_INVERT);
    }

    // This function will block until a character has been read
    uint8_t
    getByte()
    {
        return uart_getc(mUart);
    }

// private: // i am lazy
    uart_inst_t* mUart;
    size_t mPin;
};