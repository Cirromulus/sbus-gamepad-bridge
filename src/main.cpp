#include <include/common_state.hpp>
#include <include/hid_handling.h>
#include <include/sbus_decoder.hpp>
#include <include/sbus_uart.hpp>
#include <include/dummy_hexprint.hpp>

#include <pico/multicore.h>

#include <limits>
#include <stdlib.h>
#include <string.h>

static constexpr size_t onboardLedNr = 16;
static constexpr size_t failsafeAfter_ms = 500;

void sbus_handling()
{
    static SbusDecoder decoder;
    SbusUart<failsafeAfter_ms> receiver{uart0, 1};

    bool hadValidSbus = false;

    // debug code: Uart printing
    uart_init(uart1, 115200);
    // set the gpio function to uart
    gpio_set_function(4, GPIO_FUNC_UART);

    uart_puts(uart1, "INITED\r\n");
    //

    while(1)
    {
      // debug code ----
      const std::optional<uint8_t> sbusByte = receiver.getByte();
      if (!sbusByte)
      {
        global::blinkInfo->setState(BlinkInfo::State::noSbus);
        uart_puts(uart1, "no\r\n");
        continue;
      }

      if (!hadValidSbus)
      {
        uart_putc(uart1, ' ');
        hexprint(uart1, *sbusByte);
      }
      // ----

      const auto maybeSbusFrameSync =
        decoder.consumeChar(*sbusByte)
          .transform([](auto s){return global::sbusState.setLatest(s);});

      if (maybeSbusFrameSync || maybeSbusFrameSync.error() == SbusDecoder::DecodeState::consumed)
      {
        // this will probably blink only very short,
        // because it is overwritten soon again by the noSbus state (no timeout)
        global::blinkInfo->setState(BlinkInfo::State::receivingValidSbus);
        hadValidSbus = true;
      }
      else
      {
        global::blinkInfo->setState(BlinkInfo::State::lostSbusFrame);
        hadValidSbus = false;
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