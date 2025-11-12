
#include <avr/io.h>
#include "drivers/UARTdriver.h"
#include "drivers/XMEM.h"
#include "drivers/ADCdriver.h"
#include "drivers/SPIdriver.h"
#include "drivers/OLEDdriver.h"
#include "drivers/fonts.h"
#include "drivers/CANcontdriver.h"
#include "drivers/CANcomdriver.h"
#include "drivers/JOYSTICKdriver.h"

// ATmega162 cpu frequency
#define F_CPU 4915200UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <util/delay.h>
#include "sram_test.h"
#include <avr/pgmspace.h>
#include <avr/pgmspace.h>


int main(void) {

    uart0_init(MYUBRR);
    printf("Test\n");

    // initialize ADC
    if (init_clock() != 0) {

        printf("Failed to initialize ADC");
    }

    // Initialize external memory
    if (external_memory_init() != 0) {

        printf("Failed to initialize external memory");
    }

    
    SRAM_test();

    SPI_init();
    OLED_init();
    CAN_controller_init();

    _delay_ms(1000);
   
    uint8_t stat = CAN_read(0x0E);
    printf("CANSTAT: 0x%02X\n\r", stat);

    // GAME FUNCTIONALITY
    bool main_menu = true;
    OLED_main_menu();

    int* ADC_menu_values = malloc(sizeof(int)*5);
    memset(ADC_menu_values, 0, sizeof(int)*5);

    CAN_FRAME msg_send;
    msg_send.id = (uint32_t)1;
    msg_send.dlc = 2;
    msg_send.data[0] = 0;
    msg_send.data[1] = 0;
    CAN_transmit_message(msg_send, 0);

    while (main_menu) {

        ADC_menu_values = ADC_read_joystick_and_pad();
        ADC_menu_values[4] = read_joystick_button();

        if (ADC_menu_values[1] >= 200) {

            OLED_main_menu_navigate('u');
            _delay_ms(200);
            
        } else if (ADC_menu_values[1] <= 100) {

            OLED_main_menu_navigate('d');
            _delay_ms(200);
        }

        if (ADC_menu_values[4] == 1 && main_menu_position == 2) main_menu = false;

        _delay_ms(10);
    }

    free(ADC_menu_values);

    OLED_clear_screen();
    OLED_draw_string(2, 4, "Game starting in: 3", 'm');
    _delay_ms(1000);
    OLED_clear_screen();
    OLED_draw_string(2, 4, "Game starting in: 2", 'm');
    _delay_ms(1000);
    OLED_clear_screen();
    OLED_draw_string(2, 4, "Game starting in: 1", 'm');
    _delay_ms(1000);
    OLED_clear_screen();

    bool running = true;
    int time = 0;
    char score[5];
    CAN_FRAME* msg;
    OLED_draw_string(2, 4, "Game running", 'm');

    while (running) {

        sendJoystickPos();

        _delay_ms(20); // Potential starvation risk with too high frequency. Could solve this by making node 2 message higher priority

        CAN_receive(msg);

        // End game if ball block sensor
        if (msg->data[1] == 14) {

            printf("MSG received: Game over\n\r");
            running = false;
        }
        printf("Data from node 2: %d", msg->data[1]);
        
    }

    // Update score
    time = msg->data[0];
    itoa(time, score, 10);

    OLED_clear_screen();
    OLED_draw_string(2, 4, "GAME OVER", 'l');
    OLED_draw_string(3, 4, "Score:", 'm');
    OLED_draw_string(4, 20, score, 'm');
    
    return 0;
}

