#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "drivers/uart.h"
#include "drivers/can.h"

#define baud 9600
#define F_CPU 84000000
/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
//#include "../path_to/uart.h"

int main()
{
    SystemInit();

    uart_init(F_CPU, baud);
    printf("Hello World\n\r");

    CanInit init;
    init.reg = CAN_BR_BRP(20) | CAN_BR_PROPAG(0) | CAN_BR_PHASE1(3) | CAN_BR_PHASE2(1) | CAN_BR_SJW(0);

    can_init(init, 0);
    printf("CAN initialized\n\r");

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer


    // 1. Aktiver klokke til PIOB
    PMC->PMC_PCER0 |= (1 << ID_PIOB);

    PIOB->PIO_PER = (1u << 27);
    PIOB->PIO_OER = (1u << 27);

    // Test CAN
    CanMsg rx, tx;

    while (1)
    {
        /*PIOB->PIO_SODR = (1u << 27);
        for (volatile int i = 0; i < 1000000; i++);

        // LED av
        PIOB->PIO_CODR = (1u << 27);
        for (volatile int i = 0; i < 1000000; i++);*/

        // Test can
        if (can_rx(&rx)) {               // mottatt melding?
            can_printmsg(rx);

            tx.id = rx.id;               // ekko med samme ID
            tx.length = rx.length;
            for (uint8_t i = 0; i < rx.length; i++) {
                tx.byte[i] = rx.byte[i];
            }

            can_tx(tx);
        }

    }
    
}