#include "game_func.h"
#include "adc.h"



uint8_t score() {

    if (ADC_read() < 500) return 1;
    else return 0;
}