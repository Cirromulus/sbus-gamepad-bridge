
#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

// We only want one descriptor, but many would be possible.
// See tinyusb/examples/device/hid_composite for more

enum
{
  REPORT_ID_SBUS = 1,
  REPORT_ID_COUNT
};



/// HID Gamepad Protocol Report, but with SBUS 16 channels.
typedef struct TU_ATTR_PACKED
{
  uint16_t axis[2];
  uint8_t signals;  ///< Buttons mask for currently pressed buttons
} hid_sbus_report_t;


#endif /* USB_DESCRIPTORS_H_ */
