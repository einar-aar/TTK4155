#include "sam.h"
#include "motor_controller.h"
#include <math.h>
#include <stdint.h>
#include "PI.h"

#define MOTOR_DIRECTION_PIN 23

// set PHASEDIR
void set_phase_positive(void) {
    // PHASEDIR = 1 → positive direction
    PIOC->PIO_SODR = (1u << MOTOR_DIRECTION_PIN);
}
void set_phase_negative(void) {
    // PHASEDIR = 0 → negative direction
    PIOC->PIO_CODR = (1u << MOTOR_DIRECTION_PIN);
}

// Sett duty på ENABLE-kanalen (0.0–1.0)
void set_enable_pwm_duty_ratio(float duty) {
    //ensuring the duty cycle stays within bounds { // call in time increments (f.ex. 1.ms), or in this case DT_DEFAULT
    const float duty_min = 0.00f;       //what should bounds of duty_cycle be?
    const float duty_max = 0.100f;
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

//called periodically wit ref and meas
void motor_pid_step(int ref_counts, int pos_counts, float dt) {
    if (dt <= 0.0f) dt = DT_DEFAULT;

    // error
    float e = (float)(ref_counts - pos_counts);
    float y = (float)(pos_counts);


    //implementing derivative action
    float ydot = (y-y_prev)/dt; //estimate
    float a = dt/( dt + t_d);
    filter += a *(ydot-filter);
    y_prev = y;

    //PID
    float u_unsat = (Kp * e + integ- Kd*filter);

    //saturation
    float u_sat = u_unsat;
    if (u_sat >  UMAX) u_sat =  UMAX;
    if (u_sat < -UMAX) u_sat = -UMAX;

    //enter quiet mode (deadsone)
    bool enter_quiet = (fabsf(e) < e_quiet_in);
    bool exit_quiet = (fabsf(e) > e_quiet_exit);

    if(quiet_mode) { // checking if it is necessary to enter/exit deadsone
        if(exit_quiet) quiet_mode = false;
    } else {
        if(enter_quiet) quiet_mode = true;
    }

    //turning of motor in deadsone
    if(quiet_mode) {
        u_sat = 0.0f;
        integ = 0.0f;
        set_enable_pwm_duty_ratio(0.0f);
        return;
    }


    //anti-windup
    pi_integrate_with_antiwindup(e, u_unsat, u_sat, dt);

    //setting PHASEDIR, ENABLE-duty = |u|/UMAX 
    if (u_sat >= 0.0f) set_phase_negative();
    else               set_phase_positive();

    float duty = fabsf(u_sat) / UMAX;   // 0..1
    set_enable_pwm_duty_ratio(duty);
}



int scale_joystick_pos_to_motor_pos(int joystick_pos) {
    
    const float motor_pos_max = 2806;       // range of encoder meas
    const float motor_pos_min = 0; 

    const int joystick_pos_max = 100; //range og joystick output
    const int joystick_pos_min = -100; 

    //making shure the position stays within range stays within range
    if(joystick_pos > joystick_pos_max) {

        joystick_pos = joystick_pos_max;

    }else if (joystick_pos < joystick_pos_min){

        joystick_pos = joystick_pos_min;

    }

    //linear scaling from motor_pos to joystick_pos
    float t = (float)(joystick_pos - joystick_pos_min) /(float)(joystick_pos_max - joystick_pos_min);

    int scaled_joystick_pos = (int)(motor_pos_min + t * (motor_pos_max - motor_pos_min));

    return scaled_joystick_pos;

}



void control_loop_tick(int joystick_pos) { // call in time increments (f.ex. 1.ms), or in this case DT_DEFAULT
    int pos   = get_encoder_pos();       // measurements from encoder, scaled to 
    int ref   = scale_joystick_pos_to_motor_pos(joystick_pos); // joystick reference value
    motor_pid_step(ref, pos, DT_DEFAULT);
}