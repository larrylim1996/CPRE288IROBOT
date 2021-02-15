/**
* @file uart.c
* @brief this this c file will contain all needed serial communication utilities which used in the project
*
* @author Caining Eric Bryan Thein
*
* @date 12/8/2017
*/
#include <stdio.h>
#include <string.h>
#include "Timer.h"
#include "lcd.h"
#include "uart.h"

/**
 * This method will be used to initialize the uart registers.
 * @author Caining Eric Bryan Thein
 * @date 12/8/2017
 */
void uart_init(void) {

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1; //Enables clock to GPIO
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1; //Enables clock to UART
    GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1); //enables alt. functions
    GPIO_PORTB_PCTL_R |= 0x00000011; //enables Rx and Tx on P0 and P1
    GPIO_PORTB_DEN_R |= (BIT0 | BIT1); //Sets those pins to digital
    GPIO_PORTB_DIR_R &= ~BIT0; //Sets pin 0 to input
    GPIO_PORTB_DIR_R |= BIT1; //sets pin 1 to output

    uint16_t iBRD = 8;
    uint16_t fBRD = 44;
    UART1_CTL_R &= ~(UART_CTL_UARTEN);
    UART1_IBRD_R = iBRD;
    UART1_FBRD_R = fBRD;
    UART1_LCRH_R = UART_LCRH_WLEN_8;
    UART1_CC_R = UART_CC_CS_SYSCLK; //sets the system clock as the source.

    UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
}
/**
 * This method will be used to send a string to putty.
 * @author Caining Eric Bryan Thein
 * @param string The string will be sent to putty.
 * @date 12/8/2017
 */
void send_helper(char* string) {
    int i = 0;
    char c;
    for (i = 0; i < strlen(string); i++) {
        c = string[i];
        uart_send(c);
    }

}
/**
 * This method will be used to send a char to putty.
 * @author Caining Eric Bryan Thein
 * @param data The char will be sent to putty.
 * @date 12/8/2017
 */
void uart_send(char data) {
    while (UART1_FR_R & 0x20)
        ;
    UART1_DR_R = data;
}
/**
 * This method will be used to recive a char from putty.
 * @author Caining Eric Bryan Thein
 * @return data The char get from putty.
 * @date 12/8/2017
 */
char uart_recieve(void) {
    char data = 0;
    while (UART1_FR_R & UART_FR_RXFE) {

    }
    data = (char) (UART1_DR_R & 0xFF);
    return data;
}
