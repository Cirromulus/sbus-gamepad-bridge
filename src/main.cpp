#include <include/common_state.hpp>
#include <include/hid_handling.h>
#include <include/sbus_decoder.hpp>
#include <include/sbus_uart.hpp>

#include <pico/multicore.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static constexpr size_t onboardLedNr = 16;
static constexpr bool testingValues = false;

void sbus_handling()
{
    static SbusDecoder decoder;
    SbusUart receiver{uart0, 2};

    while(1)
    {

      if constexpr (testingValues)
      {
        static SbusChannels testingState{};

        testingState.axis[0] = rand();
        // FIXME: Why would this line cause (at least) core0 to hang?
        // testingState.flags = board_button_read();

        global::sbusState.setLatest(testingState);
      }
      else
      {
        // debug code ----
        global::blinkInfo->setState(BlinkInfo::State::noSbus);
        const uint8_t sbusByte = receiver.getByte();
        global::blinkInfo->setState(BlinkInfo::State::receivingSbus);

        // ----
        const auto maybeSbusFrameSync =
          decoder.consumeChar(sbusByte /*receiver.getByte()*/)
            .transform([](auto s){return global::sbusState.setLatest(s);});

        if (maybeSbusFrameSync)
        {
          global::blinkInfo->setState(BlinkInfo::State::receivingValidSbus);
        }
        else
        {
            // Debug code! TODO: Only start after some lost frames
            static SbusChannels testingState{};
            static uint16_t counter{0};

            // TODO: Just print it out
            testingState.axis[0] = sbusByte;
            testingState.axis[1] = counter++;
            // FIXME: Why would this line cause (at least) core0 to hang?
            // testingState.flags = board_button_read();

            global::sbusState.setLatest(testingState);
        }
      }
    }
}

int main(void)
{
    WS2812LED builtin{onboardLedNr};
    global::blinkInfo.emplace(BlinkInfo{builtin});
    global::blinkInfo->setState(BlinkInfo::State::usbIniting);

    multicore_reset_core1();
    multicore_launch_core1(sbus_handling);

    hid::init();

    while (1)
    {
        tud_task(); // tinyusb device task
        hid::task();
    }
}