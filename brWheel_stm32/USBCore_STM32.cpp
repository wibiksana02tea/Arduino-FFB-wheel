#include "USBCore_STM32.h"
#include "ffb_pro.h" // For gFFB object

// --- Main HID Input Report Sending Function ---

void USB_SendInputReport(void* report, uint16_t len) {
    // This function sends the standard joystick input report to the host.
    USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)report, len);
}


// --- Force Feedback (FFB) Handling ---

// This callback function is called by the STM32 USB device stack (typically from usbd_hid.c)
// when a HID OUT report is received from the host. This report contains the FFB commands.
void HID_ReceiveReport_Callback(uint8_t* buffer, uint16_t len) {
    // The received buffer contains the FFB data packet.
    // We pass it to the FFB processing library.
    gFFB.FfbOnUsbData(buffer, len);
}
