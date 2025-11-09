#include "USBCore_STM32.h"
#include "debug.h" // For DEBUG_SERIAL

// This implementation SIMULATES the USB HID functionality.

// --- HID Report Structure ---
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
    int16_t rz;
    int16_t rx;
    uint32_t buttons;
} HID_InputReport_t;
// --------------------------

USBDevice_ USBDevice;
bool _usb_configured = false;

USBDevice_::USBDevice_() {}

void USBDevice_::attach() {
    // In a real implementation, this would start the USB peripheral.
    _usb_configured = true;
}

void USBDevice_::detach() {
    _usb_configured = false;
}

bool USBDevice_::configured() {
    // Simulate that the USB device is always configured after attach() is called.
    return _usb_configured;
}

void USBDevice_::poll() {}


void SendInputReport(int16_t x, int16_t y, int16_t z, int16_t rz, int16_t rx, uint32_t buttons) {
    if (!USBDevice.configured()) {
        return;
    }

    HID_InputReport_t report;
    report.x = x;
    report.y = y;
    report.z = z;
    report.rz = rz;
    report.rx = rx;
    report.buttons = buttons;

    // In a real implementation, this would call the STM32 HAL function to send the report.
    // e.g., USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&report, sizeof(report));

    // For simulation, we can print the report data.
    // DEBUG_SERIAL.print("HID Report: X=");
    // DEBUG_SERIAL.print(x);
    // DEBUG_SERIAL.print(" Y=");
    // DEBUG_SERIAL.print(y);
    // ... and so on
}

// FFB (OUT report) functions remain as stubs for now.
bool HID_ReportAvailable() {
    return false;
}

int16_t HID_ReceiveReport(uint8_t* buffer, int16_t len) {
    return -1;
}
