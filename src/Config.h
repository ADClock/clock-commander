#ifndef _CONFIG_H_
#define _CONFIG_H_

// Communication parameters
#define CLOCKS_IN_ROW 24
#define MIN_OUT_CLOCK_HIGH 4                   // us
#define MIN_OUT_CLOCK_LOW 12                   // us
#define APPROX_SENDING_TIME_PER_INSTRUCTION 20 // us
#define DELAY_BETWEEN_INSTRUCTIONS 300         // us

// Motor
#define MAX_STEPS 3414             // steps per rotation
#define MIN_STEP_DELAY 1100        // us
#define MIN_STANDSTILL_DELAY 10000 // us

// Pins

#define CLOCK_OUT 4
#define DATA1_OUT 3
#define DATA2_OUT 5
#define DATA3_OUT 6
#define DATA4_OUT 7

// Enable Debug messages for Serial
// #define DEBUG

#endif