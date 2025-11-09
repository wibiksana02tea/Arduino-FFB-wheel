#include "HAL_STM32.h"

// --- HAL Function Implementations ---
// This code assumes that the required peripherals (ADC, I2C, Timers)
// have been initialized by the STM32CubeMX generated code.

void hal_init() {
    // The main HAL_Init() and SystemClock_Config() are called in main.c
    // We start the microsecond timer here.
    HAL_TIM_Base_Start(&htim2);
}

// Timing functions
uint32_t hal_micros() {
    // Assumes TIM2 is a 32-bit timer configured with a 1MHz clock (1us per tick)
    return __HAL_TIM_GET_COUNTER(&htim2);
}

void hal_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

// GPIO functions
// NOTE: GPIO pin modes are configured in the CubeMX-generated MX_GPIO_Init()
// This function is kept for API compatibility but is not strictly needed.
void hal_gpio_pin_mode(hal_pin_t gpio_pin, uint8_t mode) {
    // Pin configuration is handled by CubeMX.
}

uint8_t hal_gpio_digital_read(hal_pin_t gpio_pin) {
    return HAL_GPIO_ReadPin(gpio_pin.port, gpio_pin.pin);
}

void hal_gpio_digital_write(hal_pin_t gpio_pin, uint8_t val) {
    HAL_GPIO_WritePin(gpio_pin.port, gpio_pin.pin, (GPIO_PinState)val);
}

// ADC functions
uint16_t hal_adc_read(hal_pin_t adc_pin) {
    // This is a simplified ADC read for a single channel.
    // A more robust implementation would handle multiple channels.
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = adc_pin.pin; // NOTE: adc_pin.pin should correspond to an ADC_CHANNEL_x
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        return 0; // Error
    }

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        return (uint16_t)HAL_ADC_GetValue(&hadc1);
    }
    return 0; // Timeout
}

// PWM functions
void hal_pwm_init() {
    // All PWM channels are started.
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
}

void hal_pwm_write(uint32_t pwm_channel, uint16_t value) {
    // Assumes htim1 is configured for PWM on the specified channels
    __HAL_TIM_SET_COMPARE(&htim1, pwm_channel, value);
}

// I2C functions
void hal_i2c_write(uint8_t address, uint8_t* data, uint32_t length) {
    // I2C address is 7-bit, HAL expects it shifted.
    HAL_I2C_Master_Transmit(&hi2c1, (address << 1), data, length, HAL_MAX_DELAY);
}

void hal_i2c_read(uint8_t address, uint8_t* data, uint32_t length) {
    HAL_I2C_Master_Receive(&hi2c1, (address << 1), data, length, HAL_MAX_DELAY);
}
