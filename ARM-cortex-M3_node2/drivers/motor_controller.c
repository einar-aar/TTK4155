#include "motor_controller.h"
#include "sam.h"
#include "PWM.h"
#include <stdint.h>

#define MOTOR_DIRECTION_PIN 23
#define F_CPU 84000000

void encoder_init() {
    //activate clock for the Timer counter- module in Power management controller 
    //Peripheral ID = 29 (PWM) --> PMC_PCER0 bit 4
    PMC -> PMC_PCER0 |= PMC_PCER0_PID29; //setting bit nr 29 high as it corresponds to 29
    PMC -> PMC_PCER0 |= (1 << ID_PIOC);
    PIOC ->PIO_PDR |= (PIO_PC25 | PIO_PC26); //deactivating PIO, opening pin PC25&PC26 for perihperal
    PIOC -> PIO_ABSR |= (PIO_PC25 | PIO_PC26); //setting peripheral function B at pin PC25&26
    
    //PMC->PMC_PCER0 |= (1 << ID_PIOC); // Activate clock for PIOC

    //deactivating write protection
    TC2 -> TC_WPMR = (0x54494D << TC_WPMR_WPKEY_Pos); //WPKEY =0x54494D 

    TC2->TC_CHANNEL[0].TC_CMR &= ~TC_CMR_WAVE; //enable capture mode
    TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_XC0; //setting XC0 as clock
    TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_ETRGEDG_RISING; //triggering edge
    TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_ABETRG; //aetting external trigger as TIOA

    //setting block mode register
    TC2->TC_BMR = 0;
    TC2->TC_BMR |= TC_BMR_QDEN;
    TC2->TC_BMR |= TC_BMR_POSEN;
    //TC2 -> TC_BMR |= TC_BMR_SPEEDEN;
    TC2->TC_BMR |= TC_BMR_EDGPHA;
    TC2->TC_BMR |= TC_BMR_MAXFILT(4);
    PMC -> PMC_PCER0 |=PMC_PCER0_PID12;

    //starting timer counter 2 (TC2)
    TC2 -> TC_CHANNEL[0].TC_CCR = (TC_CCR_CLKEN | TC_CCR_SWTRG);

    PIOC -> PIO_PUDR |= (PIO_PC25 | PIO_PC26);; //disabling pull up resister on I/O line
    PIOC -> PIO_IFER |= (PIO_PC25 | PIO_PC26);; //enables input glitch filter on I/O-line

    //enabling direction control
    PIOC -> PIO_PER |=(1 << MOTOR_DIRECTION_PIN);
    PIOC -> PIO_OER |=(1 << MOTOR_DIRECTION_PIN);
    PIOC -> PIO_CODR |=(1 << MOTOR_DIRECTION_PIN);

    
    //reactivating write protection
    //TC2 -> TC_WPMR = ((0x54494D << 8) | 1);
    TC2 -> TC_WPMR |= 1;
    

}


int get_encoder_pos(void) {
    return (int)(TC2 -> TC_CHANNEL[0].TC_CV);
}


void set_motor_dir(int joystick_value) {

    if (joystick_value < -5) {
        
        printf("Sliding in - direction\n\r");
        PIOC->PIO_CODR |= (1 << MOTOR_DIRECTION_PIN);

    } else if (joystick_value > 5) {
        
        printf("Sliding in + direction\n\r");
        PIOC->PIO_SODR |= (1 << MOTOR_DIRECTION_PIN);
    }
}

void set_motor_pos(int joystick_value) {

    set_motor_dir(joystick_value);
    set_duty_cycle(joystick_value, F_CPU, 0);
}