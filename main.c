
#include <avr/io.h>
#include "UARTdriver.h"



// ATmega162 cpu frequency
#define F_CPU 4915200UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <util/delay.h>


int main(void) {

    /*DDRA = (0b00000001 << PA0); // Sets pin PA0 as output, rest on PA as input

    PORTA = (0b00000001 << PA1); // Sets pin PA1 as input with pullup, PA0 logic low*/

    uart0_init(MYUBRR);

    printf("Test");
    
    //fdevopen(uart0_tx, uart0_rx);
    //(void)holder;

    //stdout = holder;
    //stdin = holder;

    //uart0_tx('A', NULL);

    while (1) {
        
        uart0_tx('A', NULL);

    }

    return 0;
}

