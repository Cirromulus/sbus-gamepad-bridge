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

void coprocessor()
{
    static SbusDecoder decoder;
    static SbusUart receiver{uart0, 2};

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
        decoder.consumeChar(receiver.getByte())
          .transform([](auto s){return global::sbusState.setLatest(s);});
          // ignoring output for now.
          // A "has value" with false means that we could not update the state
          // through the thread synchronization mechanism.
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