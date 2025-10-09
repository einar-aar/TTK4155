#include "CANcontdriver.h"
#include "SPIdriver.h"
#include <util/delay.h>

char CAN_read(char address) {

    SPI_slaveselect(CAN); //pulling CS_inv low to start reading (choosing CAN slave)
    SPI_transfer(CAN_READ); //sending READ-instr
    SPI_transfer(address); //sending address

    _delay_us(40);

    char data = SPI_transfer(0x00); //sendiing dummy value  to read value shifted out from SO pin

    _delay_us(2);
    
    SPI_release_slave(); //terminating read operation by raising CS_inv pin (by releasing all slaves)

    return data; //returning data

}


void CAN_controller_reset() {

    SPI_release_slave();

    SPI_slaveselect(CAN); //pulling CS_inv low 
    SPI_transfer(CAN_RESET); //sending instruction byte

    _delay_us(2);

    SPI_release_slave(); //raising CS_inv
}

void CAN_write(char data, char address) {

    SPI_slaveselect(CAN); //pulling CS_inv low 
    SPI_transfer(CAN_WRITE); //sending write instruction
    SPI_transfer(address); //sending the address the data should be written to
    SPI_transfer(data); //sending data

    _delay_us(2); //delaying to ensure that all bits are loaded before terminating

    SPI_release_slave(); //raising CS_inv when data is sent

}


void CAN_request_to_send(char tx_buffers) {
    //tx_buffers: bit0 = TXB0, bit1 = TXB1, bit2 = TXB2

    SPI_slaveselect(CAN); //pulling CS_inv low 
    
    char masked_buffers = tx_buffers & 0b00000111; //masking to ensure that only buffer bits are let through
    char rts_instr = 0b10000000 | masked_buffers; //base_optcode + buffer bits

    SPI_transfer(rts_instr); //sending request to send instr

    _delay_us(2);

    SPI_release_slave(); //raising CS_inv

}


char CAN_read_status(void) {

    SPI_slaveselect(CAN); //pulling CS_inv low 
    SPI_transfer(CAN_READ_STATUS); //sending read status instr

    _delay_us(40);
    
    char status = SPI_transfer(0x00); //sendiing dummy value  to read value shifted out from SO pin 
    
    _delay_us(2);

    SPI_release_slave(); //terminating read operation by raising CS_inv pin (by releasing all slaves)

    return status; //returning read status

}

void CAN_bit_modify(char address, char data, char mask_byte) {

    SPI_slaveselect(CAN); //pulling CS_inv low
    SPI_transfer(CAN_BITMOD); //sending bitmod instr

    SPI_transfer(address); //sending address of register
    SPI_transfer(mask_byte); //sending mask byte containing values in address register that is allowed to be changed
    SPI_transfer(data); //sending datavalue of modified bits

    _delay_us(2);

    SPI_release_slave(); //raising CS_inv
}