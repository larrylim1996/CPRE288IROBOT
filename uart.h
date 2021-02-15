/**
* @file uart.h
* @brief this header file will contain all required
* definitions and basic utilities functions.
*
* @author Caining Eric Bryan Thein
*
* @date 12/8/2017
*/
#ifndef UART_H_
#define UART_H_

char uart_recieve(void);
void send_helper(char* string);
void uart_send(char data);
void uart_init(void);
#endif /* UART_H_ */
