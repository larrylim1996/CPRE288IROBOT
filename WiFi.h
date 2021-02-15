/**
* @file WiFi.h
* @brief this header file will contain all required
* definitions and basic utilities functions.
*
* @author Caining Eric Bryan Thein
*
* @date 12/8/2017
*/
#ifndef WIFI_H_
#define WIFI_H_

#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "UART.h"

int WiFi_start(char *password);

int WiFi_stop();

uint8_t _sendCommand(uint8_t command, uint8_t* param, uint8_t len);

#endif /* WIFI_H_ */
