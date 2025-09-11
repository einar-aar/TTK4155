#include "UARTdriver.h"
#include <avr/io.h>


#define F_CPU 4915200UL // crystal oscillator


int uart0_init (unsigned int ubrr) {

    // int ubrr = F_CPU / (16 * baud) - 1;

    UBRR0L = (unsigned char) (ubrr); //saves the first 8 bits from UBRR
    UBRR0H = (unsigned char) (ubrr >> 8); // saves the last 8 bits from UBBR by pushing them to a lower bit position
    
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    
    UCSR0C = (1 << URSEL0) | (1 << USBS0) | (3 << UCSZ00); // UCSRC is the adress containing info about data format
                                                      // URSEL is a macro saying you choose UCSRC
                                                      // USBS is number of stop bits (0 is 1, 1 is 2)
                                                      // UCSZ0 is number of databits (uses 2 bits, 3 gives 11 translating to 8 databits)
    

    fdevopen(uart0_tx, uart0_rx);

    return 0; // Success
}


int uart0_rx (FILE* filestream) {

    while (! (UCSR0A & (1<<RXC0))); // waiting for data to be present in receive buffer

    return UDR0; //get and retrn data from buffer

}


int uart0_tx ( char byte, FILE* filestream) {

    while (!(UCSR0A & (1<<UDRE0))); //waiting for the sending buffer UDR to be empty. We dont understand where UCSRA is stored

    UDR0 = byte; //stores data into buffer
    return 0;
}

/*
int uart0_tx_cast(unsigned char c, FILE* filestream) { //function used to implement fdevopen() and printf()
    uart0_tx(c);
    return 0; // returns 0 if the char was sucsessfully transmittet
}

int uart0_rx_cast(FILE* filestream) {
    uart0_rx();
    return 0;
}*/