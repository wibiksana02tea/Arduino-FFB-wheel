/*
  Inputs - STM32 HAL Port
  Copyright 2015-2025
  NOTE: This file has been adapted for an STM32 target.
  All AVR-specific and Arduino functions have been replaced with calls to the HAL.
*/

#include "Config_STM32.h"
#include "HAL_STM32.h"
#include "debug.h"
#include "ffb_pro.h" // For type definitions

//--------------------------------------- Globals --------------------------------------------------------

u8 analog_inputs_pins[] =
{
  ACCEL_PIN,
#ifdef USE_LOAD_CELL
  CLUTCH_PIN,
#else
  BRAKE_PIN,
  CLUTCH_PIN,
#endif
#ifndef USE_EXTRABTN
  HBRAKE_PIN
#endif
};

#ifdef AVG_INPUTS
s32 analog_inputs[sizeof(analog_inputs_pins)];
s8 nb_mes;
#endif

//--------------------------------------------------------------------------------------------------------

#ifdef USE_LOAD_CELL
void InitLoadCell () {
  // STM32 Port: The HX711 library will need to be made platform-agnostic or ported.
  // Assuming it uses standard GPIO, it can be adapted to use the HAL.
  // LoadCell.begin();
  // LoadCell.setGain();
  // LoadCell.start(2000);
  // LoadCell.setCalFactor(0.25 * float(LC_scaling));
}
#endif

void InitInputs() {
  for (u8 i = 0; i < sizeof(analog_inputs_pins); i++) {
    // STM32 Port: Using HAL for GPIO pin mode.
    hal_gpio_pin_mode(analog_inputs_pins[i], HAL_GPIO_INPUT_PULLUP);
  }

#ifdef USE_SHIFT_REGISTER
  // STM32 Port: This will require a HAL-based implementation.
  // InitShiftRegister();
#else
  InitButtons();
#endif

#ifdef USE_LOAD_CELL
  InitLoadCell();
#endif

#ifdef AVG_INPUTS
  nb_mes = 0;
#endif
}

//--------------------------------------------------------------------------------------------------------

#ifndef USE_SHIFT_REGISTER
void InitButtons() {
  hal_gpio_pin_mode(BUTTON0, HAL_GPIO_INPUT_PULLUP);
  hal_gpio_pin_mode(BUTTON1, HAL_GPIO_INPUT_PULLUP);
  hal_gpio_pin_mode(BUTTON2, HAL_GPIO_INPUT_PULLUP);
  hal_gpio_pin_mode(BUTTON3, HAL_GPIO_INPUT_PULLUP);
#ifndef USE_BTNMATRIX
  hal_gpio_pin_mode(BUTTON4, HAL_GPIO_INPUT_PULLUP);
  hal_gpio_pin_mode(BUTTON5, HAL_GPIO_INPUT_PULLUP);
  hal_gpio_pin_mode(BUTTON6, HAL_GPIO_INPUT_PULLUP);
  hal_gpio_pin_mode(BUTTON7, HAL_GPIO_INPUT_PULLUP);
#else
  hal_gpio_pin_mode(BUTTON4, HAL_GPIO_OUTPUT_PP);
  hal_gpio_pin_mode(BUTTON5, HAL_GPIO_OUTPUT_PP);
  hal_gpio_pin_mode(BUTTON6, HAL_GPIO_OUTPUT_PP);
  setMatrixRow(BUTTON4, 1);
  setMatrixRow(BUTTON5, 1);
  setMatrixRow(BUTTON6, 1);
#ifndef USE_LOAD_CELL
  hal_gpio_pin_mode(BUTTON7, HAL_GPIO_OUTPUT_PP);
  setMatrixRow(BUTTON7, 1);
#endif // end of load cell
#endif // end of button matrix
#ifdef USE_EXTRABTN
  hal_gpio_pin_mode(BUTTON8, HAL_GPIO_INPUT_PULLUP);
  hal_gpio_pin_mode(BUTTON9, HAL_GPIO_INPUT_PULLUP);
#endif
}
#endif

u32 readInputButtons() {
  u32 buttons = 0;
#ifdef USE_SHIFT_REGISTER
  // STM32 Port: Shift register logic needs to be ported.
#else
#ifndef USE_BTNMATRIX
  bitWrite(buttons, 0, readSingleButton(BUTTON0));
  bitWrite(buttons, 1, readSingleButton(BUTTON1));
  bitWrite(buttons, 2, readSingleButton(BUTTON2));
  bitWrite(buttons, 3, readSingleButton(BUTTON3));
  bitWrite(buttons, 4, readSingleButton(BUTTON4));
  bitWrite(buttons, 5, readSingleButton(BUTTON5));
  bitWrite(buttons, 6, readSingleButton(BUTTON6));
  bitWrite(buttons, 7, readSingleButton(BUTTON7));
#ifdef USE_EXTRABTN
  bitWrite(buttons, 8, readSingleButton(BUTTON8));
  bitWrite(buttons, 9, readSingleButton(BUTTON9));
#endif
#else // do matrix button readout
  for (uint8_t i = 0; i < 4; i++) {
    setMatrixRow (i, 0); // Active low
    for (uint8_t j = 0; j < 4; j++) {
      // STM32 Port: The column pins (BUTTON0-3) need to be read here.
      // This logic needs to be completed.
      // bitWrite(buttons, i * 4 + j, readSingleButton(COLUMN_PIN));
    }
    setMatrixRow (i, 1);
  }
#endif
#endif

#ifdef USE_HATSWITCH
  // buttons = decodeHat(buttons); // decodeHat logic is platform-independent
#else
  buttons = buttons << 4;
#endif

  return (buttons);
}

#ifndef USE_SHIFT_REGISTER
bool readSingleButton (uint8_t pin) {
  // STM32 Port: Using HAL for digital read. The original code inverts the logic.
  return !hal_gpio_digital_read(pin);
}
#endif

#ifdef USE_BTNMATRIX
void setMatrixRow (uint8_t row_index, uint8_t value) {
  uint8_t pin_to_set = 0;
  if (row_index == 0) pin_to_set = BUTTON4;
  else if (row_index == 1) pin_to_set = BUTTON5;
  else if (row_index == 2) pin_to_set = BUTTON6;
  else if (row_index == 3) pin_to_set = BUTTON7;

  if(pin_to_set != 0) {
    hal_gpio_digital_write(pin_to_set, value);
  }
}
#endif

#ifdef AVG_INPUTS
void ClearAnalogInputs() {
  for (u8 i = 0; i < sizeof(analog_inputs_pins); i++) {
    analog_inputs[i] = 0;
  }
  nb_mes = 0;
}

void ReadAnalogInputs() {
  for (u8 i = 0; i < sizeof(analog_inputs_pins); i++) {
    // STM32 Port: Using HAL for ADC read.
    analog_inputs[i] += hal_adc_read(analog_inputs_pins[i]);
  }
  nb_mes++;
}

void AverageAnalogInputs() {
  for (u8 i = 0; i < sizeof(analog_inputs_pins); i++) {
    // This logic is platform-independent.
    analog_inputs[i] = (analog_inputs[i] / nb_mes);
  }
}
#endif
