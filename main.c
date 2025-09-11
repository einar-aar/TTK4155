
#include <avr/io.h>
#include "drivers/UARTdriver.h"


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
    printf("Test");
    
    /*
    while (1) {

        uart0_tx('A', NULL);

    }*/

    DDRA |= (0b00000001 << PA0) | (0b00000001 << PA1) | (0b00000001 << PA2) | (0b00000001 << PA3)
            | (0b00000001 << PA4) | (0b00000001 << PA5) | (0b00000001 << PA6) | (0b00000001 << PA7);

    DDRE |= (0b00000001 << PE1);

    PORTE = (0b00000001 << PE1);

    /*while (1) {

        PORTA = 0b11111111;
        _delay_ms(1000);
        PORTA = 0b00000000;
        _delay_ms(1000);

    }*/

    SRAM_test();

    return 0;
}

