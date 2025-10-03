#include <include/common_state.hpp>
#include <include/hid_handling.h>
#include <include/sbus_decoder.hpp>

#include <pico/multicore.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static constexpr size_t onboardLedNr = 16;
static constexpr bool testingValues = true;

void coprocessor()
{
    static SbusDecoder decoder;

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
        // normal operation
        // TODO: SBUS link layer (aka serial 100'000 baud)
        std::optional<uint8_t> newChar{};
        newChar
          .and_then([](auto c){return decoder.consumeChar(c);})
          .transform([](auto s){return global::sbusState.setLatest(s);});
      }

    }
}

int main(void)
{
    WS2812LED builtin{onboardLedNr};
    global::blinkInfo.emplace(BlinkInfo{builtin});
    global::blinkInfo->setState(BlinkInfo::State::usbIniting);

    multicore_reset_core1();
    multicore_launch_core1(coprocessor);

    hid::init();

    while (1)
    {
        tud_task(); // tinyusb device task
        hid::task();
    }
}