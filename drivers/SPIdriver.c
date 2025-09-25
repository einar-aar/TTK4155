#include <SPIdriver.h>

// Initialize SPI
void SPI_init (void) {

    // Activate MOSI and Serial clock and set Slave select as output
    // MISO is set as input when MISO-bit is 0
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_SS);

    // Activate Atmega162 as master, activate SPI, regulate SPI clock frequency to f_osc / 16
    SPCR = (1 << MSTR) | (1 << SPE) | (1 << SPR0):

    // Set Slave select physical high, logical low
    PORTB |= (1 << DD_SS);

}

// Transmit over SPI
void SPI_transmit (char data) {

    // Fill data register
    SPDR = data;

    // Wait until data is transmitted
    while (!((1 << SPIF) & SPSR));
}

// Receive over SPI
char SPI_receive (void) {

    // Wait for data
    while(!((1 << SPIF) & SPSR));

    return SPDR;
}

// Slave select function
void SPI_slaveselect (char slave[]) {

    // Set Slave select logical high to select OLED
    if (slave == "OLED") PORTB &= ~(1 << DD_SS);
}