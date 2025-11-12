#include "game_func.h"
#include "adc.h"
#include "sam.h"
#include <stdint.h>
#include <time.h>

// uint32_t last_goal_time = 0;
bool register_block = true; // If the IR led is not blocked, we get a value higher than 3000. If blocked, we get around 0 - 50.

uint8_t score() {

    if (ADC_read() > 700 && ADC_read() < 1300 && register_block) {
        
        register_block = false;
        return 1;
    }

    if (ADC_read() > 2500) register_block = true;
    
    return 0;

}
// (time_now() / ticksPerMs() - last_goal_time) > 500
// last_goal_time = time_now() / ticksPerMs();