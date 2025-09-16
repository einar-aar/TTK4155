#include "BUSdriver.h"
#include <util/delay.h>


// Bitmask function
/*uint16_t bitmask (uint16_t bit, uint16_t bitmask) {

    return (bit & bitmask);
}*/


void BUS_send(uint16_t address, uint8_t data) {

    /*uint16_t mask = 0b0000111111111111;
    uint16_t new_address = bitmask(address, mask);*/

    SRAM[address] = data;
}

uint8_t BUS_read(uint16_t address) {

    /*uint16_t mask = 0b0000111111111111;
    uint16_t new_address = bitmask(address, mask);*/

    return SRAM[address];
}
// Send data over BUS
/*void BUS_send (uint16_t address, uint8_t data) {

    // Sending address first, then data

    uint16_t mask = 0b0000111111111111;
    uint16_t new_address = bitmask(address, mask);


    // As long as XMEM is activated, the mcu will automatically toggle the ALE pin
    // PORTA |= ALE;

    //ensuring that data pins & address pins are set to output

    if (DDRA != 0b11111111) {        // checking if PA0–PA7 are configured as output
        DDRA = 0b11111111;
    }


    // Toggle Write pin for SRAM
    PORTD |= PD6;

    // Toggle ALE
    PORTE |= PE1;

    // Manually send all address bits for every pin
    if (new_address & (1 << 11)) {

        PORTC |= PC3;
    }

    if (new_address & (1 << 10)) {

        PORTC |= PC2;
    }
   
    if (new_address & (1 << 9)) {

        PORTC |= PC1;
    }

    if (new_address & (1 << 8)) {

        PORTA |= PC0;
    }

    if (new_address & (1 << 7)) {

        PORTA |= PA7;
    }

    if (new_address & (1 << 6)) {

        PORTA |= PA6;
    }

    if (new_address & (1 << 5)) {

        PORTA |= PA5;
    }

    if (new_address & (1 << 4)) {

        PORTA |= PA4;
    }

    if (new_address & (1 << 3)) {

        PORTA |= PA3;
    }

    if (new_address & (1 << 2)) {

        PORTA |= PA2;
    }

    if (new_address & (1 << 1)) {

        PORTA |= PA1;
    }

    if (new_address & (1 << 0)) {

        PORTA |= PA0;
    }

    // Sending data

    // Delay
    _delay_ms(50);

    // Toggle ALE
    PORTE |= PE1;

    // Reset porta data register
    PORTA = 0;

    if (data & (1 << 7)) {

        PORTA |= PA7;
    }

    if (data & (1 << 6)) {

        PORTA |= PA6;
    }

    if (data & (1 << 5)) {
	0x0800
        PORTA |= PA5;
    }

    if (data & (1 << 4)) {

        PORTA |= PA4;
    }

    if (data & (1 << 3)) {

        PORTA |= PA3;
    }

    if (data & (1 << 2)) {

        PORTA |= PA2;
    }

    if (data & (1 << 1)) {

        PORTA |= PA1;
    }

    if (data & (1 << 0)) {

        PORTA |= PA0;
    }

    // Toggle Write pin for SRAM
    PORTD |= PD6;
}


// BUS read fuction
uint8_t BUS_read (uint16_t address) {

    uint16_t mask = 0b0000111111111111;
    uint16_t new_address = bitmask(address, mask);

    
    //ensuring that address pins are set as output

    if (DDRA != 0b11111111) {        // checking if PA0–PA7 are configured as output
        DDRA = 0b11111111;
    }

    // Toggle Read signal
    if(PORTD >> PD7 == 1 ) {
        PORTD |= PD7;
    }

    // Toggle ALE
    PORTE |= PE1;

    // Manually send all address bits for every pin
    if (new_address & (1 << 11)) {

        PORTC |= PC3;
    }

    if (new_address & (1 << 10)) {

        PORTC |= PC2;
    }
   
    if (new_address & (1 << 9)) {

        PORTC |= PC1;
    }

    if (new_address & (1 << 8)) {

        PORTA |= PC0;
    }

    if (new_address & (1 << 7)) {

        PORTA |= PA7;
    }

    if (new_address & (1 << 6)) {

        PORTA |= PA6;
    }

    if (new_address & (1 << 5)) {

        PORTA |= PA5;
    }

    if (new_address & (1 << 4)) {

        PORTA |= PA4;
    }

    if (new_address & (1 << 3)) {

        PORTA |= PA3;
    }

    if (new_address & (1 << 2)) {

        PORTA |= PA2;
    }

    if (new_address & (1 << 1)) {

        PORTA |= PA1;
    }

    if (new_address & (1 << 0)) {

        PORTA |= PA0;
    }

    // Delay
    _delay_ms(50);

    // Toggle ALE
    PORTE |= PE1;

    // Reset porta data register
    PORTA = 0;

    //reading data

    //ensuring that data pins are set to input

    if (DDRA != 0) {        // checking if PA0–PA7 are configured as input
        DDRA = 0;
    }

    // delay
    _delay_ms(50);

    // Activate/deactivate internal pull-up
    // PORTA = 0b11111111;
    PORTA = 0b00000000;

    // Read data pins
    uint8_t result = PINA;

    return result;    
}*/