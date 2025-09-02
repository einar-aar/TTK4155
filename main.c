#include <avr/io.h>
#include <UART_driver.h>


// ATmega162 cpu frequency
#define F_CPU 4915200UL
#include <util/delay.h>


int main(void) {

    DDRA = (1 << PA0);

    for (int i = 0; i < 10; i++) {

        PORTA ^= (1 << PA0);
        _delay_ms(1000);

    }
    return 0;
}

