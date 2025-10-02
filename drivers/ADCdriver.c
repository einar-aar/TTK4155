#include "ADCdriver.h"
#include "SPIdriver.h"

// Initialize clock
int init_clock(void) {

    // Set pin D4 as output for PWM signal
    DDRD |= (1 << PD5);

    // Register for timer 1
    // This will make sure the selected pin will toggle when the timer matches the defined toggle-time
    TCCR1A |= (1 << COM1A0);

    // FYI: The timer has 2 registers: A and B. These have different bit-information tied to them. See page 128 and 131 in datasheet

    // Initialize CTC mode. This makes sure the timer is reset every time the timer reaches the defined value.
    TCCR1A |= (1 << WGM10);
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << WGM13);

    // Set the value that the timer should be compared to and trigger output
    // The formula for frequency equals F_CPU / 2 * prescaler * (1 + OCR1A)
    OCR1A = 1;
    // This sets the clock frequency to 4.9152 / 2*(1 + 1) == 1,2288 MHz

    // Choose prescaler to make the frequency between 0,5 MHz to 5MHz
    // We choose prescaler == 1 to make the clock frequency == mcu clock frequency
    TCCR1B |= (1 << CS10);

    return 0;
}


// Read function
int* ADC_read_joystick_and_pad(void) {

    int* result = malloc(sizeof(uint8_t)*4);
    memset(result, 0, sizeof(uint8_t)*4);

    SPI_slaveselect(OLED);

    *ADC = 0;

    _delay_ms(50);

    const int joystick_x_center = 158; //center, max and min value for joystick x
    const int joystick_x_min_val= 71;
    const int joystick_x_max_val = 240;

    const int joystick_y_center= 158; //center, max and min value for joystick y
    const int joystick_y_min_val= 0;
    const int joystick_y_max_val = 243;

    const int touchpad_center = 157; //center, max and min value for touchpad 
    const int touchpad_min_val= 0;
    const int touchpad_max_val = 255;




    // Put CH0, 1, 2 and 3 in array result
    for (int i = 0; i < 4; i++) {

         int original_value = *ADC;
         int scaled = 0;

         if (i == 1) {//x axis joystick
            if (original_value < joystick_x_center) {
                
                scaled = (int) (((float)((original_value-joystick_x_center)*100)/(joystick_x_min_val-joystick_x_center)));
            }
            else {
                scaled = (int) (((float)(original_value-joystick_x_center)/(joystick_x_max_val-joystick_x_center))*-100);
            }
         }

         else if (i == 0) { //y_axis joystick
            if (original_value < joystick_y_center) {
                
                scaled = (int) (((float)(original_value-joystick_y_center)/(joystick_y_min_val-joystick_y_center))*100/55*100);
            }
            else {
                scaled = (int) (((float)(original_value-joystick_y_center)/(joystick_y_max_val-joystick_y_center))*-100);
            }
         }
         else { //touchpad x and y axis
            if (original_value < touchpad_center) {
                
                scaled = (int) (((float)(original_value-touchpad_center)/(touchpad_min_val-touchpad_center))*100);
            }
            else {
                scaled = (int) (((float)(original_value-touchpad_center)/(touchpad_max_val-touchpad_center))*-100);
            }
         }
          

        

        // limiting from [-100, 100]
        if (scaled > 100) scaled = 100;
        if (scaled < -100) scaled = -100;

        result[i]= -scaled;
    }

    SPI_release_slave();

    return result;
}

// read IO module
void ADC_read_IO_module(uint8_t right_buttons[6], uint8_t left_buttons[7], uint8_t navigation_buttons[5]) {

    uint8_t right, left, navigation;

    SPI_slaveselect(IO_MCU);

    SPI_transfer(0x04);
    _delay_us(40);

    right = SPI_transfer(0x00);
    _delay_us(2);

    left = SPI_transfer(0x00);
    _delay_us(2);

    navigation = SPI_transfer(0x00);

    SPI_release_slave();

    for (int i = 0; i < 6; i++) right_buttons[i] = (right >> i) & 1;
    for (int i = 0; i < 7; i++) left_buttons[i] = (left >> i) & 1;
    for (int i = 0; i < 5; i++) navigation_buttons[i] = (navigation >> i) & 1;
}


// Read joystick button
uint8_t read_joystick_button(void) {

    return (PINB & (1 << Joystick_btn) ? 1 : 0);
}
