#include <avr/io.h>
<<<<<<< HEAD
#include <UART_driver.h>
=======
>>>>>>> 0800f27ac69368cd55f1a14734e082e1b305596a


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
<<<<<<< HEAD

=======
>>>>>>> 0800f27ac69368cd55f1a14734e082e1b305596a
