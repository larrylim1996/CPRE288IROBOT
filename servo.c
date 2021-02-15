/**
* @file servo.c
* @brief this this c file will contain all needed servo control utilities which used in the project
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
#include <stdbool.h>
/**
 * This method will be used to initial servo.
 * @author Caining Eric Bryan Thein
 * @date 12/8/2017
 */
void servo_init(void)
{
    //Initialize GPIO
    SYSCTL_RCGCGPIO_R |= 0b000010;
    GPIO_PORTB_AFSEL_R |= 0b100000;
    GPIO_PORTB_PCTL_R |= 0x00700000;
    GPIO_PORTB_DEN_R |= 0xFF;
    GPIO_PORTB_DIR_R |= 0b100000;

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; // Turn on clock to TIMER1
    //Configure the timer for periodic mode
    TIMER1_CTL_R &= ~( TIMER_CTL_TBEN); //disable timerB to allow us to change settings
    TIMER1_CFG_R |= TIMER_CFG_16_BIT; //set to a 16 bit timer
    TIMER1_TBMR_R |= TIMER_TBMR_TBMR_PERIOD | TIMER_TBMR_TBAMS; //periodic and PWM enable
    TIMER1_TBPR_R = 320000 >> 16;        //set the upper 8 bits of the interval
    TIMER1_TBILR_R = 320000 & 0xFFFF;    //lower 16 bits of the interval
    TIMER1_TBPMR_R = 320000 - 24000 >> 16;
    TIMER1_TBMATCHR_R = 320000 - 23000; //move servo to the middle  23000 is mid for bot 8

    TIMER1_CTL_R |= TIMER_CTL_TBEN; //Enable TIMER1B
}
/**
 * This method will be used to turn the servo.
 * @author Caining Eric Bryan Thein
 * @param degrees The degrees servo will turn.
 * @date 12/8/2017
 */
void servo_move(int degrees)
{
    int countMax = 38360;
        int countMin = 8300;
        int count = countMin;

        if (degrees > 0)
        {
            count = countMin + degrees * 167;
            if (count > countMax)
            {
                count = countMax;
            }
        }
        TIMER1_TBMATCHR_R = 320000 - count;
}
