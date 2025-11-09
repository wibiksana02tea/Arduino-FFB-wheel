/*
  Arduino Leonardo Force Feedback Wheel firmware - STM32 Port
  Copyright 2015-2025
  NOTE: This file has been adapted for an STM32 target.
*/

#include "Config_STM32.h"
#include "USBCore_STM32.h"
#include "HAL_STM32.h" // Include the new HAL header
#include "debug.h"
#include "ffb_pro.h"

// (Other library includes remain the same)
#ifdef USE_AS5600
#include "AS5600.h"
#endif

// (Globals remain the same)
fwOpt fwOptions;
s16a accel, clutch, hbrake;
s32a brake;
s32v turn, axis, ffbs;
u32 button = 0;
// ... other globals

#ifdef USE_AS5600
AS5600L as5600x(0x36);
#ifdef USE_TWOFFBAXIS
#ifdef USE_TCA9548
AS5600L as5600y(0x36);
#endif
#endif
#endif

void TcaChannelSel(uint8_t addr, uint8_t ch) {
    uint8_t data = 1 << ch;
    hal_i2c_write(addr, &data, 1);
}

void setup() {
  hal_init(); // Initialize the HAL
  USBDevice.attach(); // Initialize and enable the (simulated) USB device

  // (Variable initializations remain the same)
  accel.val = 0;
  // ...

#ifdef USE_EEPROM
  // SetEEPROMConfig(); // Needs STM32 Flash implementation
  // LoadEEPROMConfig();
#endif
  ROTATION_MAX = int32_t(float(CPR) / 360.0 * float(ROTATION_DEG));
  ROTATION_MID = ROTATION_MAX >> 1;

  InitInputs(); // Now uses HAL
  FfbSetDriver(0);
  InitPWM();    // Now uses HAL

  ffbs.x = 0;
#ifdef USE_TWOFFBAXIS
  ffbs.y = 0;
#endif
  SetPWM(&ffbs); // Now uses HAL

#ifdef USE_AS5600
  hal_i2c_init();
#ifdef USE_TCA9548
  TcaChannelSel(0x70, 0);
#endif
  // as5600x.begin();
  // as5600x.resetCumulativePosition(ROTATION_MID);
#endif

  last_refresh = hal_micros();
}

void loop() {
  now_micros = hal_micros();
  timeDiffConfigSerial = now_micros - last_ConfigSerial;

  if ((now_micros - last_refresh) >= CONTROL_PERIOD) {
    last_refresh = now_micros;

#ifdef USE_AS5600
#ifdef USE_TCA9548
      TcaChannelSel(0x70, 0);
#endif
      // turn.x = as5600x.getCumulativePosition() - ROTATION_MID;
#ifdef USE_TWOFFBAXIS
#ifdef USE_TCA9548
      TcaChannelSel(0x70, 1);
      // turn.y = as5600y.getCumulativePosition() - ROTATION_MID;
#endif
#endif
#endif

    axis.x = turn.x;
    ffbs = gFFB.CalcTorqueCommands(&axis);
    turn.x *= float(X_AXIS_PHYS_MAX) / float(ROTATION_MAX);
    turn.x = constrain(turn.x, -MID_REPORT_X - 1, MID_REPORT_X);

    SetPWM(&ffbs);

#ifdef AVG_INPUTS
    AverageAnalogInputs();
#else
    accel.val = hal_adc_read(ACCEL_PIN);
    brake.val = hal_adc_read(BRAKE_PIN);
    clutch.val = hal_adc_read(CLUTCH_PIN);
#endif

    // (Calibration logic remains)

    button = readInputButtons();

    // (Shifter logic remains)

    SendInputReport(turn.x + MID_REPORT_X + 1, brake.val, accel.val, clutch.val, hbrake.val, button);

    // (Config CDC logic needs porting)
  }
}
