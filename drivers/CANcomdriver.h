#include <stdint.h>
#include <stdbool.h>

//defining a frame struct
typedef struct  {

    uint32_t id; //message identifier, (11-bits or 29 bits depending on frame (standard/extended))
    uint8_t dlc; //data length code: 0-4
    uint8_t data[8]; //data bytes

}CAN_FRAME;

void CAN_controller_init();

void CAN_transmit_message(CAN_FRAME frame, int buffer);

void CAN_receive_message(CAN_FRAME *frame, int buffer);