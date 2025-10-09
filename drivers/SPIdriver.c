#include "SPIdriver.h"

// Initialize SPI
void SPI_init (void) {

    // Activate MOSI and Serial clock and set Slave select as output
    // MISO is set as input when MISO-bit is 0
    // set pin PB4 as slave select for CAN
    // Set pin PD2 as D/!C, pin PD3 as Slave select for I/O MCU and pin PD4 as Slave select for OLED
    DDR_SPI |= (1 << DD_MOSI) | (1 << DD_SCK) | (1 << SS_CAN);  //obs: initializing slave select for CAN at the sane time as SPI function because it is also at port B
    DDRD |= (1 << D_notC) | (1 << SS_IO_MCU) | (1 << SS_OLED);

    // Activate input with internal pullup on joystick button read pin
    DDRB  &= ~(1 << Joystick_btn); 
    PORTB |= (1 << Joystick_btn);

    // Activate Atmega162 as master, activate SPI, regulate SPI clock frequency to f_osc / 16
    SPCR = (1 << MSTR) | (1 << SPE) | (1 << SPR0);

    // Set Slave select logical low, physical high by default 
    PORTD |= (1 << SS_OLED) | (1 << SS_IO_MCU);
    PORTB |= (1 << SS_CAN);
}

// Transmit or receive over SPI
char SPI_transfer (char data) {

    // Fill data register
    SPDR = data;

    // Wait until data is transmitted
    while (!((1 << SPIF) & SPSR));

    return SPDR;
}

// Slave select function
void SPI_slaveselect (int slave) {

    // Set Slave select logical high to select OLED
    if (slave == OLED) {
        
        PORTD &= ~(1 << SS_OLED);
        PORTD |= (1 << SS_IO_MCU);
        PORTB |= (1 << SS_CAN);

    } else if (slave == IO_MCU) {
        
        PORTD |= (1 << SS_OLED);
        PORTD &= ~(1 << SS_IO_MCU);
        PORTB |= (1 << SS_CAN);

    } else if (slave == CAN){

        PORTD |= (1 << SS_OLED);
        PORTD |= (1 << SS_IO_MCU);
        PORTB &= ~(1 << SS_CAN);
    }
}

// Release slave
void SPI_release_slave (void) {

    PORTD |= (1 << SS_OLED) | (1 << SS_IO_MCU);
    PORTB |= (1 << SS_CAN);
}