#include "ADCdriver.h"


// Initialize clock
int init_clock(void) {

    // Set pin D4 as output for PWM signal
    DDRD |= (1 << PD5);

    // Register for timer 1
    // This will make sure the selected pin will toggle when the timer matches the defined toggle-time
    TCCR1A |= (1 << COM1A0);

    // FYI: The timer has 2 registers: A and B. These have different bit-information tied to them. See page 128 and 131 in datasheet

    // Initialize CTC mode. This makes sure the timer is reset every time the timer reaches the defined value.
    TCCR1A |= (1 << WGM10);
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << WGM13);

    // Set the value that the timer should be compared to and trigger output
    // The formula for frequency equals F_CPU / 2 * prescaler * (1 + OCR1A)
    OCR1A = 1;
    // This sets the clock frequency to 4.9152 / 2*(1 + 1) == 1,2288 MHz

    // Choose prescaler to make the frequency between 0,5 MHz to 5MHz
    // We choose prescaler == 1 to make the clock frequency == mcu clock frequency
    TCCR1B |= (1 << CS10);

    return 0;
}


// Read function
uint8_t ADC_read(uint16_t address) {

    /*uint16_t mask = 0b0000111111111111;
    uint16_t new_address = bitmask(address, mask);*/

    return ADC[address];
}