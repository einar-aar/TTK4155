<<<<<<< HEAD
#include "UARTdriver.h"
#include "avr/io.h"
=======
#include <avr/io.h>
<<<<<<< HEAD
#include <UART_driver.h>
=======
>>>>>>> 0800f27ac69368cd55f1a14734e082e1b305596a

>>>>>>> 966fc6ea92d4f504325058f8df097edd100b8a74

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
<<<<<<< HEAD

    return 0;
}
=======
    return 0;
}
<<<<<<< HEAD

=======
>>>>>>> 0800f27ac69368cd55f1a14734e082e1b305596a
>>>>>>> 966fc6ea92d4f504325058f8df097edd100b8a74
