#include "XMEM.h"


int external_memory_init(void) {

    // Set the external memory bit in the MCU control register
    MCUCR |= (1 << SRE);

    // Enable masking of highest 4 bits (PC7-PC4) in the Special function IO register
    SFIOR |= (1 << XMM2);

    return 0; // Success
}


void SRAM_send(uint16_t address, uint8_t data) {

    /*uint16_t mask = 0b0000111111111111;
    uint16_t new_address = bitmask(address, mask);*/

    SRAM[address] = data;
}

uint8_t SRAM_read(uint16_t address) {

    /*uint16_t mask = 0b0000111111111111;
    uint16_t new_address = bitmask(address, mask);*/

    return SRAM[address];
}