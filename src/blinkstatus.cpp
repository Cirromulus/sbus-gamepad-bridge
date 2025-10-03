#include <include/blinkstatus.hpp>
#include <generated/ws2812.pio.h>

void
BlinkInfo::setState(State const& state)
{
    mState = state;
    mLed.put_pixel(getColorFromState(state));
}

void
BlinkInfo::update()
{
    // nothing time-based yet.
    // and to avoid re-sending the same stuff,
    // nothing is done here.
}