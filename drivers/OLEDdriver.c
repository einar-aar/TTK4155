#include "OLEDdriver.h"
#include "SPIdriver.h"


// Transmit data or command
void OLED_transmit (char data, bool command) {

    // If we want to send command, D/!C line needs to be low, else high
    if (!command) {

        PORTD |= (1 << D_notC);

    } else {

        PORTD &= ~(1 << D_notC);
    }

    SPI_slaveselect(OLED);

    SPI_transmit(data);

    SPI_release_slave();
}

// Initialize OLED
void OLED_init (void) {

    OLED_transmit(0xae, true); // display off
    OLED_transmit(0xa1, true); // Segment remap
    OLED_transmit(0xda, true); // Common pads hardware: alternative
    OLED_transmit(0x12, true);
    OLED_transmit(0xc8, true); // Common output scan direction com63-com0
    OLED_transmit(0xa8, true); // multiplex ratio mode 63
    OLED_transmit(0x3f, true);
    OLED_transmit(0xd5, true); // display divide ratio
    OLED_transmit(0x80, true);
    OLED_transmit(0x81, true); // constrast control
    OLED_transmit(0x50, true);
    OLED_transmit(0xd9, true); // set pre-change period
    OLED_transmit(0x21, true);
    OLED_transmit(0x20, true); // set memory addressing mode
    OLED_transmit(0x02, true);
    OLED_transmit(0xdb, true); // vcom deselect level mode
    OLED_transmit(0x30, true);
    OLED_transmit(0xad, true); // master conf
    OLED_transmit(0x00, true);
    OLED_transmit(0xa4, true); // out follows RAM content
    OLED_transmit(0xa6, true); // set normal display
    OLED_transmit(0xaf, true); // display on
}

// Go to line
void OLED_goto_line (uint8_t page) {

    // Mask to make sure we are defining page with 3 bits, since we have 64 pixels height / 8 pixels per page = 8 pages
    page &= 0b00000111;

    // 5 msb is saying we want to get a page, 3 lsb define what page
    OLED_transmit(0b10110000 | page, true);
}

// Go to column
void OLED_goto_column (uint8_t column) {

    // Mask to make sure we are defining page with 7 bits, since we have 128 pixel wide screen
    column &= 0b01111111;

    // 4 msb is saying we want to set address, 4 lsb define what address
    // 0000 msb == set lower column start address
    OLED_transmit(0b00000000 | (column & 0b00001111), true);
    // 0001 msb == set higher column start address
    OLED_transmit(0b00010000 | (column >> 4), true);
}

// Go to spesific address
void OLED_goto_address (uint8_t page, uint8_t column) {

    OLED_goto_line(page);
    OLED_goto_column(column);
}

