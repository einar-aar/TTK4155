#include "sam.h"
#include <math.h>
#include <stdint.h>

#define UMAX        100.0f     // Maximum "control signal" (scaling unit), a limit
#define DT_DEFAULT  0.001f     // 1 kHz regulating frequency --> T = dt = 1/1000= 0.01

static volatile float Kp = 0.6f;    //tuning variables
static volatile float Ki = 50.0f;    // Ki = Ki * dt for each call
static volatile float integ = 0.0f;  //integral action

// set PHASEDIR
static inline void set_phase_positive(void) {
    // PHASEDIR = 1 → positive direction
    PIOC->PIO_SODR = (1u << MOTOR_DIRECTION_PIN);
}
static inline void set_phase_negative(void) {
    // PHASEDIR = 0 → negative direction
    PIOC->PIO_CODR = (1u << MOTOR_DIRECTION_PIN);
}

// Sett duty på ENABLE-kanalen (0.0–1.0)
static inline void set_enable_pwm_duty_ratio(float duty) {
    //ensuring the duty cycle stays within bounds
    const float duty_min = 0.045f;       
    const float duty_max = 0.105f;
    if (duty < duty_min) duty = duty_min;
    if (duty > duty_max) duty = duty_max;

    uint32_t cprd = PWM->PWM_CH_NUM[0].PWM_CPRD;
    uint32_t cdty = (uint32_t)(duty * (float)cprd);
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = cdty;
}

static inline void pi_integrate_with_antiwindup(float e, float u_unsat, float u_sat, float dt) {
    // // integrating only when the signal is not saturated
    if ( (u_unsat == u_sat) || ((u_unsat > u_sat) && (e < 0.0f)) || ((u_unsat < u_sat) && (e > 0.0f)) ) {
        integ += (Ki * dt) * e;
    }
}

// Kalles periodisk med mål og referanse (i tellemikro-trinn)
void motor_pi_step(int ref_counts, int pos_counts, float dt) {
    if (dt <= 0.0f) dt = DT_DEFAULT;

    // error
    float e = (float)(ref_counts - pos_counts);

    // 2) PI
    float u_unsat = Kp * e + integ;

    // 3) saturation
    float u_sat = u_unsat;
    if (u_sat >  UMAX) u_sat =  UMAX;
    if (u_sat < -UMAX) u_sat = -UMAX;

    // 4) Anti-windup
    pi_integrate_with_antiwindup(e, u_unsat, u_sat, dt);

    // 5) setting PHASE, ENABLE-duty = |u|/UMAX
    if (u_sat >= 0.0f) set_phase_positive();
    else               set_phase_negative();

    float duty = fabsf(u_sat) / UMAX;   // 0..1
    set_enable_pwm_duty_ratio(duty);
}



float scale_motorpos_to_joystick_pos(int motor_pos) {
    
    const float motor_pos_max = 2102;       // range of encoder meas
    const float mototr_pos_min = 0; 

    const int joystick_pos_max = 100; //range og joystick output
    const int joystick_pos_min = -100; 

    //making shure the duty sycle stays within range
    if(motor_pos > motor_pos_max) {

        motor_pos = motor_pos_max;

    }else if (motor_pos < motor_pos_min){

        motor_pos = motor_pos_min;

    }

    //linear scaling from motor_pos to joystick_pos
    float t = (float)(motor_pos - motor_pos_min) /(float)(motor_pos_max - motor_pos_min);

    float scaled_motor_pos = joystick_pos_min + t * (joystick_pos_max - joystick_pos_min);

    return scaled_motor_pos;

}



void control_loop_tick(void) { // call in time increments (f.ex. 1.ms), or in this case DT_DEFAULT
    int pos   = (int)(scale_motor_pos_to_joystick_pos(get_encoder_pos()));       // measurements from encoder, scaled to 
    int ref   = joystick_value_to_counts(); // scaling joystick to the same unirt as encoder skaler joystick til samme enhet som encoder
    motor_pi_step(ref, pos, DT_DEFAULT);
}