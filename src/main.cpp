#include <include/common_state.hpp>
#include <include/hid_handling.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static constexpr size_t onboardLedNr = 16;

int main(void)
{
    WS2812LED builtin{onboardLedNr};
    global::blinkInfo.emplace(BlinkInfo{builtin});
    global::blinkInfo->setState(BlinkInfo::State::usbIniting);

    hid::init();

    while (1)
    {
        tud_task(); // tinyusb device task
        hid::task();
    }
}