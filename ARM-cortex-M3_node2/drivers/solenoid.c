#include "solenoid.h"
#include "sam.h"

#define SOLENOID_PIN 25

void solenoid_init() {

    PMC->PMC_PCER0 |= (1 << 12); // Activate clock for PIOB

    PIOB->PIO_PER |= (1 << SOLENOID_PIN); // Activate control over pin B25

    PIOB->PIO_OER |= (1 << SOLENOID_PIN); // Activate output

    PIOB->PIO_SODR |= (1 << SOLENOID_PIN); // Clear output register
}

void solenoid_activate() {

    PIOB->PIO_CODR |= (1 << SOLENOID_PIN); // Set pin low, logic high

}

void solenoid_deactivate() {

    PIOB->PIO_SODR |= (1 << SOLENOID_PIN); // Set pin high, logic low
}