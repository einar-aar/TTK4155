#include <OLEDdriver.h>
#include <SPIdriver.h>

void OLED_init (void) {

    write_c(0xae); // display off
    write_c(0xa1); // Segment remap
    write_c(0xda); // Common pads hardware: alternative
    write_c(0x12);
    write_c(0xc8); // Common output scan direction com63-com0
    write_c(0xa8); // multiplex ratio mode 63
    write_c(0x3f);
    write_c(0xd5); // display divide ratio
    write_c(0x80);
    write_c(0x81); // constrast control
    write_c(0x50);
    write_c(0xd9); // set pre-change period
    write_c(0x21);
    write_c(0x20); // set memory addressing mode
    write_c(0x02);
    write_c(0xdb); // vcom deselect level mode
    write_c(0x30);
    write_c(0xad); // master conf
    write_c(0x00);
    write_c(0xa4); // out follows RAM content
    write_c(0xa6); // set normal display
    write_c(0xaf); // display on
}