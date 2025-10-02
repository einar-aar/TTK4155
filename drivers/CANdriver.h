#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>


//instructions 
#define CAN_RESET 0b11000000
#define CAN_READ 0b00000011
#define CAN_WRITE 0b00000010
#define CAN_RTS 0b10000000
#define CAN_READ_STATUS 0b10100000
#define CAN_BITMOD 0b00000101


char CAN_read(char address);

void CAN_reset();

void CAN_write(char data, char address);

void CAN_request_to_send(char tx_buffers);

char CAN_read_status(void);

void CAN_bit_modify(char address, char data, char mask_byte);



