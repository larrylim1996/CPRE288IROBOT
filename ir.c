/**
* @file ir.c
* @brief this this c file will contain all needed ir sensor utilities which used in the project
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

/**
 * This method will be used to initial ir sensor.
 * @author Caining Eric Bryan Thein
 * @date 12/8/2017
 */
void ir_init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    SYSCTL_RCGCADC_R |= 0x1;
    GPIO_PORTB_AFSEL_R |= 0x01;
    GPIO_PORTB_DEN_R &= 0xFE;
    GPIO_PORTB_AMSEL_R |= 0x01;
    GPIO_PORTB_DIR_R = 0x10;

    GPIO_PORTB_ADCCTL_R = 0x0;
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN0;
    ADC0_EMUX_R = ADC_EMUX_EM0_PROCESSOR;
    ADC0_SSMUX0_R |= 0x000A;
    ADC0_SSCTL0_R |= (ADC_SSCTL0_IE0 | ADC_SSCTL0_END0);
    ADC0_SAC_R |= ADC_SAC_AVG_64X;
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
}
/**
 * This method will be used to calculate distance through the data from ir sensor.
 * @author Caining Eric Bryan Thein
 * @return cm The distance in cm
 * @date 12/8/2017
 */
double ir_getDistance()
{
    double cm;
    ADC0_PSSI_R = ADC_PSSI_SS0;
    while ((ADC0_RIS_R & ADC_RIS_INR0) == 0)
    {

    }
    double readingIR = ADC0_SSFIFO0_R;
    cm  = 87293 * pow(readingIR, -1.132);
    return cm;
}
