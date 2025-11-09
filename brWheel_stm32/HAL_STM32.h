#ifndef _HAL_STM32_H_
#define _HAL_STM32_H_

#include <stdint.h>
// Include the main STM32 HAL header.
// The specific device header (e.g., stm32f4xx_hal.h) will be set by the build environment.
#include "stm32_hal_includes.h" // Placeholder for actual HAL includes

#include "Config_STM32.h"

// --- Hardware Handle extern declarations ---
// These handles are defined by CubeMX and are needed by the HAL functions.
extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim1; // Timer for PWM
extern TIM_HandleTypeDef htim2; // Timer for micros()

// Define a structure to represent a GPIO Pin for convenience
typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} hal_pin_t;

// --- HAL Function Prototypes ---

// HAL Initialization
void hal_init();

// Timing functions
uint32_t hal_micros();
void hal_delay_ms(uint32_t ms);

// GPIO functions
void hal_gpio_pin_mode(hal_pin_t gpio_pin, uint8_t mode);
uint8_t hal_gpio_digital_read(hal_pin_t gpio_pin);
void hal_gpio_digital_write(hal_pin_t gpio_pin, uint8_t val);

// ADC functions
uint16_t hal_adc_read(hal_pin_t adc_pin);

// PWM functions
void hal_pwm_init();
void hal_pwm_write(uint32_t pwm_channel, uint16_t value);

// I2C functions
void hal_i2c_write(uint8_t address, uint8_t* data, uint32_t length);
void hal_i2c_read(uint8_t address, uint8_t* data, uint32_t length);

#endif // _HAL_STM32_H_
