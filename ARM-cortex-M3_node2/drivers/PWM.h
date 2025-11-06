#include "can_controller.h"


void PWMinit(uint32_t mck);
float controller_output_to_duty_ratio(int contr_output);
void set_duty_cycle(int power, uint32_t mck, int channel);
