#include "Config_STM32.h"
#include "HAL_STM32.h"
#include "ffb_pro.h"

void InitPWM() {
  // PWM Timers are started by the HAL now.
  hal_pwm_init();

  TOP = MAX_PWM_RESOLUTION;
  MM_MAX_MOTOR_TORQUE = TOP;
}

void SetPWM(s32v *torque) {
  if (torque == NULL) return;

  // FFB clipping LED logic
  if (abs(torque->x) >= 0.9 * MM_MAX_MOTOR_TORQUE) {
      // hal_gpio_digital_write(FFB_CLIP_PIN, 1);
  } else {
      // hal_gpio_digital_write(FFB_CLIP_PIN, 0);
  }

  // NOTE: PWM mode logic for PWM+-, PWM+DIR etc. is retained.
  // The output is now directed to timer channels instead of raw pins.
  if (!bitRead(pwmstate, 1)) { // PWM+- mode
      if (torque->x > 0) {
          s32 mapped_torque = map(torque->x, 0, MM_MAX_MOTOR_TORQUE, 0, MM_MAX_MOTOR_TORQUE);
          hal_pwm_write(TIM_CHANNEL_1, 0); // Left channel
          hal_pwm_write(TIM_CHANNEL_2, mapped_torque); // Right channel
      } else {
          s32 mapped_torque = map(-torque->x, 0, MM_MAX_MOTOR_TORQUE, 0, MM_MAX_MOTOR_TORQUE);
          hal_pwm_write(TIM_CHANNEL_1, mapped_torque); // Left channel
          hal_pwm_write(TIM_CHANNEL_2, 0); // Right channel
      }
  } else { // PWM+DIR mode
      if (torque->x >= 0) {
          // hal_gpio_digital_write(DIR_PIN, 1);
      } else {
          // hal_gpio_digital_write(DIR_PIN, 0);
      }
      s32 mapped_torque = map(abs(torque->x), 0, MM_MAX_MOTOR_TORQUE, 0, MM_MAX_MOTOR_TORQUE);
      hal_pwm_write(TIM_CHANNEL_1, mapped_torque); // Magnitude channel
  }

#ifdef USE_TWOFFBAXIS
  // Similar logic for the Y-axis using TIM_CHANNEL_3 and TIM_CHANNEL_4
#endif
}
