#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>

#define ADC_BASE 0x1000
#define ADC ((volatile uint8_t*)ADC_BASE)

// Init function
int init_clock(void);


// Read function
int* ADC_read(void);