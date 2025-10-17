#include "CANcomdriver.h"
#include "CANcontdriver.h"
#include "SPIdriver.h"
#include "mcp2515.h"
#include <stdio.h>

#define TQ 0.00000025

void CAN_controller_init() {
    
    CAN_controller_reset(); // reset CAN controller (sets to config mode automatically)

    _delay_ms(3);

    uint8_t stat = CAN_read(0x0E);
    printf("CANSTAT: 0x%02X\n\r", stat);

    // Configrue bit_timing

    // Bit rate = 500 kbits/sek (maks 1 Mbit/s)
    // TQ per bit = 8
    // TQ time = 1 / bitrate / TQ per bit

    // Fosc = 16 MHz
    // BRP = TQ time * Fosc / 2 - 1 = 1

    // BTLMODE: 0 -> the length of PS2 is set to max(PS1, 2*TQ), 1 -> the length of PS2 is explicitly set in CNF3

    // BIT timing constraints:
    // PropSeg + PS1 >= PS2
    // PropSeg + PS1 >= Tdelay, where Tdelay is typically 1-2 TQ
    // PS2 > SJW (We set SJW to 1, assuming low input delay)

    // Recommended to place sampling point at 60-70% of bit time
    // We chose 75%:
    // 1 TQ for Sync Segment
    // 1 TQ for Propagation Segment
    // 4 TQ for PS1
    // Leaving 8 - 6 = 2 TQ for PS2

    // setting SJW bits to 0 --> synch jump width length = 1 x TQ

    _delay_us(40);

    uint8_t BRP = 0b00000001;

    uint8_t cnf1_val = SJW1 | BRP; 
    CAN_write(cnf1_val, MCP_CNF1); //configurating CNF1 register

    uint8_t cnf2_val = BTLMODE | SAMPLE_1X | 0b00011000 | 0b00000000 ; //BTLMODE = 1, SAM = 0, PHseg1 = 4 (011 + 1), PRSEG = 1 (000 + 1)
    CAN_write(cnf2_val, MCP_CNF2);

    uint8_t cnf3_val = SOF_DISABLE | WAKFIL_DISABLE | 0b00000001; //PHSEG2 = 2 (001 + 1), assuming CANCTRL = 0 -> SOF = dont care, WAKFIL = 0 ( wake up filter diabled)
    CAN_write(cnf3_val, MCP_CNF3);


    // Configure Receive buffers 0 and 1

    // Bit 6 and 5 is set to 11 to accept all messages
    // Bit 2 is set to 0 to disable buffer rollover from buffer 0 to buffer 1
    // Bit 7 & 4 are not implemented
    // Bit 3, 1 and 0 are read only. 
    uint8_t rxb0ctrl_val = 0b01100000;
    CAN_write(rxb0ctrl_val, MCP_RXB0CTRL);

    // For receive buffer 1, bit 2 is also read only
    uint8_t rxb1ctrl_val = 0b01100000;
    CAN_write(rxb1ctrl_val, MCP_RXB1CTRL);

    //Enable interrupts
    uint8_t caninte_val = 0b00000011; //interrupting when message recieved in buffers (RX1IE = 1, RX0IE = 1), other interrupts disabled
    CAN_write(caninte_val, MCP_CANINTE);

    // Set Mode = Loopback
    uint8_t canctrl_val = MODE_NORMAL/*MODE_LOOPBACK*/; // 3 MSB defines the mode (010 = loopback, 000 = normal, 100 = configuration)
    CAN_write(canctrl_val, MCP_CANCTRL);
}


void CAN_transmit_message(CAN_FRAME frame, int buffer) {

    uint8_t ID_high = (uint8_t)(frame.id >> 3); // 8 MSB of id. Casting gives us LSB of uint32_t
    uint8_t ID_low = (uint8_t)(frame.id) << 5; // 3 LSB of id, put in 3 MSB of id low register

    uint8_t DLC = (frame.dlc & 0b00001111);

    // Use buffer 1 if said so, else use buffer 0 by default
    if (buffer == 1) {

        CAN_write(ID_high, MCP_TXB1SIDH);
        CAN_write(ID_low, MCP_TXB1SIDL);
        CAN_write(DLC, MCP_TXB1DLC);

        for (int i = 0; i < frame.dlc; i++) CAN_write(frame.data[i], MCP_TXB1D0 + i);

        //CAN_write(MCP_RTS_TX1, MCP_TXB1CTRL);
        //CAN_request_to_send(1);
        SPI_slaveselect(CAN);
        SPI_transfer(MCP_RTS_TX1);
        SPI_release_slave();

        printf("Wrote to b1\n\r");
    
    } else if (buffer == 0) {

        CAN_write(ID_high, MCP_TXB0SIDH);
        CAN_write(ID_low, MCP_TXB0SIDL);
        CAN_write(DLC, MCP_TXB0DLC);

        for (int i = 0; i < frame.dlc; i++) CAN_write(frame.data[i], MCP_TXB0D0 + i);

        //CAN_write(MCP_RTS_TX0, MCP_TXB0CTRL);
        //CAN_request_to_send(0);
        SPI_slaveselect(CAN);
        SPI_transfer(MCP_RTS_TX0);
        SPI_release_slave();

        printf("Wrote to b0\n\r");
    }
}

void CAN_receive_message(CAN_FRAME *frame) {

    //declearing addresses
    uint8_t data_bytes[8]; //storage array for the 8 bytes that are read directly from the MCP2515
    uint8_t sidhAddr;
    uint8_t sidlAddr;
    uint8_t dlcAddr;
    uint8_t dataAddr;

    
    printf("in receive func\n\r");

    uint8_t intf = 0; //leser frainterrupt registeret

    while(!(intf & 0b00000011)) {
        intf = CAN_read(MCP_CANINTF);
        //if((MCP_CANINTF & 0b00000001)==0b00000001) break;
        //if((MCP_CANINTF & 0b00000010)==0b00000010) break;
        //if(MCP_CANINTF == 0b00000010) break;
    }


    int buffer = 2;

    //choosing registeraddresses based on buffer 
    if((intf & 0b00000001)==0b00000001) {
        buffer = 0;
        printf("interupt b0 raised\n\r");
    } else if ((intf & 0b00000010)==0b00000010){
        buffer = 1;
        printf("interupt b1 raised\n\r");
    }

    if (buffer == 1) { //buffer = RXB1

        sidhAddr = MCP_RXB1SIDH;
        sidlAddr = MCP_RXB1SIDL;
        dlcAddr = MCP_RXB1DLC;
        dataAddr = MCP_RXB1D0;
        printf("fetching b1 data\n\r");

    } else if (buffer ==0) { //buffer = RXB0

        sidhAddr = MCP_RXB0SIDH;
        sidlAddr = MCP_RXB0SIDL;
        dlcAddr = MCP_RXB0DLC;
        dataAddr = MCP_RXB0D0;
        printf("fetching b0 data\n\r");

    }

    //calculating 11-bit CAN ID
    // ID[10:3] = SIDH[7:0]
    // ID[2:0] = SIDL[7:5]

    //reading ID-registers
    uint32_t ID_high = ((uint32_t)CAN_read(sidhAddr)) << 3; // 8 MSB of id. Casting gives us LSB of uint32_t
    uint32_t ID_low = (uint32_t)(CAN_read(sidlAddr)) >> 5; // 3 LSB of id, put in 3 MSB of id low register

    frame -> id = ID_high | ID_low; //CAN Id calculated

    //reading DLC registers
    frame -> dlc = CAN_read(dlcAddr) & 0b00001111; // masking all bits but the 4 lowest

    //reading data and saving to frame

    uint8_t i = 0;

    while (i < frame -> dlc) {

        data_bytes[i] = CAN_read(dataAddr +i);
        frame -> data[i] = data_bytes[i];
        i++;
    }

    //reset relevant CANINTF flag
    uint8_t mask;
    if(buffer == 1) {
        mask = 0b00000010; 
    }
    else {
        mask = 0b00000001;
    }

    CAN_bit_modify(MCP_CANINTF, mask, 0b00000000);
}