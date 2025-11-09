// pwm.ino - Ported for STM32 Arduino Core

#include "Config.h"

void InitPWM() {
  pinMode(DIR_PIN, OUTPUT);
  pinMode(FFBCLIP_LED_PIN, OUTPUT);

  // Set the PWM resolution for all motor pins
  analogWriteResolution(PWM_RESOLUTION);

  MM_MAX_MOTOR_TORQUE = MAX_PWM_VALUE;
  minTorquePP = ((float)MM_MIN_MOTOR_TORQUE) / ((float)MM_MAX_MOTOR_TORQUE);

  // Blink the LED to signal successful boot
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(FFBCLIP_LED_PIN, HIGH);
    delay(20);
    digitalWrite(FFBCLIP_LED_PIN, LOW);
    delay(20);
  }
}

void activateFFBclipLED(s32 t) {
  if (abs(t) >= 0.9 * MM_MAX_MOTOR_TORQUE) {
    digitalWrite(FFBCLIP_LED_PIN, HIGH);
  } else {
    digitalWrite(FFBCLIP_LED_PIN, LOW);
  }
}

void SetPWM(s32v *torque) {
  if (torque == NULL) return;

  activateFFBclipLED(torque->x);

  // Simplified PWM logic for STM32 using analogWrite
  if (!bitRead(pwmstate, 1)) { // PWM+- mode
      if (torque->x > 0) {
          long mapped_torque = map(torque->x, 0, MM_MAX_MOTOR_TORQUE, 0, MAX_PWM_VALUE);
          analogWrite(PWM_PIN_L, 0);
          analogWrite(PWM_PIN_R, mapped_torque);
      } else {
          long mapped_torque = map(-torque->x, 0, MM_MAX_MOTOR_TORQUE, 0, MAX_PWM_VALUE);
          analogWrite(PWM_PIN_L, mapped_torque);
          analogWrite(PWM_PIN_R, 0);
      }
  } else { // PWM+DIR mode
      if (torque->x >= 0) {
          digitalWrite(DIR_PIN, HIGH);
      } else {
          digitalWrite(DIR_PIN, LOW);
      }
      long mapped_torque = map(abs(torque->x), 0, MM_MAX_MOTOR_TORQUE, 0, MAX_PWM_VALUE);
      analogWrite(PWM_PIN_L, mapped_torque); // Use one pin for magnitude
  }

#ifdef USE_TWOFFBAXIS
  // Similar logic for Y axis on PWM_PIN_U and PWM_PIN_D
  if (torque->y > 0) {
      long mapped_torque_y = map(torque->y, 0, MM_MAX_MOTOR_TORQUE, 0, MAX_PWM_VALUE);
      analogWrite(PWM_PIN_D, 0);
      analogWrite(PWM_PIN_U, mapped_torque_y);
  } else {
      long mapped_torque_y = map(-torque->y, 0, MM_MAX_MOTOR_TORQUE, 0, MAX_PWM_VALUE);
      analogWrite(PWM_PIN_D, mapped_torque_y);
      analogWrite(PWM_PIN_U, 0);
  }
#endif
}
