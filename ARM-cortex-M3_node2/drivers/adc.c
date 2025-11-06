#include "../sam/sam3x/include/sam.h"
#include "adc.h"

#define ADC_PIN

void ADC_init() {

    // Set mode freerun (new conversion starts automatically)
    ADC->ADC_MR |= (1 << 7);

    // Enable chanel register
    ADC->ADC_CHER |= (1 << 0); // Enable channel 0

    // Activate clock for ADC
    PMC->PMC_PCR |= (1 << 28) | (37 << 0); // Set enable bit, leave DIV unset to use MCK as clock, leave CMD unset to enable read and set PID for ADC
    PMC->PMC_PCER1 |= (1 << 5); // Set PID 37 bit

    // Start conversion
    ADC->ADC_CR |= (1 << 1);
}

int ADC_read() {

    // Return data register value
    return ADC->ADC_CDR[0];
}