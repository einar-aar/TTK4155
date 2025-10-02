#include <stdint.h>
#include <stdint.h>
#include <stdbool.h>

extern volatile int main_menu_position;

void OLED_init (void);

void OLED_transmit (char data, bool command);

void OLED_goto_line (uint8_t page);

void OLED_goto_column (uint8_t column);

void OLED_goto_address (uint8_t page, uint8_t column);

void OLED_clear_page (uint8_t page);

void OLED_clear_screen (void);

void OLED_draw_char(uint8_t page, uint8_t column, char c, char font);

void OLED_draw_string(uint8_t page, uint8_t column, char s[], char font);


// Main menu
void make_arrow (uint8_t page);

void clear_arrow (uint8_t page);

void OLED_main_menu (void);

void OLED_main_menu_navigate (char direction);