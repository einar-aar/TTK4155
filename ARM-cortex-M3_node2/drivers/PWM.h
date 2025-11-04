#include "can_controller.h"


void PWMinit(uint32_t mck);
int controller_output_to_duty_cycle(int contr_output);
void set_duty_cycle(CAN_MESSAGE* msg, uint32_t mck);
