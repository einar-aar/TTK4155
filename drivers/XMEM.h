#include <avr/io.h>

#define SRAM_BASE 0x1800
#define SRAM ((volatile uint8_t*)SRAM_BASE)

int external_memory_init(void);

void SRAM_send (uint16_t address, uint8_t data);
uint8_t SRAM_read (uint16_t address);