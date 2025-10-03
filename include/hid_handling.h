

#include <include/tusb_config.h>
#include <bsp/board_api.h>
#include <tusb.h>

#include "usb_descriptors.h"

namespace hid
{
    void init();

    // updates stuff
    void task();
};