#include "OLEDdriver.h"
#include "SPIdriver.h"
#include "fonts.h"
#include <avr/pgmspace.h>
#include <string.h>
#include "fonts.h"
#include <avr/pgmspace.h>
#include <string.h>

volatile int main_menu_position = 0;
volatile int main_menu_position = 0;

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

    OLED_transmit(0xA1, true);      // Segment remap: normal (SEG0->col0)
    OLED_transmit(0xC8, true);      // COM scan direction: normal (COM0->row0)

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

// Clear whole screen
void OLED_clear_screen (void) {

    for (int i = 0; i < 8; i++) {

        OLED_clear_page(i);
    }
}

// Clear whole screen
void OLED_clear_screen (void) {

    for (int i = 0; i < 8; i++) {

        OLED_clear_page(i);
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

            OLED_draw_char(page, column + i*(column_number + 1), s[i], font);
        }
    }
}


// Main menu navigation helper
void make_arrow (uint8_t page) {

    OLED_goto_address(page, 115);
    OLED_transmit(0b00011000, false);

    OLED_goto_address(page, 116);
    OLED_transmit(0b00111100, false);

    OLED_goto_address(page, 117);
    OLED_transmit(0b00111100, false);

    OLED_goto_address(page, 118);
    OLED_transmit(0b01111110, false);

    OLED_goto_address(page, 119);
    OLED_transmit(0b01111110, false);

    OLED_goto_address(page, 120);
    OLED_transmit(0b11111111, false);
}

void clear_arrow (uint8_t page) {

    OLED_goto_address(page, 115);
    OLED_transmit(0, false);

    OLED_goto_address(page, 116);
    OLED_transmit(0, false);

    OLED_goto_address(page, 117);
    OLED_transmit(0, false);

    OLED_goto_address(page, 118);
    OLED_transmit(0, false);

    OLED_goto_address(page, 119);
    OLED_transmit(0, false);

    OLED_goto_address(page, 120);
    OLED_transmit(0, false);
}


// Initialize main menu
void OLED_main_menu (void) {

    OLED_clear_screen();

    OLED_draw_string(0, 3, "Main menu", 'm');
    OLED_draw_string(2, 3, "Start game", 's');
    OLED_draw_string(3, 3, "Highscore", 's');
    OLED_draw_string(4, 3, "Options", 's');
    OLED_draw_string(5, 3, "Placeholder", 's');

    OLED_draw_string(7, 3, "Credits: EAA, TMS, RF", 's');

    main_menu_position = 2;

    make_arrow(main_menu_position);
}


// Navigate main menu
void OLED_main_menu_navigate (char direction) {

    if (main_menu_position == 2) {

        if (direction == 'u') {

            clear_arrow(main_menu_position);
            main_menu_position = 5;
            make_arrow(main_menu_position);

        } else if (direction == 'd') {

            clear_arrow(main_menu_position);
            main_menu_position++;
            make_arrow(main_menu_position);
        }

    } else if (main_menu_position == 5) {

        if (direction == 'u') {

            clear_arrow(main_menu_position);
            main_menu_position--;
            make_arrow(main_menu_position);

        } else if (direction == 'd') {

            clear_arrow(main_menu_position);
            main_menu_position = 2;
            make_arrow(main_menu_position);
        }

    } else if (direction == 'd') {

        clear_arrow(main_menu_position);
        main_menu_position++;
        make_arrow(main_menu_position);
    
    } else if (direction == 'u') {

        clear_arrow(main_menu_position);
        main_menu_position--;
        make_arrow(main_menu_position);
    }
}