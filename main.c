
#include <avr/io.h>
#include "drivers/UARTdriver.h"
#include "drivers/BUSdriver.h"
#include "drivers/XMEM.h"
#include "drivers/ADCdriver.h"
#include "drivers/SPIdriver.h"
#include "drivers/OLEDdriver.h"
#include "drivers/IO_MCUdriver.h"
#include "drivers/font5x7.h"
#include "drivers/fonts.h"
#include "drivers/fonts.h"

// ATmega162 cpu frequency
#define F_CPU 4915200UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <util/delay.h>
#include "sram_test.h"
#include <avr/pgmspace.h>
#include <avr/pgmspace.h>


int main(void) {

    /*DDRA = (0b00000001 << PA0); // Sets pin PA0 as output, rest on PA as input

    PORTA = (0b00000001 << PA1); // Sets pin PA1 as input with pullup, PA0 logic low*/
    uart0_init(MYUBRR);
    printf("Test\n");
    
    /*
    while (1) {

int external_memory_init(void) {

    // Set the external memory bit in the MCU control register
    MCUCR |= (1 << SRE);

    // Enable masking of highest 4 bits (PC7-PC4) in the Special function IO register
    SFIOR |= (1 << XMM2);

    return 0; // Success
}

        uart0_tx('A', NULL);

    }*/

    /*DDRA |= (0b00000001 << PA0) | (0b00000001 << PA1) | (0b00000001 << PA2) | (0b00000001 << PA3)
            | (0b00000001 << PA4) | (0b00000001 << PA5) | (0b00000001 << PA6) | (0b00000001 << PA7);

    DDRE |= (0b00000001 << PE1);

    PORTE = (0b00000001 << PE1);*/





    /*while (1) {

        PORTA = 0b11111111;
        _delay_ms(1000);
        PORTA = 0b00000000;
        _delay_ms(1000);

    }*/

    // initialize ADC
    if (init_clock() != 0) {

        printf("Failed to initialize ADC");
    }

    // Initialize external memory
    if (external_memory_init() != 0) {

        printf("Failed to initialize external memory");
    }

    SRAM_test();

    /*ADC[0]=0x04;
    uint8_t value=ADC[0];
    printf("ADC-verdi: %02X\n\n\r", value);

    _delay_ms(50);*/

    /*int* ADC_values = malloc(sizeof(int)*4);
    memset(ADC_values, 0, sizeof(int)*4);

    while (1) {
        
        ADC_values = ADC_read();
        printf("Joystick_x: %d, joystick_y: %d, touchpad_x: %d, touchpad_y: %d\n\r", ADC_values[1], ADC_values[0], ADC_values[2], ADC_values[3]);
        free(ADC_values);
    }*/

    SPI_init();
    OLED_init();
    
    // Write all pixels
    /*
    for (int i = 0; i < 8; i++) {
        
        for (int j = 0; j < 128; j++) {

            OLED_goto_address(i, j);
            OLED_transmit(255, false);
        }
    }
    */
    /*
    // Clear all pixels
    */
    /*
    // Clear all pixels
    for (int i = 0; i < 8; i++) {
        
        
        for (int j = 0; j < 128; j++) {

            OLED_goto_address(i, j);
            OLED_transmit(0, false);
        }
    }*/

    /*
    OLED_draw_char(0, 20, 'H');
    OLED_draw_char(0, 26, 'E');
    OLED_draw_char(0, 32, 'I');*/

    //OLED_goto_address(5, 20);
    //OLED_transmit(255, false);

    /*
    for (int i = 0; i < 8; i++) {
        
        for (int j = 0; j < 128; j++) {

            OLED_goto_address(i, j);
            OLED_transmit(255, false);
        }
    }*/

    // OLED_draw_string(0, 0, "Hei, jeg heter Anders og liker fisk", 'm');

    OLED_main_menu();

    int* ADC_values = malloc(sizeof(int)*4);
    memset(ADC_values, 0, sizeof(int)*4);

    uint8_t right_buttons[6], left_buttons[7], navigation_buttons[5];

    while (1) {
        
        ADC_read_IO_module(right_buttons, left_buttons, navigation_buttons);

        if (right_buttons[4] == 1) {

            OLED_clear_page(main_menu_position);
        }

        ADC_values = ADC_read_joystick_and_pad();
        
        if (ADC_values[0] >= 50) {

            OLED_main_menu_navigate('u');
            _delay_ms(200);
            
        } else if (ADC_values[0] <= -50) {

            OLED_main_menu_navigate('d');
            _delay_ms(200);
        }


        free(ADC_values);
    }

    //OLED_goto_address(2, 20);

    /*
    uint16_t message_sent = 80;

    SRAM_send(0x0200, message_sent);

    uint8_t SRAM_message_received = SRAM_read(0x0200);

    uint8_t ADC_message_received;
    while (1) {
        ADC_message_received = ADC_read(0x0100);
        printf("%d\n\r", ADC_message_received);
    }
    printf("Message sent: %d\n\rMessage received: %d\n\rADC message received: %d\n\r", message_sent, SRAM_message_received, ADC_message_received);
    */
    return 0;
}

