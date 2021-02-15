/**
* @file WiFi.c
* @brief this c file will contain all needed wifi utilities which used in the project
*
* @author Caining Eric Bryan Thein
*
* @date 12/8/2017
*/
#include "WiFi.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdio.h>
#include <timer.h>
#define COMMAND_PIN	0x04

#define COMMAND_START	0
#define COMMAND_STOP	1

#define RETVAL_SUCCESS	0


/**
 * starts WiFi bridge
 * @param password a null terminated string to be used as wireless password
 * @return 0 on success, -1 failure
 */
int WiFi_start(char *password) {
	//Initialize UART
	//UART_Init();

	//Initialize command pin for WiFi connection
	SYSCTL_RCGCGPIO_R |= BIT1; //Turn on GPIO Port B
	GPIO_PORTB_DEN_R |= COMMAND_PIN; //Enable PB2
	GPIO_PORTB_DIR_R |= COMMAND_PIN; //Set PB2 to output
	GPIO_PORTB_DATA_R &= ~COMMAND_PIN; //Clear command pin

	//Wait for pin to settle
	timer_waitMillis(1);

	//send the start command with password to the wifi
	uint8_t response = _sendCommand(COMMAND_START, password, strlen(password) + 1);

	if(response != RETVAL_SUCCESS) {
		return -1;
	}
	else {
		return 0;
	}
}

int WiFi_stop() {
	return _sendCommand(COMMAND_STOP, NULL, 0);
}

uint8_t _sendCommand(uint8_t command, uint8_t* param, uint8_t len) {
	//Raise command Pin High
	GPIO_PORTB_DATA_R |= COMMAND_PIN;

	//Send WiFi command - 0x00
	uart_send(command);

	//Send parameter (if applicable)
	if(len > 0) {
		//send password
		while(*param != 0){
		    uart_send(*param);
		    param++;
		}
		//send null
		uart_send(0);
	}

	//Wait for response from WiFi microcontroller
	int response =  uart_recieve();

	//Lower the command Pin
	GPIO_PORTB_DATA_R &= ~COMMAND_PIN;

	return response;
}
