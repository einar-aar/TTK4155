#include <SPIdriver.h>

// Initialize SPI
void SPI_init (void) {

    // Activate MOSI, MISO and Serial clock
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_MISO) || (1 << DD_SCK);

    // Activate Atmega162 as master, activate SPI
    SPCR = (1 << MSTR) | (1 << SPE):
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
void SPI_slaveselect (int slave) {

    
}