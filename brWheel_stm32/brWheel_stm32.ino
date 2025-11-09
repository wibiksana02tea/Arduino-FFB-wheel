/*
  Arduino Leonardo Force Feedback Wheel firmware - STM32 Port
  Copyright 2015-2025
  NOTE: This file has been adapted for an STM32 target.
  AVR-specific code has been commented out and marked for porting.
*/

// STM32 Port: Include the new configuration and USB core headers.
#include "Config_STM32.h"
#include "USBCore_STM32.h"

// STM32 Port: These files will also need to be ported or replaced.
#include "debug.h"
#include "ffb_pro.h"

// STM32 Port: These libraries need to be verified for STM32 compatibility
// or replaced with STM32-specific versions.
#ifdef USE_QUADRATURE_ENCODER
#include "QuadEncoder.h"
#endif
#ifdef USE_VNH5019
#include "DualVNH5019MotorShield.h"
#endif
// STM32 Port: Use the STM32 HAL I2C library.
// #include <Wire.h>
#ifdef USE_EEPROM
// STM32 Port: Use STM32 Flash emulation for EEPROM.
// #include <EEPROM.h>
#endif
#ifdef USE_LOAD_CELL
#include <HX711_ADC.h>
#endif
#ifdef USE_LCD
#include <LiquidCrystal_I2C.h>
#endif
#ifdef USE_ADS1015
#include <Adafruit_ADS1015.h>
#endif
#ifdef USE_MCP4725
#include <Adafruit_MCP4725.h>
#endif
#ifdef USE_AS5600
#include "AS5600.h"
#endif


//extern u8 valueglobal; // milos, commented out

//--------------------------------------- Globals --------------------------------------------------------
// (Globals are mostly platform-independent and are kept)
fwOpt fwOptions;
s16a accel, clutch, hbrake;
#ifdef USE_XY_SHIFTER
xysh shifter;
#endif
s32a brake;
s32v turn;
s32v axis;
s32v ffbs;
u32 button = 0;

#ifdef USE_ADS1015
Adafruit_ADS1015 ads(0x48);
#endif

#ifdef USE_MCP4725
Adafruit_MCP4725 dac0;
Adafruit_MCP4725 dac1;
#endif

cFFB gFFB;
BRFFB brWheelFFB;

#ifdef AVG_INPUTS
extern s32 analog_inputs[];
u8 asc = 0;
#endif

u32 last_ConfigSerial = 0;
u32 last_refresh = 0;
u32 now_micros;
u32 timeDiffConfigSerial;

uint16_t dz, bdz;
uint8_t last_LC_scaling;
//----------------------------------------- Options -------------------------------------------------------

#ifdef USE_LOAD_CELL
// STM32 Port: Pin numbers are placeholders from Config_STM32.h
HX711_ADC LoadCell(4, 5); // Placeholder pins
#endif

#ifdef USE_QUADRATURE_ENCODER
cQuadEncoder myEnc;
#endif

#ifdef USE_LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#endif

#ifdef USE_VNH5019
DualVNH5019MotorShield ms;
void stopIfFault() {
  if (ms.getM1Fault()) {
    // DEBUG_SERIAL.println("M1 fault"); // STM32 Port: Replace with STM32 serial print
    while (1);
  }
  if (ms.getM2Fault()) {
    // DEBUG_SERIAL.println("M2 fault"); // STM32 Port: Replace with STM32 serial print
    while (1);
  }
}
#endif

#ifdef USE_AS5600
AS5600L as5600x(0x36);
#ifdef USE_TWOFFBAXIS
#ifdef USE_TCA9548
AS5600L as5600y(0x36);
#endif
#endif
#endif

//--------------------------------------------------------------------------------------------------------
//-------------------------------------------- SETUP -----------------------------------------------------
//--------------------------------------------------------------------------------------------------------

void setup() {
  // STM32 Port: Replace with STM32 UART/Serial initialization
  // CONFIG_SERIAL.begin(115200);

  accel.val = 0;
  brake.val = 0;
  clutch.val = 0;
  axis.x = 0;
  turn.x = 0;
#ifdef USE_TWOFFBAXIS
  axis.y = 0;
  turn.y = 0;
#endif

  // STM32 Port: All direct pin/port manipulation must be replaced with STM32 HAL calls.
  // pinModeFast(LCSYNC_LED_PIN,OUTPUT);
  // pinMode(SCK,INPUT);

#ifdef USE_EEPROM
  // STM32 Port: These functions must be rewritten to use STM32 Flash Emulation
  // SetEEPROMConfig();
  // LoadEEPROMConfig();
#else
  // This section is fine as it sets software variables.
#endif
  ROTATION_MAX = int32_t(float(CPR) / 360.0 * float(ROTATION_DEG));
  ROTATION_MID = ROTATION_MAX >> 1;

#ifdef USE_QUADRATURE_ENCODER
  // STM32 Port: The Quadrature Encoder library will need to be ported to use STM32 timers.
  // myEnc.Init(ROTATION_MID, true);
#endif

  // STM32 Port: This function in Inputs.ino must be rewritten for STM32 GPIO.
  // InitInputs();
  FfbSetDriver(0);

#ifdef USE_VNH5019
  ms.init();
#endif

  // STM32 Port: InitPWM() from pwm.ino is AVR-specific and must be completely rewritten using STM32 timers.
  // InitPWM();
  ffbs.x = 0;
#ifdef USE_TWOFFBAXIS
  ffbs.y = 0;
#endif
  // STM32 Port: SetPWM() from pwm.ino is AVR-specific and must be completely rewritten.
  // SetPWM(&ffbs);

#ifdef USE_QUADRATURE_ENCODER
  // STM32 Port: This logic needs to be adapted for the ported encoder library.
  // (CALIBRATE_AT_INIT ? brWheelFFB.calibrate() : myEnc.Write(ROTATION_MID));
#endif

#ifdef USE_ADS1015
  // STM32 Port: This relies on a working I2C implementation.
  // ads.begin();
#endif

#ifdef USE_AS5600
  // STM32 Port: This relies on a working I2C implementation (replace Wire.begin()).
  // Wire.begin();
  // TcaChannelSel(baseTCA0, 0);
  // as5600x.begin();
  // as5600x.resetCumulativePosition(ROTATION_MID);
#endif
  // STM32 Port: micros() needs to be replaced with an STM32 timer equivalent.
  // last_refresh = micros();
}

//--------------------------------------------------------------------------------------------------------
//------------------------------------ Main firmware loop ------------------------------------------------
//--------------------------------------------------------------------------------------------------------

void loop() {
  // STM32 Port: micros() needs to be replaced with an STM32 timer equivalent.
  // now_micros = micros();
  timeDiffConfigSerial = now_micros - last_ConfigSerial;

  if ((now_micros - last_refresh) >= CONTROL_PERIOD) {
    last_refresh = now_micros;

    // STM32 Port: Logic for reading digital inputs (shift registers) needs to be ported.
    // #ifdef  USE_SHIFT_REGISTER

    // STM32 Port: Logic for reading encoders needs to be ported.
    // if (zIndexFound) { ... } else { turn.x = myEnc.Read() - ROTATION_MID; }

    // STM32 Port: I2C calls for AS5600 need to use STM32 HAL
    // #ifdef USE_AS5600
    // TcaChannelSel(...)
    // turn.x = as5600x.getCumulativePosition() - ROTATION_MID;
    // #endif

    axis.x = turn.x;
    // ... (Force calculation logic is platform-independent and can be kept)
    ffbs = gFFB.CalcTorqueCommands(&axis);
    turn.x *= float(X_AXIS_PHYS_MAX) / float(ROTATION_MAX);
    turn.x = constrain(turn.x, -MID_REPORT_X - 1, MID_REPORT_X);

    // STM32 Port: SetPWM() is AVR-specific and must be rewritten.
    // SetPWM(&ffbs);

    // --- USB Report Section ---

    // STM32 Port: All analogRead and ads.readADC_... calls must be replaced with STM32 ADC reads.
    // accel.val = analogRead(ACCEL_PIN);
    // brake.val = LoadCell.getData();

    // ... (Calibration and mapping logic can be kept)

    // STM32 Port: This function from Inputs.ino must be rewritten for STM32 GPIO.
    // button = readInputButtons();

    // ... (Shifter decoding logic can be kept)

    // STM32 Port: SendInputReport now calls the function defined in USBCore_STM32.h
    // The implementation of that function will use the STM32 USB stack.
#ifdef USE_QUADRATURE_ENCODER
    SendInputReport(turn.x + MID_REPORT_X + 1, brake.val, accel.val, clutch.val, hbrake.val, button);
#elif defined(USE_AS5600)
#ifdef USE_TCA9548
    SendInputReport(turn.x + MID_REPORT_Y + 1, turn.y + MID_REPORT_Y + 1, accel.val, clutch.val, hbrake.val, button);
#else
    SendInputReport(turn.x + MID_REPORT_X + 1, brake.val, accel.val, clutch.val, hbrake.val, button);
#endif
#else
    SendInputReport(turn.x + MID_REPORT_X + 1, brake.val, Z_AXIS_PHYS_MAX >> 1, clutch.val, hbrake.val, button);
#endif


#ifdef USE_CONFIGCDC
    if (timeDiffConfigSerial >= CONFIG_SERIAL_PERIOD) {
        // STM32 Port: configCDC() from SerialInterface.ino is AVR-specific and must be rewritten for STM32 UART.
        // configCDC();
        last_ConfigSerial = now_micros;
    }
#endif
  }
}
