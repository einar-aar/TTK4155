#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>

#define ADC_BASE 0x1000
#define ADC ((volatile uint8_t*)ADC_BASE)

// Init function
int init_clock(void);

// Read OLED
int* ADC_read_joystick_and_pad(void);

// Read IO module
void ADC_read_IO_module(uint8_t right_buttons[6], uint8_t left_buttons[7], uint8_t navigation_buttons[5]);

// Read joystick button state
uint8_t read_joystick_button(void);