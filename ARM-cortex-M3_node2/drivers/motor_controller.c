#include "motor_controller.h"
#include "sam.h"
#include "PWM.h"

#define MOTOR_DIRECTION_PIN 23
#define F_CPU 84000000

void encoder_init() {
    //activate clock for the Timer counter- module in Power management controller 
    //Peripheral ID = 29 (PWM) --> PMC_PCER0 bit 4
    PMC -> PMC_PCER0 =(1 << 29); //setting bit nr 29 high as it corresponds to 29

    PIOC -> PIO_PDR |= PIO_PDR_P25; ////deactivating PIO, opening pin PC25&PC26 for perihperal
    PIOC -> PIO_PDR |= PIO_PDR_P26;
    PIOC -> PIO_ABSR |= PIO_ABSR_P25; //setting peripheral function B at pin PC25
    PIOC -> PIO_ABSR |= PIO_ABSR_P26; //setting peripheral function B at pin PC26

    PMC->PMC_PCER0 |= (1 << ID_PIOC); // Activate clock for PIOC


    //deactivating write protection
    TC2 -> TC_WPMR = (0x54494D << 8); //WPKEY =0x54494D 

    //setting block mode register
    uint32_t bmr = (1<<8) | (1<<9) | (1<<0);  //activating quadrature mode and  enabling postition on channel 0
    uint32_t bmr = (1<<8) | (1<<9) | (1<<0);  //activating quadrature mode and  enabling postition on channel 0
    TC2->TC_BMR = bmr;

    //selecting clock for channel 0
    TC2->TC_CHANNEL[0].TC_CMR = 0b101; //selcting clock XC0

    //enabling channel 0
    TC2->TC_CHANNEL[0].TC_CCR = (1<<0) | (1<<2);

    // ENABLE DIRECTION CONTROLL

    PIOC->PIO_PER |= (1 << MOTOR_DIRECTION_PIN); // Activate control over pin C23

    PIOC->PIO_OER |= (1 << MOTOR_DIRECTION_PIN); // Activate output

    PIOC->PIO_CODR |= (1 << MOTOR_DIRECTION_PIN); // Clear output register
}

void set_motor_dir(int joystick_value) {

    if (joystick_value < 0) {
        
        printf("Sliding in - direction\n\r");
        PIOC->PIO_CODR |= (1 << MOTOR_DIRECTION_PIN);

    } else {
        
        printf("Sliding in + direction\n\r");
        PIOC->PIO_SODR |= (1 << MOTOR_DIRECTION_PIN);
    }
}

void set_motor_pos(int joystick_value) {

    set_motor_dir(joystick_value);
    set_duty_cycle(joystick_value, F_CPU, 0);
}