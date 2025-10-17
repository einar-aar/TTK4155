#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "drivers/uart.h"

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

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    uart_init(F_CPU, baud);
    printf("Hello World\n\r");

    // 1. Aktiver klokke til PIOB
    PMC->PMC_PCER0 |= (1 << ID_PIOB);

    PIOB->PIO_PER = (1u << 27);
    PIOB->PIO_OER = (1u << 27);

    while (1)
    {
        PIOB->PIO_SODR = (1u << 27);
        for (volatile int i = 0; i < 1000000; i++);

        // LED av
        PIOB->PIO_CODR = (1u << 27);
        for (volatile int i = 0; i < 1000000; i++);
    }
    
}