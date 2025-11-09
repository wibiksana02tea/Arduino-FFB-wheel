#ifndef _USBCORE_STM32_H_
#define _USBCORE_STM32_H_

#include <stdint.h>

// This file is a placeholder for the STM32 USB Core implementation.
// The functions and classes defined here should be implemented using the
// STM32 USB Device Library (e.g., from STM32Cube HAL).
// The original API from the AVR implementation is mimicked here to
// ease the porting of the main application logic.

// USB HID report descriptor and other definitions would go here.
// These should match the original descriptors from USBDesc.h

// Placeholder for the main USB device class
class USBDevice_
{
public:
    USBDevice_();
    void attach();
    void detach();
    bool configured();
    void poll();
};

extern USBDevice_ USBDevice;

// Function prototypes to be implemented in USBCore_STM32.cpp
// These functions will replace the direct AVR register manipulation.

void USB_Send(uint8_t ep, const void* d, int len);
int USB_Recv(uint8_t ep, void* d, int len);
uint8_t USB_Available(uint8_t ep);
void USB_Flush(uint8_t ep);

// FFB (Force Feedback) specific functions
void SendInputReport(int16_t x, int16_t y, int16_t z, int16_t rz, int16_t rx, uint32_t buttons);
bool HID_ReportAvailable();
int16_t HID_ReceiveReport(uint8_t* buffer, int16_t len);


#endif // _USBCORE_STM32_H_
