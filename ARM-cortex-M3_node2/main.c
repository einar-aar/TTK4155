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
#include "drivers/motor_controller.h"
#include "drivers/time.h"
#include "drivers/PI.h"
#include "drivers/game_func.h"

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
    encoder_init();

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
    int old_y_value = 0;

    int start_time = (int)totalMsecs(time_now());
    int last_PI_time = 0;
    int time;
    int last_time_print = 0;

    bool running = true;
    bool game_over = false;

    CAN_MESSAGE msg_tx;
    msg_tx.id = 1;
    msg_tx.data_length = 2;
    
    while (running)
    {
        time = (int)totalMsecs(time_now()) - start_time;
        /*
        PIOB->PIO_SODR = (1u << 27);
        for (volatile int i = 0; i < 1000000; i++);

        // LED av
        PIOB->PIO_CODR = (1u << 27);
        for (volatile int i = 0; i < 1000000; i++);*/
        
        //for (volatile int i = 0; i < 100000; i++);
        
        
        CAN_MESSAGE msg_rx;
        can_receive(&msg_rx, 0);

        //set_duty_cycle(&msg_rx, F_CPU);

        //printf("Data received: %d %d %d %d %d\n\r", msg_rx.data[0], msg_rx.data[1], msg_rx.data[2], msg_rx.data[3], msg_rx.data[4]);

        ADC_values = scale_result(&msg_rx);
        //printf("Scaled data received: %d %d %d %d %d\n\r", ADC_values[0], ADC_values[1], ADC_values[2], ADC_values[3], ADC_values[4]);

        if (ADC_values[1] >= old_y_value + 3 || ADC_values[1] <= old_y_value - 3) set_duty_cycle(ADC_values[1], F_CPU, 1);
        old_y_value = ADC_values[1];

        /*if (ADC_values[0] >= old_x_value + 3 || ADC_values[0] <= old_x_value - 3) set_motor_pos(ADC_values[0]);
        old_x_value = ADC_values[0];*/
        
        /*IR_value = ADC_read();
        printf("IR value: %d\n\r", IR_value);*/

        if (time - last_PI_time >= 10) {

            control_loop_tick(ADC_values[0]);

            last_PI_time = time;
        }
        
        if (score()) {

            bool goal = true;
            register_goal = true;
            
            for (int i = 0; i < 5; i++) {

                register_goal = true;
                if (!score()) goal = false;

            }

            if (goal) {

                goals++;
                printf("Goals: %d\n\r", goals);
                register_goal = false;
            }
            register_goal = false;
        }

        if (ADC_values[4] == 1) solenoid_activate();

        if (ADC_values[4] == 0) solenoid_deactivate();

        // printf("Value: %d\n\r", TC2 -> TC_CHANNEL[0].TC_CV);
        //printf("Value: %d\n\r", get_encoder_pos());
        
        /*
        if (time - last_time_print > 1000) {
            
            printf("Time in seconds: %d\n\r", time / 1000);
            last_time_print = time;
        }
        */

        // can_send makes control janky, maybe it takes alot of time
        // can_send(&msg_tx, 0);
        int A = ADC_read();
        //if (A < 1100) printf("IR value: %d\n\r", A);
        
        if (goals > 0) {

            printf("Game over\n\r");
            set_enable_pwm_duty_ratio(0.00f);
            running = false;
            game_over = true;
        }

        if (game_over) {

            set_enable_pwm_duty_ratio(0.00f);

            

            msg_tx.data[0] = time;
            msg_tx.data[1] = 1;
            can_send(&msg_tx, 1);
            game_over = false;

            while (1) {

                set_enable_pwm_duty_ratio(0.0f);
            }
        }

        fflush(stdout);
        free(ADC_values);
    }
}