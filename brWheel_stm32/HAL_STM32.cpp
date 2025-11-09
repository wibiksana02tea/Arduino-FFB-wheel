#include "HAL_STM32.h"
#include <map> // For simulating hardware states
#include <vector> // For simulating I2C bus

// NOTE: This file requires the STM32 HAL library to be included in the project.
// This implementation SIMULATES the hardware for development purposes.

// --- Simulated Hardware State ---
struct PinState {
    uint8_t mode;
    uint16_t value;
};
std::map<uint32_t, PinState> simulated_pins;
std::map<uint8_t, std::vector<uint8_t>> i2c_device_memory; // Simulate memory for I2C devices
// --------------------------------

volatile uint32_t g_micros_count = 0;

void hal_init() {
    simulated_pins.clear();
    i2c_device_memory.clear();
    g_micros_count = 0;
}

// Timing functions
uint32_t hal_micros() {
    g_micros_count += 1000;
    return g_micros_count;
}

void hal_delay_ms(uint32_t ms) {
    uint32_t start = hal_micros();
    while((hal_micros() - start) < (ms * 1000));
}

// GPIO functions
void hal_gpio_pin_mode(uint32_t pin, uint8_t mode) {
    simulated_pins[pin].mode = mode;
    if (mode == HAL_GPIO_INPUT_PULLUP) simulated_pins[pin].value = 1;
}

uint8_t hal_gpio_digital_read(uint32_t pin) {
    return (simulated_pins.count(pin)) ? simulated_pins[pin].value : 0;
}

void hal_gpio_digital_write(uint32_t pin, uint8_t val) {
    if (simulated_pins.count(pin) && simulated_pins[pin].mode == HAL_GPIO_OUTPUT_PP) {
        simulated_pins[pin].value = val;
    }
}

// ADC functions
void hal_adc_init() {}
uint16_t hal_adc_read(uint32_t pin) { return 2048; }

// PWM functions
void hal_pwm_init() {
    hal_gpio_pin_mode(PWM_PIN_L, HAL_GPIO_OUTPUT_PP);
    hal_gpio_pin_mode(PWM_PIN_R, HAL_GPIO_OUTPUT_PP);
}

void hal_pwm_write(uint32_t pin, uint16_t value) {
    if (simulated_pins.count(pin)) simulated_pins[pin].value = value;
}

// I2C functions (Simulated Implementation)
void hal_i2c_init() {
    // Initializes the simulated I2C bus
}

void hal_i2c_write(uint8_t address, uint8_t* data, uint32_t length) {
    // Simulate writing data to an I2C device's memory
    i2c_device_memory[address].assign(data, data + length);
}

void hal_i2c_read(uint8_t address, uint8_t* data, uint32_t length) {
    // Simulate reading data from an I2C device.
    // For AS5600, this would return angle data. We'll return a fixed value.
    if (i2c_device_memory.count(address)) {
        // This is a simplification. A real simulation would be more complex.
    }
    for(uint32_t i=0; i<length; ++i) data[i] = (i==0) ? 0x0C : 0x00; // Simulate some data
}

// The simple Wire-like functions are not strictly needed if using the above, but are here for compatibility.
void hal_i2c_begin_transmission(uint8_t address) {}
void hal_i2c_write_byte(uint8_t data) {}
void hal_i2c_end_transmission() {}
