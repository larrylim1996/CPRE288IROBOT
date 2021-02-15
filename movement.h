/**
* @file movement.h
* @brief this header file will contain all required
* definitions and basic utilities functions.
*
* @author Caining Eric Bryan Thein
*
* @date 12/8/2017
*/
#include "open_interface.h"
#ifndef MOVEMENT_H_
#define MOVEMENT_H_
void forward (oi_t *sensor_data,int dist);
void back (oi_t *sensor_data,int dist);
void right (oi_t *sensor_data,int ang);
void left (oi_t *sensor_data,int ang);
int linedetector(oi_t *sensor);



#endif /* FAYEZMOVEMENT_H_ */
