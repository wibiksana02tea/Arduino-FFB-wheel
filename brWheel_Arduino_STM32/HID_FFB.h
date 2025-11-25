#pragma once

#include <USBHID.h>

// Standard HID Joystick Report structure
typedef struct {
    int16_t xAxis;
    int16_t yAxis;
    int16_t zAxis;
    int16_t rxAxis;
    int16_t ryAxis;
    uint32_t buttons;
} HID_Report;

// HID FFB Descriptor - this describes the joystick and its FFB capabilities
const uint8_t HID_FFB_ReportDescription[] = {
  0x05, 0x01,	// USAGE_PAGE (Generic Desktop)
  0x09, 0x04,	// USAGE (Joystick)
  0xA1, 0x01,	// COLLECTION (Application)
  0x85, 0x04,	// REPORT_ID (04)
  0x09, 0x01, // USAGE (Pointer)
  0xA1, 0x00, // COLLECTION (Physical)

  0x09, 0x30,          // USAGE (x)
  0x17, 0x00, 0x80, 0x00, 0x00, // LOGICAL_MINIMUM (-32768)
  0x27, 0xFF, 0x7F, 0x00, 0x00, // LOGICAL_MAXIMUM (32767)
  0x75, 16,   // REPORT_SIZE (16)
  0x95, 0x01,            // REPORT_COUNT (1)
  0x81, 0x02,         // INPUT (Data,Var,Abs)

  0x09, 0x31,         // USAGE (y)
  0x17, 0x00, 0x80, 0x00, 0x00,
  0x27, 0xFF, 0x7F, 0x00, 0x00,
  0x75, 16,
  0x95, 0x01,
  0x81, 0x02,

  0x09, 0x32,         // USAGE (z)
  0x17, 0x00, 0x80, 0x00, 0x00,
  0x27, 0xFF, 0x7F, 0x00, 0x00,
  0x75, 16,
  0x95, 0x01,
  0x81, 0x02,

  0x09, 0x33,         // USAGE (rx)
  0x17, 0x00, 0x80, 0x00, 0x00,
  0x27, 0xFF, 0x7F, 0x00, 0x00,
  0x75, 16,
  0x95, 0x01,
  0x81, 0x02,

  0x09, 0x34,         // USAGE (ry)
  0x17, 0x00, 0x80, 0x00, 0x00,
  0x27, 0xFF, 0x7F, 0x00, 0x00,
  0x75, 16,
  0x95, 0x01,
  0x81, 0x02,

  0x05, 0x09,       // USAGE_PAGE (Button)
  0x19, 0x01,		// USAGE_MINIMUM (button 1)
  0x29, 32,         // USAGE_MAXIMUM (button 32)
  0x15, 0x00,       // LOGICAL_MINIMUM (0)
  0x25, 0x01,       // LOGICAL_MAXIMUM (1)
  0x75, 0x01,       // REPORT_SIZE (1)
  0x95, 32,			// REPORT_COUNT (32)
  0x81, 0x02,       // Input (Data,Var,Abs)

  0xC0,             // END_COLLECTION (Physical)

  // Force Feedback HID Descriptor starts here
  0x05, 0x0F,	// USAGE_PAGE (Physical Interface)
  0x09, 0x92,	// USAGE (PID State Report)
  0xA1, 0x02,	// COLLECTION (Logical)
  0x85, 0x02,	// REPORT_ID (02)
  0x09, 0x9F,	// USAGE (Device Paused)
  0x09, 0xA0,	// USAGE (Actuators Enabled)
  0x09, 0xA4,	// USAGE (Safety Switch)
  0x09, 0xA5,	// USAGE (Actuator Override Switch)
  0x09, 0xA6,	// USAGE (Actuator Power)
  0x15, 0x00,	// LOGICAL_MINIMUM (00)
  0x25, 0x01,	// LOGICAL_MINIMUM (01)
  0x75, 0x01,	// REPORT_SIZE (01)
  0x95, 0x05,	// REPORT_COUNT (05)
  0x81, 0x02,	// INPUT (Data,Var,Abs)
  0x95, 0x03,	// REPORT_COUNT (03)
  0x81, 0x03,	// INPUT (Constant,Var,Abs)
  0x09, 0x94,	// USAGE (Effect Playing)
  0x95, 0x01,	// REPORT_COUNT (01)
  0x81, 0x02,	// INPUT (Data,Var,Abs)
  0x09, 0x22,	// USAGE (Effect Block Index)
  0x25, 0x28,	// LOGICAL_MAXIMUM (40)
  0x75, 0x07,	// REPORT_SIZE (07)
  0x95, 0x01,	// REPORT_COUNT (01)
  0x81, 0x02,	// INPUT (Data,Var,Abs)
  0xC0,	// END COLLECTION

  0x09, 0x21,	// USAGE (Set Effect Output Report)
  0xA1, 0x02,	// COLLECTION (Logical)
  0x85, 0x01,	// REPORT_ID (01)
  0x09, 0x22,	// USAGE (Effect Block Index)
  0x25, 0x28,	// LOGICAL_MAXIMUM (40)
  0x75, 0x08,	// REPORT_SIZE (08)
  0x95, 0x01,	// REPORT_COUNT (01)
  0x91, 0x02,	// OUTPUT (Data,Var,Abs)
  0x09, 0x25,	// USAGE (Effect type)
  0xA1, 0x02,	// COLLECTION (Logical)
  0x09, 0x26,	// USAGE (ET Constant Force)
  // ... (and so on for all other effect types)
  0x25, 0x0B,   // LOGICAL_MAXIMUM (11)
  0x95, 0x01,	// REPORT_COUNT (01)
  0x91, 0x00,	// OUTPUT (Data)
  0xC0,	        // END COLLECTION
  // ... (rest of the FFB descriptor)

  0xC0,	// END COLLECTION (Application)
};

// HID FFB class that extends the base USBHID
class HID_FFB : public USBHID {
public:
    HID_FFB(void) : USBHID(HID_FFB_ReportDescription, sizeof(HID_FFB_ReportDescription), 64, 64) {
    };

    void sendReport(HID_Report* report) {
        this->sendReport(4, report, sizeof(HID_Report));
    }

    void setFFBCallback(void (*callback)(uint8_t *)) {
        this->hid_get_report_callback = callback;
    }
};
