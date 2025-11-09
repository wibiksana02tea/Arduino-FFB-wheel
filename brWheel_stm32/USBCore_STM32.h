#ifndef _USBCORE_STM32_H_
#define _USBCORE_STM32_H_

#include <stdint.h>
#include "stm32_hal_includes.h" // Placeholder for actual HAL includes
#include "usbd_hid.h" // Standard STM32 USB HID header

// --- USB Device Handle ---
// This handle is defined by CubeMX in usb_device.c
extern USBD_HandleTypeDef hUsbDeviceFS;

// --- Function Prototypes ---

// These functions provide a simplified interface to the STM32 USB stack.
void USB_SendInputReport(void* report, uint16_t len);

// FFB (Force Feedback) specific functions
// The main application will need a callback to handle received FFB data.
void HID_ReceiveReport_Callback(uint8_t* buffer, uint16_t len);

#endif // _USBCORE_STM32_H_
