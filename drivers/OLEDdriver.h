#include <stdint.h>
#include <stdbool.h>



void OLED_init (void);

void OLED_transmit (char data, bool command);

void OLED_goto_line (uint8_t page);

void OLED_goto_column (uint8_t column);

void OLED_goto_address (uint8_t page, uint8_t column);

void OLED_draw_char(uint8_t page, uint8_t column, char c);
