#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "sam.h"
#include "drivers/uart.h"
#include "drivers/can_controller.h"
#include "drivers/can_interrupt.h"
#include "drivers/PWM.h"
#include "drivers/adc.h"
#include "drivers/solenoid.h"

#define baud 9600
#define F_CPU 84000000 // 84 MHz
/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

int goals = 0;

int main()

{
    SystemInit();

    uart_init(F_CPU, baud);
    printf("Hello World\n\r");

    uint32_t can_br = CAN_BR_BRP(20) | CAN_BR_PROPAG(0) | CAN_BR_PHASE1(4) | CAN_BR_PHASE2(2) | CAN_BR_SJW(1);

    can_init_def_tx_rx_mb(can_br);

    printf("CAN initialized\n\r");

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer


    // 1. Aktiver klokke til PIOB
    PMC->PMC_PCER0 |= (1 << ID_PIOB);

    PIOB->PIO_PER = (1u << 27);
    PIOB->PIO_OER = (1u << 27);

    PWMinit(F_CPU);

    ADC_init();
    solenoid_init();

    /*// Test CAN
    CAN_MESSAGE msg;

    msg.id = (uint32_t)0b00000001;
    msg.data_length = 4;

    msg.data[0] = 1;
    msg.data[1] = 3;
    msg.data[2] = 5;
    msg.data[3] = 7;

    // can_send(&msg, 0);
    // printf("Can message sent\n\r");
    */
    
    //CAN_MESSAGE msg_test;

    //set_duty_cycle(&msg_test, F_CPU);

    int* ADC_values = malloc(sizeof(int)*5);
    memset(ADC_values, 0, sizeof(int)*5);

    int IR_value = 10;
    int old_x_value = 0;

    while (1)
    {
        /*
        PIOB->PIO_SODR = (1u << 27);
        for (volatile int i = 0; i < 1000000; i++);

        // LED av
        PIOB->PIO_CODR = (1u << 27);
        for (volatile int i = 0; i < 1000000; i++);*/
        for (volatile int i = 0; i < 100000; i++);
        
        CAN_MESSAGE msg_rx;
        can_receive(&msg_rx, 0);

        //set_duty_cycle(&msg_rx, F_CPU);

        //printf("Data received: %d %d %d %d\n\r", msg_rx.data[0], msg_rx.data[1], msg_rx.data[2], msg_rx.data[3]);

        ADC_values = scale_result(&msg_rx);
        //printf("Scaled data received: %d %d %d %d %d\n\r", ADC_values[0], ADC_values[1], ADC_values[2], ADC_values[3], ADC_values[4]);

        if (ADC_values[0] >= old_x_value - 1 && ADC_values[0] <= old_x_value + 1);
        else set_duty_cycle(ADC_values[0], F_CPU);
        
        old_x_value = ADC_values[0];
        
        /*IR_value = ADC_read();
        printf("IR value: %d\n\r", IR_value);*/

        if (score()) {
            
            goals++;
            printf("Goals: %d\n\r", goals);
        }

        if (ADC_values[4] == 1) {
            
            printf("Solenoid activated\n\r");
            solenoid_activate();
        }

        if (ADC_values[4] == 0) solenoid_deactivate();

        fflush(stdout);
        free(ADC_values);
    }
}