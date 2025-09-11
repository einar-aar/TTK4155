
#include "UARTdriver.h"
#include <avr/io.h>


// ATmega162 cpu frequency
#define F_CPU 4915200UL
#include <util/delay.h>


int main(void) {

    DDRA = (0b00000001 << PA0); // Sets pin PA0 as output, rest on PA as input

    PORTA = (0b00000001 << PA1); // Sets pin PA1 as input with pullup, rest as floating

    if (uart0_init(115200)) {

        printk("Error initializing uart0");
    }

    for (int i = 0; i < 10; i++) {

        uart0_tx('A');
        char res = uart0_rx();
        printk("%c", res);

    }

    return 0;
}