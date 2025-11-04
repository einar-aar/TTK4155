
#include "sam.h"
#include "PWM.h"
#include "can_controller.h"


void PWMinit(uint32_t mck) {

    //activate clock for the PWM- module in Power management controller 
    //Peripheral ID = 36 (PWM) --> PMC_PCER1 bit 4
    PMC -> PMC_PCER1 =(1 << (ID_PWM - 32)); //setting bit nr 4 high as it corresponds to 36

    //pin config
    PIOC -> PIO_PDR = PIO_PDR_P19; //deactivating PIO, opening pin PC19(D44) for perihperal
    PIOC ->PIO_ABSR &= ~PIO_ABSR_P19; //enabling PWMH5 by setting peripheral function A

    /*
    PIOC -> PIO_PDR = PIO_PDR_P18; //deactivating PIO, opening pin PC18(D45) for perihperal
    PIOC ->PIO_ABSR |= PIO_ABSR_P18; //enabling PWMH6 by setting peripheral function B
    */

    
    PWM -> PWM_ENA = PWM_ENA_CHID0; //enabling PWM output for channel 0

    PWM -> PWM_IER1 = PWM_IER1_CHID0; //enabling PWM interrupt on channel 0

    //setting channel mode 
    uint32_t prescaler = 0b0000; //prescaler = MCK
    uint32_t CPOL = 0b1000000000; //setting channel polarity as high (=1)
    PWM -> PWM_CH_NUM[0].PWM_CMR = prescaler|CPOL ;
    //PWM -> PWM_CH_NUM[0].PWM_CMR = PWM_CMR_


    //setting channel period (CPRD) 
    //(for left aligned waveform)
    // T = (prescaler*CPRD)/ MCK --> CPRD = T*MCK/prescaler 
    int CPRD_val = 0.02*mck;
    PWM -> PWM_CH_NUM[0].PWM_CPRD = CPRD_val;

    //seting duty cycle for channel //not in init function! move next time
    //CDTY = MCK*T_high
    int CDTY_val = mck*0.0015; //
    PWM -> PWM_CH_NUM[0].PWM_CDTY = CDTY_val;

    //set clock for PWM module
    //F_PWM = CPRD/T, T = 20ms (forslag) -> F_PWM = 10Mhz
    //CLKA = MCK/DIVA --> DIVA = 840000000/1000000 = 84

    PWM -> PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);

}

int duty_cycle_calc(uint8_t pos_val){ //function scaling output from joystick and touchpad display values from to duty cycele value: 
                                        //min_val -> 0.9 ms duty cycle, max_val -> 2.1 ms duty cycle
                                        

}

void controller_output_to_duty_cycle(CAN_MESSAGE* can_msg) {


}