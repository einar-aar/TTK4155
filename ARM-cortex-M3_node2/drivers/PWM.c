#include "sam.h"
#include "PWM.h"
#include "can_controller.h"


void PWMinit(uint32_t mck) {

    //activate clock for the PWM- module in Power management controller 
    //Peripheral ID = 36 (PWM) --> PMC_PCER1 bit 4
    PMC -> PMC_PCER1 = (1 << 4); //setting bit nr 4 high as it corresponds to 36. Setting clk for PWM
    PMC -> PMC_PCER0 |= (1u << ID_PIOB); // Set clk for PIOB

    //the motor shield (Servo sig) uses pb13 
    PMC -> PMC_PCER0 |= PMC_PCER0_PID12; //enables peripheral clock TC1 (timer counter channel 1)
    PIOB -> PIO_PDR |= PIO_PDR_P13; //deactivating PIO, opening pin PB13 for perihperal
    PIOB -> PIO_ABSR |= PIO_ABSR_P13; //setting peripheral function B at pin PB13
    PIOB -> PIO_MDDR |= (1<<13); //deactivating multi-driver, setting as standard output on PB13

    //enabling pwm signal for motor driver (PB12)
    PIOB -> PIO_PDR |= PIO_PDR_P12; //deactivating PIO, opening pin PB13 for perihperal
    PIOB -> PIO_ABSR |= PIO_ABSR_P12; //setting peripheral function B at pin PB12
    PIOB -> PIO_MDDR |= (1<<12); //deactivating multi-driver, setting as standard output on PB12

    //deactivating write protection
    PWM->PWM_WPCR = (0x50574D << 8) | (0b11111100); //WPKEY =0x50574D
    PWM-> PWM_WPCR &= ~((1<<0)||(1<<1));

    if(((PWM -> PWM_WPSR & PWM_WPSR_WPHWS0) | (PWM -> PWM_WPSR & PWM_WPSR_WPHWS1)) !=0) {
        printf("WP-fault\r\n");
        return;
    }
    
    //set clock for PWM module (initialize PWM chapter 38.6.5.1

    PWM -> PWM_CLK = 0; // stopping all clocks
    PWM -> PWM_CLK |= (1<<0) | (1<<16); //activating both clocks (CLKA&B) without dividing

    //setting channel mode 
    uint32_t prescaler = 0b0111 ; //dividing MCK/128
    uint32_t CPOL = 0b1000000000; //setting channel polarity as high (=1)

    // channel 1
    PWM -> PWM_CH_NUM[1].PWM_CMR = prescaler|CPOL ;
    PWM -> PWM_CH_NUM[1].PWM_CMR &= ~(1<<10); //setting CES = 0 The channel counter is clocked by the prescaler output

    //channel 0
    PWM -> PWM_CH_NUM[0].PWM_CMR = prescaler|CPOL ;
    PWM -> PWM_CH_NUM[0].PWM_CMR &= ~(1<<10); //setting CES = 0 The channel counter is clocked by the prescaler output


    //setting channel period (CPRD) 
    //(for left aligned waveform)
    // T = (prescaler*CPRD)/ MCK --> CPRD = T*MCK/prescaler 
    int CPRD = 0.02*mck/128; //definert fra før av?
    PWM -> PWM_CH_NUM[1].PWM_CPRD = CPRD;
    PWM -> PWM_CH_NUM[0].PWM_CPRD = CPRD;


    //seting duty cycle for channel (only init value, real value will be set as a function of controller output )
    //duty cycle = (T- 1/f_channel*CDTY)/(T) , f_channel = mck/128

    int CDTY = (CPRD/2); //
    PWM -> PWM_CH_NUM[1].PWM_CDTY = CDTY;
    PWM -> PWM_CH_NUM[0].PWM_CDTY = CDTY;


    PWM -> PWM_ENA |= PWM_ENA_CHID1; //enabling PWM output for channel 1
    PWM -> PWM_ENA |= PWM_ENA_CHID0; //enabling PWM output for channel 0


    PWM -> PWM_IER1 |= PWM_IER1_CHID1; //enabling PWM interrupt on channel 1
    PWM -> PWM_IER1 |= PWM_IER1_CHID0; //enabling PWM interrupt on channel 0
 

    //active write protection
    PWM -> PWM_WPCR = (0x50574D << 8) | (0b11011100) |(1<<0); //enabling write protect for everything but the period and duty_cycle registers

    return;

}


//takes in joystick_pos in x direction and calculates duty cycle 


float controller_output_to_duty_ratio(int contr_output) {
    
    const int contr_output_max = 100; //what is the range of the controller output?
    const int contr_output_min = -100; 

    const float duty_min = 0.045f;       //  0.9 ms / 20 ms ≈ 4.5 %
    const float duty_max = 0.105f;       //  2.1 ms / 20 ms ≈ 10.5 %


    //making shure the duty sycle stays within range
    if(contr_output > contr_output_max) {

        contr_output = contr_output_max;

    }else if (contr_output < contr_output_min){

        contr_output = contr_output_min;

    }

    //linear scaling from contr output to duty_ratio
    float t = (float)(contr_output - contr_output_min) /(float)(contr_output_max - contr_output_min);

    float duty_ratio = duty_min + t * (duty_max - duty_min);

    return duty_ratio;

}



void set_duty_cycle(int power, uint32_t mck, int channel) {
    //channel 0 = motor driver (joystick xpos)
    //channel 1 = servo (joystick y_pos)
    /*if (channel != 0 || channel != 1) {
        printf("invalid channel choice\n\r");
        return;
    }*/

    float duty_ratio = controller_output_to_duty_ratio(power); // testverdi

    //CDTY = duty_ratio*CPRD
    uint32_t CPRD = PWM->PWM_CH_NUM[channel].PWM_CPRD;

    //uint32_t CDTYmax = (uint32_t)(0.105 * (float)CPRD);// calculating max and min 

    uint32_t CDTY_val = (uint32_t)(duty_ratio * (float)CPRD);    
    //uint32_t CDTY_val = CPRD/8;

    if (channel==0) CDTY_val *= 3;

    //setting duty_cycle
    PWM->PWM_CH_NUM[channel].PWM_CDTYUPD = CDTY_val;
    
}
