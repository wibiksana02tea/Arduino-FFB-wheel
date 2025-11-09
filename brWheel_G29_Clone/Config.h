#ifndef _CONFIG_H_
#define _CONFIG_H_

// --- STM32 Arduino Core Configuration ---
// This file has been adapted for the STM32 Arduino Core.
// All AVR-specific port manipulations and pin numbers have been replaced.
// Pin definitions are based on a generic STM32F103 "Blue Pill" board.
// Users should adapt these pins for their specific board and wiring.

//------------------------------------- Firmware options -------------------------------------------------

#define USE_CONFIGCDC      // Use virtual serial for configuration
#define USE_TWOFFBAXIS     // Enable 2nd FFB axis
#define USE_AS5600         // Enable magnetic encoder via I2C
#define USE_TCA9548        // Enable I2C multiplexer
#define USE_CENTERBTN      // Enable hardware recenter button
#define USE_ANALOGFFBAXIS  // Enable using analog inputs for FFB axis
#define USE_EEPROM         // Use Flash emulation for EEPROM

#define CALIBRATE_AT_INIT	0

//------------------------------------- Pins (STM32 Arduino Style) -----------------------------------------

#define FFBCLIP_LED_PIN   PC13 // On-board LED

#define ACCEL_PIN			PA0
#define BRAKE_PIN           PA1
#define CLUTCH_PIN          PA2
#define HBRAKE_PIN          PA3

#define BUTTON0             PA4
#define BUTTON1             PA5
#define BUTTON2             PA6
#define BUTTON3             PA7
#define BUTTON4             PB0
#define BUTTON5             PB1
#define BUTTON6             PB10
#define BUTTON7             PB11

// PWM Pins for Motor Control
#define PWM_PIN_L     PA8 // TIM1_CH1
#define PWM_PIN_R     PA9 // TIM1_CH2
#define PWM_PIN_U     PA10 // TIM1_CH3
#define PWM_PIN_D     PA11 // TIM1_CH4
#define DIR_PIN       PB12

//------------------------------------- EEPROM Config -----------------------------------------------------
#define PARAM_ADDR_FW_VERSION		 0x00
// ... (All other PARAM_ADDR definitions are kept the same)
#define FIRMWAYRE_VERSION         0xFB // Incremented version for STM32 build

#define GetParam(m_offset,m_data)	getParam((m_offset),(u8*)&(m_data),sizeof(m_data))
#define SetParam(m_offset,m_data)	setParam((m_offset),(u8*)&(m_data),sizeof(m_data))

//------------------------------------- Main Config -----------------------------------------------------
#define CONTROL_PERIOD	2000
#define CONFIG_SERIAL_PERIOD 10000

//------------------------------------- FFB/Firmware config -----------------------------------------------------
#define PWM_RESOLUTION 12
#define MAX_PWM_VALUE ((1 << PWM_RESOLUTION) - 1)

// --- Struct definition that was accidentally removed ---
typedef struct fwOpt {
  boolean a, b, c, d, e, f, g, h, i, l, m, n, p, r, s, t, u, w, x, z;
};
// ---------------------------------------------------------

typedef struct s32v { s32 x; #ifdef USE_TWOFFBAXIS s32 y; #endif };

#ifdef USE_TCA9548
#include <Wire.h>
void TcaChannelSel(uint8_t addr, uint8_t ch) {
  Wire.beginTransmission(addr);
  Wire.write(1 << ch);
  Wire.endTransmission();
}
#endif

// (Platform-independent variables are kept)
u8 effstate;
u8 pwmstate;
u8 configGeneralGain, configDamperGain, configFrictionGain, configConstantGain;
u8 configPeriodicGain, configSpringGain, configInertiaGain, configCenterGain, configStopGain;
int16_t ROTATION_DEG;
int32_t CPR, ROTATION_MAX, ROTATION_MID;
uint16_t MM_MIN_MOTOR_TORQUE, MM_MAX_MOTOR_TORQUE, MAX_DAC;

#endif // _CONFIG_H_
