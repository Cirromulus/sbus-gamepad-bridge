#include "synchronizer_buffer.hpp"
#include "sbus.hpp"
#include "blinkstatus.hpp"

#include <optional>

namespace global
{

// How ugly is that!?
// This is a promise that somewhere, there is an oracle that returns sbus states.
// This will produced by the SBUS parser and consumed by the usb hid.
extern SynchronizerBuffer<SbusChannels> sbusState;

// optional for delayed initialization
extern std::optional<BlinkInfo> blinkInfo;

} // namespace global