#include <avr/io.h>

#define DDR_SPI DDRB
#define DD_SS PB4
#define DD_MOSI PB5
#define DD_SCK PB7

#define D_notC PD2
#define SS_IO_MCU PD3
#define SS_OLED PD4

#define OLED 1
#define IO_MCU 2

void SPI_init (void);

void SPI_transmit (char data);

char SPI_receive (void);

void SPI_slaveselect (int slave);

void SPI_release_slave (void);