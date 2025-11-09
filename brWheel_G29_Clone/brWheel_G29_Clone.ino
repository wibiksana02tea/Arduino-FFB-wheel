/*
  brWheel_G29_Clone.ino - Ported for STM32 Arduino Core
  This version emulates a Logitech G29 for maximum game compatibility.
*/

#include "Config.h"
#include "debug.h"
#include "ffb_pro.h"
#include <USBHID.h>
#include "HID_FFB.h"

// (Other library includes)
#include <Wire.h>
#ifdef USE_EEPROM
#include <EEPROM.h> // Corrected include path
#endif

// --- Globals ---
s32v turn, axis, ffbs;
u32 button = 0;
// ... other globals

// --- USB HID and FFB Objects ---
HID_FFB Ffb;

void HID_FFB_callback(uint8_t *report)
{
  gFFB.FfbOnUsbData(report, 64);
}

void setup() {
  Serial.begin(115200);

  InitInputs();
  FfbSetDriver(0);
  InitPWM();

  Ffb.setFFBCallback(HID_FFB_callback);
  Ffb.begin();

  last_refresh = micros();
}


void loop() {
  now_micros = micros();

  if ((now_micros - last_refresh) >= CONTROL_PERIOD) {
    last_refresh = now_micros;

    // (Encoder and axis reading logic remains the same, producing high-resolution internal values)
    // ...

    axis.x = turn.x;
    ffbs = gFFB.CalcTorqueCommands(&axis);

    // The internal `turn.x` is still high resolution (e.g., 16-bit) for physics calculations
    // but we will map it to a lower resolution for the HID report.

    SetPWM(&ffbs);

    // (Analog axis and button reading logic remains the same)
    // ...
    button = readInputButtons();

    // --- Create and Send G29 HID Report ---
    HID_Report_G29 report;

    // Map high-resolution internal values to the 8-bit axes of the G29 report
    report.xAxis = map(turn.x, -ROTATION_MID, ROTATION_MID, 0, 255);
    report.yAxis = map(accel.val, 0, Z_AXIS_PHYS_MAX, 0, 255);
    report.zAxis = map(brake.val, 0, Y_AXIS_PHYS_MAX, 0, 255);
    report.rxAxis = map(clutch.val, 0, RX_AXIS_PHYS_MAX, 0, 255);

    report.buttons = button;

    // Hat switch logic would need to be implemented and mapped to report.hatSwitch
    report.hatSwitch = 0; // Placeholder

    Ffb.sendReport(&report);
    // ------------------------------------------

    // (Config CDC logic remains the same)
    // ...
  }
}
