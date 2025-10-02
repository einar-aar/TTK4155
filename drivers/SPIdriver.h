#include <avr/io.h>

#define DDR_SPI DDRB
#define DD_SS PB4
#define DD_MOSI PB5
#define DD_SCK PB7


#define D_notC PD2
#define SS_IO_MCU PD3
#define SS_OLED PD4
#define Joystick_btn PB3
#define SS_CAN PB4

#define OLED 1
#define IO_MCU 2
#define CAN 3

void SPI_init (void);

char SPI_transfer (char data);

char SPI_receive (void);

void SPI_slaveselect (int slave);

void SPI_release_slave (void);

void oled_print_char(char c);