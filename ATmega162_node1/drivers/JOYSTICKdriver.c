#include "JOYSTICKdriver.h"
#include "CANcomdriver.h"
#include "ADCdriver.h"
#include <stdio.h>
#include <stdlib.h>


void sendJoystickPos() {

    ADC[0]=0x04;
    uint8_t value = ADC[0];

    _delay_ms(50);

    int* ADC_values = malloc(sizeof(int)*4);
    memset(ADC_values, 0, sizeof(int)*4);
    ADC_values = ADC_read_joystick_and_pad();

    CAN_FRAME msg_send;
    msg_send.id = (uint32_t)0b00000001;
    msg_send.dlc = 4;
    msg_send.data[0] = ADC_values[1];
    msg_send.data[1] = ADC_values[0];
    msg_send.data[2] = ADC_values[2];
    msg_send.data[3] = ADC_values[3];

    CAN_transmit_message(msg_send, 0);
    printf("CAN message sent\n\r");
    
    free(ADC_values);
}
