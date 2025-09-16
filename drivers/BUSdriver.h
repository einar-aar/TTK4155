#include <stdint.h>
#include <avr/io.h>
#define SRAM_BASE 0x1800
#define SRAM ((volatile uint8_t*)SRAM_BASE)

uint16_t bitmask (uint16_t bit, uint16_t bitmask);
void BUS_send (uint16_t address, uint8_t data);
uint8_t BUS_read (uint16_t address);
