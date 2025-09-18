
#include <avr/io.h>
#include "drivers/UARTdriver.h"
#include "drivers/BUSdriver.h"
#include "drivers/XMEM.h"
#include "drivers/ADCdriver.h"

// ATmega162 cpu frequency
#define F_CPU 4915200UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <util/delay.h>
#include "sram_test.h"


int main(void) {

    /*DDRA = (0b00000001 << PA0); // Sets pin PA0 as output, rest on PA as input

    PORTA = (0b00000001 << PA1); // Sets pin PA1 as input with pullup, PA0 logic low*/
    uart0_init(MYUBRR);
    printf("Test\n");
    
    /*
    while (1) {

int external_memory_init(void) {

    // Set the external memory bit in the MCU control register
    MCUCR |= (1 << SRE);

    // Enable masking of highest 4 bits (PC7-PC4) in the Special function IO register
    SFIOR |= (1 << XMM2);

    return 0; // Success
}

        uart0_tx('A', NULL);

    }*/

    /*DDRA |= (0b00000001 << PA0) | (0b00000001 << PA1) | (0b00000001 << PA2) | (0b00000001 << PA3)
            | (0b00000001 << PA4) | (0b00000001 << PA5) | (0b00000001 << PA6) | (0b00000001 << PA7);

    DDRE |= (0b00000001 << PE1);

    PORTE = (0b00000001 << PE1);*/





    /*while (1) {

        PORTA = 0b11111111;
        _delay_ms(1000);
        PORTA = 0b00000000;
        _delay_ms(1000);

    }*/

    // initialize ADC
    if (init_clock() != 0) {

        printf("Failed to initialize ADC");
    }

    // Initialize external memory
    if (external_memory_init() != 0) {

        printf("Failed to initialize external memory");
    }

    SRAM_test();

    uint16_t message_sent = 79;

    SRAM_send(0x0200, message_sent);

    uint8_t message_received = SRAM_read(0x0200);

    printf("Message sent: %d\n\rMessage received: %d\n\r", message_sent, message_received);

    return 0;
}

