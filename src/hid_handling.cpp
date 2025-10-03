#include <include/tusb_config.h>
#include <include/common_state.hpp>

#include <bsp/board_api.h>
#include <class/hid/hid_device.h>
#include <tusb.h>

#include "usb_descriptors.h"

namespace hid
{
    struct Report
    {
        hid_sbus_report_t mainReport;
        hid_sbus_report_extension_t restReport;

        bool
        operator== (Report const& other)
        {
            return memcmp(&mainReport, &other.mainReport, sizeof(hid_sbus_report_t)) == 0 &&
                   memcmp(&restReport, &other.restReport, sizeof(hid_sbus_report_extension_t)) == 0;
        }
    };

    constexpr
    Report
    fromSbusState(SbusChannels const& sbus)
    {
        static_assert(NUM_AXIS_PER_REPORT == 8, "Assumption wrong");
        return Report {
            hid_sbus_report_t {
                sbus.axis[0],
                sbus.axis[1],
                sbus.axis[2],
                sbus.axis[3],
                sbus.axis[4],
                sbus.axis[5],
                sbus.axis[6],
                sbus.axis[7],
                sbus.flags
            },
            hid_sbus_report_extension_t{
                sbus.axis[8 + 0],
                sbus.axis[8 + 1],
                sbus.axis[8 + 2],
                sbus.axis[8 + 3],
                sbus.axis[8 + 4],
                sbus.axis[8 + 5],
                sbus.axis[8 + 6],
                sbus.axis[8 + 7],
            }
        };
    }

    static std::optional<Report> lastSentState{};

// ---

// "private"

    void send_main_report()
    {
        // skip if hid is not ready yet
        if ( !tud_hid_ready() || !lastSentState ) return;

        tud_hid_report(REPORT_ID_SBUS_1, &lastSentState->mainReport, sizeof(lastSentState->mainReport));
    }

    void send_ext_report()
    {
        if ( !lastSentState ) return;

        tud_hid_report(REPORT_ID_SBUS_2, &lastSentState->restReport, sizeof(lastSentState->restReport));
    }

    ///


    // ---- "public" functions

    void init()
    {
        board_init();

        // init device stack on configured roothub port
        tusb_rhport_init_t dev_init = {
            .role = TUSB_ROLE_DEVICE,
            .speed = TUSB_SPEED_AUTO
        };
        tusb_init(BOARD_TUD_RHPORT, &dev_init);

        if (board_init_after_tusb) {
            board_init_after_tusb();
        }
    }

    // updates stuff
    void task()
    {
        {
            // Limit update to every 10ms
            constexpr uint32_t interval_ms = 10;
            static uint32_t start_ms = 0;

            if ( board_millis() - start_ms < interval_ms) return; // not enough time
            start_ms += interval_ms;
        }

        const Report newState = fromSbusState(global::sbusState.getLatest());
        if (lastSentState && *lastSentState == newState)
        {
            return;
        }
        lastSentState = newState;

        // Remote wakeup
        if ( tud_suspended())
        {
            // Wake up host if we are in suspend mode
            // and REMOTE_WAKEUP feature is enabled by host
            tud_remote_wakeup();
        }

        send_main_report();
    }
};


/// ------- Tiny-USB callbacks -------

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) instance;
  (void) len;

  const uint8_t next_report_id = report[0] + 1u;

  switch (next_report_id)
  {
    case REPORT_ID_SBUS_2:
      hid::send_ext_report();
      break;
  }
}


// Invoked when device is mounted
void tud_mount_cb(void)
{
    // reset last state to send it again once mounted
    hid::lastSentState = std::nullopt;
    if (global::blinkInfo)
    {
        global::blinkInfo->setState(BlinkInfo::State::mounted);
    }
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    if (global::blinkInfo)
    {
        global::blinkInfo->setState(BlinkInfo::State::notMounted);
    }
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void) remote_wakeup_en;
    if (global::blinkInfo)
    {
        global::blinkInfo->setState(BlinkInfo::State::suspended);
    }
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
    if (tud_mounted())
    {
        tud_mount_cb();
    }
    else
    {
        tud_umount_cb();
    }
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) instance;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  (void) instance;

  if (report_type == HID_REPORT_TYPE_OUTPUT)
  {
    switch (report_id)
    {
    case REPORT_ID_SBUS_1:
    // case REPORT_ID_SBUS_2:
      // Does this implement a return path?
      // Would be interesting.
      break;
    }
  }
}

