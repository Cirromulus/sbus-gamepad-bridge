
#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

#include <include/tusb_config.h>

// We only want one descriptor, but many would be possible.
// See tinyusb/examples/device/hid_composite for more

enum
{
  REPORT_ID_SBUS_1 = 1,
  REPORT_ID_SBUS_2,
  REPORT_ID_COUNT
};


#define NUM_AXIS_PER_REPORT 8

/// HID Gamepad Protocol Report, Part 1.
typedef struct TU_ATTR_PACKED
{
  uint16_t axis[NUM_AXIS_PER_REPORT];
  uint8_t signals;  ///< Buttons mask for currently pressed buttons
} hid_sbus_report_t;

/// HID Gamepad Protocol Report, Part 2 (remaining 8 channels)
typedef struct TU_ATTR_PACKED
{
  uint16_t axis[NUM_AXIS_PER_REPORT];
} hid_sbus_report_extension_t;


static_assert(CFG_TUD_HID_EP_BUFSIZE >= sizeof(hid_sbus_report_t));

#endif /* USB_DESCRIPTORS_H_ */
