#ifndef _HAL_STM32_H_
#define _HAL_STM32_H_

#include <stdint.h>
#include "Config_STM32.h" // For pin definitions

// HAL Initialization
void hal_init();

// Timing functions
uint32_t hal_micros();
void hal_delay_ms(uint32_t ms);

// GPIO functions
void hal_gpio_pin_mode(uint32_t pin, uint8_t mode); // mode: INPUT, OUTPUT, etc.
uint8_t hal_gpio_digital_read(uint32_t pin);
void hal_gpio_digital_write(uint32_t pin, uint8_t val);

// ADC functions
void hal_adc_init();
uint16_t hal_adc_read(uint32_t pin);

// PWM functions
void hal_pwm_init();
void hal_pwm_write(uint32_t pin, uint16_t value);

// I2C functions
void hal_i2c_init();
void hal_i2c_write(uint8_t address, uint8_t* data, uint32_t length);
void hal_i2c_read(uint8_t address, uint8_t* data, uint32_t length);
void hal_i2c_begin_transmission(uint8_t address);
void hal_i2c_write_byte(uint8_t data);
void hal_i2c_end_transmission();


// Placeholder definitions for pin modes (would be defined by STM32 HAL)
#define HAL_GPIO_INPUT_PULLUP 0x02
#define HAL_GPIO_OUTPUT_PP    0x01


#endif // _HAL_STM32_H_
