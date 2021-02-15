/**
* @file ping.c
* @brief this this c file will contain all needed sonar sensor utilities which used in the project
*
* @author Caining Eric Bryan Thein
*
* @date 12/8/2017
*/
#include <stdint.h>
#include <stdio.h>
#include <inc/tm4c123gh6pm.h>
#include <timer.h>
#include <lcd.h>
#include <math.h>
#include <ping.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"

#define TIMER3B_PRESCALER 0xFFFF
volatile int pulseW = 0;
volatile int rising = 0;
volatile int falling = 0;
int isRisingEdge = 1;
int reNumedge = 0;

/**
 * Function to send one pulse of the ping sensor
 * Initializes the PORTB registers for the ping sensor as well as the timer 3. It sets PB3 to high for output,
 * sends a pulse, waits 5 microseconds, sets the PB3 to low for input and records the pulse that is returned.
 * @author Caining Eric Bryan Thein
 *
 * @date 12/8/2017
 */
void send_pulse()
{
    GPIO_PORTB_AFSEL_R &= ~0b01000; //disable alt fun
    GPIO_PORTB_DIR_R |= 0x08;       //enable output
    GPIO_PORTB_DATA_R |= 0x08;      //set high

    timer_waitMicros(5);

    GPIO_PORTB_DATA_R &= 0xF7;
    GPIO_PORTB_DIR_R &= 0xF7;
    GPIO_PORTB_AFSEL_R |= 0b01000;
    GPIO_PORTB_PCTL_R |= 0x00007000;               //set alt fun
    TIMER3_CTL_R |= TIMER_CTL_TBEN; //disable timerB to allow us to change settings

    timer_waitMillis(100);
}
/**
 * Handler fucntion for interrupts for the ping sensor
 * Function takes in no parameters and returns no values.
 * If the state is LOW, set the state to HIGH and the rising time to this time.
 * If it was HIGH, set it to DONE and the falling time to this time.
 *
 *  @author Caining Eric Bryan Thein
 *  @date 12/8/2017
 */
void TIMER3B_Handler(void)
{
    if ( TIMER3_MIS_R & TIMER_MIS_CBEMIS)
    {
        TIMER3_ICR_R = TIMER_ICR_CBECINT; //clear flag
        if (isRisingEdge)
        {
            rising = TIMER3_TBR_R | (TIMER3_TBPS_R << 16);
            isRisingEdge = 0;
        }
        else
        {
            falling = TIMER3_TBR_R | (TIMER3_TBPS_R << 16);
            TIMER3_CTL_R &= ~( TIMER_CTL_TBEN); //disable timerB to allow us to change settings
            isRisingEdge = 1;
        }
        reNumedge++;
    }

    if ( TIMER3_MIS_R & TIMER_MIS_TBTOMIS)
    {
        TIMER3_ICR_R = TIMER_ICR_TBTOCINT; //clear flag/part 4 overflow
    }

}
/**
 * Sets up Ping on robot
 * Initializes the Ping registers which allows us to use the Ping module for edge detection.
 *
 *  @author Caining Eric Bryan Thein
 *  @date 12/8/2017
 */
void ping_init(void)
{
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; // Turn on clock to TIMER3
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;    //enable clk B
    GPIO_PORTB_DEN_R |= 0b01000;                   //enabling PIN 3
    //Configure the timer for input capture mode
    TIMER3_CTL_R &= ~( TIMER_CTL_TBEN); //disable timerB to allow us to change settings
    TIMER3_CFG_R |= TIMER_CFG_16_BIT; //set to a 16 bit timer
    TIMER3_TBMR_R |= TIMER_TBMR_TBMR_CAP | TIMER_TBMR_TBCMR | TIMER_TBMR_TBCDIR;
    TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;
    TIMER3_MIS_R |= 0xFF;
    TIMER3_TBPR_R = TIMER3B_PRESCALER;  // 200 ms clock
    TIMER3_TBILR_R = 0xFFFF; //
    //clear TIMER3B interrupt flags
    TIMER3_ICR_R |= ( TIMER_ICR_CBECINT | TIMER_ICR_TBTOCINT); //set bit to clear TIMER3 time-out interrupt flags pg. 754 3more bits
    TIMER3_IMR_R |= (TIMER_IMR_TBTOIM | TIMER_IMR_CBEIM); //enable TIMER3 time-out interrupts pg. 745

    //initialize local interrupts
    NVIC_EN1_R = 0b10000; //#warning "enable interrupts for TIMER3B" n = 0, 1, 2, 3, or 4
    //go to page 105 and find the corresponding interrupt numbers for TIMER3 A&B
    //then set those bits in the correct interrupt set EN register (p. 142)

    IntRegister(INT_TIMER3B, TIMER3B_Handler); //register TIMER3B interrupt handler

    IntMasterEnable(); //intialize global interrupts

    TIMER3_CTL_R |= TIMER_CTL_TBEN; //Enable TIMER4B
}
/**
 * This method will be used to calculate distance through the data from ping sensor.
 * @author Caining Eric Bryan Thein
 * @return cm The distance in cm
 * @date 12/8/2017
 */
float ping_getDistance()
{
    float cm = 0;
    reNumedge = 0;
    send_pulse();
    while (reNumedge != 2)
    {
    }
    pulseW = falling - rising;
    cm = (1 / 16000000.0) * .5 * 34000 * pulseW;
    return cm;
}

