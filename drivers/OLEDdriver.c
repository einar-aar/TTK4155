#include "OLEDdriver.h"
#include "SPIdriver.h"
#include "fonts.h"
#include <avr/pgmspace.h>
#include <string.h>


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

    OLED_transmit(0xAE, true);      // Display OFF

    // — Anbefalt grunnoppsett —
    OLED_transmit(0x20, true);      // Set Memory Addressing Mode
    OLED_transmit(0x00, true);      //   0x00 = Horizontal addressing mode

    OLED_transmit(0x21, true);      // Set Column Address
    OLED_transmit(0x00, true);      //   start = 0
    OLED_transmit(0x7F, true);      //   end   = 127

    OLED_transmit(0x22, true);      // Set Page Address
    OLED_transmit(0x00, true);      //   start = 0
    OLED_transmit(0x07, true);      //   end   = 7

    OLED_transmit(0xA0, true);      // Segment remap: normal (SEG0->col0)
    OLED_transmit(0xC0, true);      // COM scan direction: normal (COM0->row0)
    OLED_transmit(0xD3, true);      // Display offset
    OLED_transmit(0x00, true);      //   0
    OLED_transmit(0x40, true);      // Set display start line = 0

    OLED_transmit(0xA6, true);      // Normal display (ikke invers)
    OLED_transmit(0xA4, true);      // Output følger RAM (ikke "entire display ON")

    // (Valgfritt: timing/pins avhengig av panel)
    // OLED_transmit(0xD5, true); OLED_transmit(0x80, true); // clock div/osc
    // OLED_transmit(0xA8, true); OLED_transmit(0x3F, true); // multiplex 1/64
    // OLED_transmit(0xDA, true); OLED_transmit(0x12, true); // COM pins cfg

    // Clear én gang (nå peker den horisontalt og auto-inkrementerer kolonne)
    for (uint8_t page = 0; page < 8; page++) {
        OLED_transmit(0x22, true); OLED_transmit(page, true); OLED_transmit(page, true);
        OLED_transmit(0x21, true); OLED_transmit(0, true);    OLED_transmit(127, true);
        for (uint8_t col = 0; col < 128; col++) {
            OLED_transmit(0x00, false);
        }
    }

    OLED_transmit(0xAF, true); 
}

// Go to line
void OLED_goto_line (uint8_t page) {

    // Mask to make sure we are defining page with 3 bits, since we have 64 pixels height / 8 pixels per page = 8 pages
    // Mask to make sure we are defining page with 3 bits, since we have 64 pixels height / 8 pixels per page = 8 pages
    page &= 0b00000111;

    // 5 msb is saying we want to get a page, 3 lsb define what page
    OLED_transmit(0b10110000 | page, true);
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

// Clear whole page
void OLED_clear_page (uint8_t page) {

    for (int i = 0; i < 127; i++) {

        OLED_goto_address(page, i);
        OLED_transmit(0, false);
    }
}

//using ASCII 32-127 fonr (5x7)
void OLED_draw_char(uint8_t page, uint8_t column, char c, char font) {

    if ( c<32 || c>127) {
        c='?';
    }

    // setting position for char
    OLED_goto_address(page,column);

    OLED_transmit(0, false);

    uint8_t byte;
    int iterations;

    if (font == 's') iterations = 4;
    else if (font == 'l') iterations = 8;
    else iterations = 5;

    for ( int i = 0; i < iterations; i++) { // writing each column in the char to the display

        if (font == 's') byte = pgm_read_byte(&font4[c - 32][i]);
        else if (font == 'l') byte = pgm_read_byte(&font8[c - 32][i]);
        else byte = pgm_read_byte(&font5[c - 32][i]);
        

        OLED_goto_address(page, column + i + 1);
        OLED_transmit(byte, false);
    }

    OLED_transmit(0x00, false); // adding a free column after the char
}

void OLED_draw_string(uint8_t page, uint8_t column, char s[], char font) {

    int column_number;

    if (font == 's') column_number = 4;
    else if (font == 'l') column_number = 8;
    else column_number = 5;

    for (int i = 0; i < strlen(s); i++) {

        if (i * (column_number + 1) > 125) {
            
            OLED_draw_char(page + 1, column + i*(column_number + 1), s[i], font);
        
        } else if (i * (column_number + 1) > 250) { 

            OLED_draw_char(page, column + i*(column_number + 1), s[i], font);

        } else {

            OLED_draw_char(page + 1, column + i*(column_number + 1), s[i], font);
        }
    }
}
