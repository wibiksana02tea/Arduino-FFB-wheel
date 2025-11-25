/*
  Inputs.ino - Ported for STM32 Arduino Core
*/

#include "Config.h"
#include "QuadEncoder.h"
#include "debug.h"
#include <Arduino.h>
#include <Wire.h>

void InitInputs() {
  for (u8 i = 0; i < sizeof(analog_inputs_pins); i++) {
    pinMode(analog_inputs_pins[i], INPUT);
  }
  InitButtons();
}

void InitButtons() {
  pinMode(BUTTON0, INPUT_PULLUP);
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  pinMode(BUTTON5, INPUT_PULLUP);
  pinMode(BUTTON6, INPUT_PULLUP);
  pinMode(BUTTON7, INPUT_PULLUP);
}

u32 readInputButtons() {
  u32 buttons = 0;

  bitWrite(buttons, 0, !digitalRead(BUTTON0));
  bitWrite(buttons, 1, !digitalRead(BUTTON1));
  bitWrite(buttons, 2, !digitalRead(BUTTON2));
  bitWrite(buttons, 3, !digitalRead(BUTTON3));
  bitWrite(buttons, 4, !digitalRead(BUTTON4));
  bitWrite(buttons, 5, !digitalRead(BUTTON5));
  bitWrite(buttons, 6, !digitalRead(BUTTON6));
  bitWrite(buttons, 7, !digitalRead(BUTTON7));
  // ... and so on for more buttons if defined

#ifdef USE_HATSWITCH
  // buttons = decodeHat(buttons); // This function is platform-independent
#else
  buttons = buttons << 4;
#endif

  return buttons;
}
