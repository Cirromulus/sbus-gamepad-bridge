#include <inttypes.h>
#include "ws2812led.hpp"

class BlinkInfo
{
    // fixedpoint fraction up to 255.
    static constexpr uint8_t lightness = 20;

public:
    enum class State
    {
        usbIniting,
        notMounted,
        mounted,
        suspended,
        noSbus,
        receivingValidSbus,
        lostSbusFrame,
    };

    void
    setState(State const& state);

    // Task that decides the state of the LED based on state
    void update();

    constexpr BlinkInfo(WS2812LED& led) : mLed{led}
    {
    }

private:

    static constexpr WS2812LED::Color
    getColorFromState(State const& state)
    {
        // small note: Values below `lightness` will probably become zero.
        switch (state)
        {
            case State::usbIniting:
                return WS2812LED::Color{40, 20, 180} * lightness;
            case State::notMounted:
                return WS2812LED::Color{255, 20, 20} * lightness;
            case State::mounted:
                return WS2812LED::Color{0, 100, 0} * lightness;
            case State::suspended:
                return WS2812LED::Color{0, 100, 200} * lightness;
            case State::noSbus:
                return WS2812LED::Color{100, 0, 100} * lightness;
            case State::receivingValidSbus:
                return WS2812LED::Color{0, 255, 100} * lightness;
            case State::lostSbusFrame:
                return WS2812LED::Color{200, 200, 0} * lightness;
        }

        // should never happen
        return WS2812LED::Color{0, 1, 0};
    }

    State mState = State::notMounted;

    WS2812LED& mLed;
};