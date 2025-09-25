#include <avr/io.h>

#define D_notC PD2
#define SS_IO_MCU PD3
#define SS_OLED PD4

void SPI_init (void);

void SPI_transmit (char data);

char SPI_receive (void);

void SPI_slaveselect (char slave[]);

void SPI_release_slave (void);