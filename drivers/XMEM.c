#include "XMEM.h"


int external_memory_init(void) {

    // Set the external memory bit in the MCU control register
    MCUCR |= (1 << SRE);

    // Enable masking of highest 4 bits (PC7-PC4) in the Special function IO register
    SFIOR |= (1 << XMM2);

    return 0; // Success
}