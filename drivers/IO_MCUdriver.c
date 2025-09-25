#include <IO_MCUdriver.h>
#include <SPIdriver.h>

void IO_MCU_transmit (char data, bool command) {

    // If we want to send command, D/!C line needs to be low, else high
    if (!command) {

        PORTD |= (1 << D_notC);

    } else {

        PORTD &= ~(1 << D_notC);
    }

    SPI_transmit(data);
}