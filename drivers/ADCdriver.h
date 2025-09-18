#include <avr/io.h>

#define ADC_BASE 0x1000
#define ADC ((volatile uint8_t*)ADC_BASE)

// Init function
int init_clock(void);


// Read function
uint8_t ADC_read(uint16_t address);