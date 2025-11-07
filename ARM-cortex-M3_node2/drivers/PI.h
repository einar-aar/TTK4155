


#define UMAX        1.0f     // Maximum "control signal" (scaling unit), a limit
#define DT_DEFAULT  0.01f     // 1 kHz regulating frequency --> T = dt = 1/1000= 0.01

static volatile float Kp = 0.001f;    //tuning variables
static volatile float Ki = 0.000f;    // Ki = Ki * dt for each call
static volatile float Kd = 0.00001f;  

static volatile float integ = 0.0f;  //integral action

//lowpass filter: a = dt/ (dt+ t_d)
static volatile float t_d = 0.01f;  //timestep on lowpass_filter (adjustable)
static float y_prev = 0.0f; //measurement of previous time step
static float filter = 0.0f;


static inline void set_phase_positive(void);
static inline void set_phase_negative(void);
static inline void set_enable_pwm_duty_ratio(float duty);
static inline void pi_integrate_with_antiwindup(float e, float u_unsat, float u_sat, float dt);
void motor_pid_step(int ref_counts, int pos_counts, float dt);
int scale_joystick_pos_to_motor_pos(int joystick_pos);
void control_loop_tick(int joystick_pos);
