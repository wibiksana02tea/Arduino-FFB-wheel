/*
  brWheel_my.ino - Ported for STM32 Arduino Core
*/

#include "Config.h"
#include "debug.h"
#include "ffb_pro.h"
#include <USBHID.h>      // STM32 Arduino: Use the built-in USB HID library
#include "HID_FFB.h"     // STM32 Arduino: Include the Force Feedback HID extension

// (Other library includes)
#ifdef USE_QUADRATURE_ENCODER
#include "QuadEncoder.h"
#endif
#include <Wire.h>
#ifdef USE_EEPROM
#include <EEPROM.h>
#endif
// ... other includes

// --- Globals ---
// (Globals remain the same)
s32v turn, axis, ffbs;
u32 button = 0;
// ... other globals

// --- USB HID and FFB Objects ---
HID_FFB Ffb;

// This callback is triggered by the USBHID library when FFB data is received
void HID_FFB_callback(uint8_t *report)
{
  gFFB.FfbOnUsbData(report, 64); // Pass the data to our FFB processing logic
}

void setup() {
  Serial.begin(115200); // For debug output

  // (Variable initializations remain the same)
  // ...

  InitInputs();
  FfbSetDriver(0);
  InitPWM();

  // --- USB HID Initialization ---
  Ffb.setFFBCallback(HID_FFB_callback);
  Ffb.begin();
  // -----------------------------

  last_refresh = micros();
}


void loop() {
  now_micros = micros();

  if ((now_micros - last_refresh) >= CONTROL_PERIOD) {
    last_refresh = now_micros;

    // (Encoder and axis reading logic remains the same)
    // ...

    axis.x = turn.x;
    ffbs = gFFB.CalcTorqueCommands(&axis);
    turn.x *= float(X_AXIS_PHYS_MAX) / float(ROTATION_MAX);
    turn.x = constrain(turn.x, -MID_REPORT_X - 1, MID_REPORT_X);

    SetPWM(&ffbs);

    // (Analog axis and button reading logic remains the same)
    // ...
    button = readInputButtons();

    // --- Send HID Report via USBHID Library ---
    HID_Report report;
    report.xAxis = turn.x + MID_REPORT_X + 1;
    report.yAxis = brake.val;
    report.zAxis = accel.val;
    report.rxAxis = clutch.val;
    report.ryAxis = hbrake.val;
    report.buttons = button;

    Ffb.sendReport(&report);
    // ------------------------------------------

    // (Config CDC logic remains the same)
    // ...
  }
}
