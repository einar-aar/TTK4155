#include "UARTdriver.h"
#include <avr/io.h>

#define F_CPU 4915200UL // crystal oscillator


int uart0_init (unsigned int baud) {

    int ubrr = F_CPU / (16 * baud) - 1;

    UBRRL0 = (unsigned char) (ubrr); //saves the first 8 bits from UBRR
    UBRRH0 = (unsigned char) (ubrr >> 8); // saves the last 8 bits from UBBR by pushing them to a lower bit position
    
    UCSRC0 = (1 << URSEL0) | (1 << USBS0) | (1 << UCSZ0); // UCSRC is the adress containing info about data format
                                                      // URSEL is a macro saying you choose UCSRC
                                                      // USBS is number of stop bits (0 is 1, 1 is 2)
                                                      // UCSZ0 is number of databits (uses 2 bits, 3 gives 11 translating to 8 databits)
    UCSRB0 = (1 << TXEN0) | (1 << RXEN0);

    return 0; // Success
}


char uart0_rx (void) {

    while (! (UCSR0A & (1<<RXC0))); // waiting for data to be present in receive buffer

    return UDR0; //get and retrn data from buffer

}


int uart0_tx (char byte) {

    while (!(UCSR0A & (1<<UDRE0))); //waiting for the sending buffer UDR to be empty. We dont understand where UCSRA is stored

    UDR0 = byte; //stores data into buffer

    return 0; // Success
}