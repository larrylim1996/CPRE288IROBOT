/**
 * @file movement.c
 * @brief this this c file will contain all needed cybot movement control and bump & cliff sensor utilities which used in the project
 *
 * @author Caining Eric Bryan Thein
 *
 * @date 12/8/2017
 */
#include <movement.h>
#include "open_interface.h"
#include <stdio.h>
#include <uart.h>
/**
 * This method will be used to make cybot move forward.
 * @author Caining Eric Bryan Thein
 * @param sensor_data The cybot sensor information
 * @param dist The distance the cybot moves
 * @date 12/8/2017
 */
void forward(oi_t *sensor_data, int dist)
{
    oi_update(sensor_data);
    int sum = 0;
    oi_setWheels(130, 130);
    while (sum < dist)
    {
        oi_update(sensor_data);

        //if a white or black or a cliff are detected, stop and send comment to putty
        if (linedetector(sensor_data) == 1)
        {
            back(sensor_data, 100);
            oi_setWheels(0, 0);
            break;
        }

        //if a bump is hit move back a bit and send command to putty
        else if (sensor_data->bumpLeft || sensor_data->bumpRight
                || (sensor_data->bumpLeft && sensor_data->bumpRight))
        {
            if (sensor_data->bumpLeft)
            {
                send_helper("Left bump detected.\n\r");
                lcd_printf("Left bump warning!!");
            }
            else if (sensor_data->bumpRight)
            {
                send_helper("Right bump detected.\n\r");
                lcd_printf("Right bump warning!!");
            }
            else if (sensor_data->bumpLeft && sensor_data->bumpRight)
            {
                send_helper("Both bump detected.\n\r");
                lcd_printf("Both bump warning!!");
            }
            oi_setWheels(0, 0);
            back(sensor_data, 50);
            break;
        }

        sum += sensor_data->distance;
    }
    oi_setWheels(0, 0);
}
/**
 * This method will be used to make cybot move backward.
 * @author Caining Eric Bryan Thein
 * @param sensor_data The cybot sensor information
 * @param dist The distance the cybot moves
 * @date 12/8/2017
 */
void back(oi_t *sensor_data, int dist)
{
    oi_update(sensor_data);
    int sum = 0;
    oi_setWheels(-130, -130);
    while (sum < dist)
    {
        oi_update(sensor_data);
        sum += (sensor_data->distance * -1);
    }
    oi_setWheels(0, 0);
}
/**
 * This method will be used to make cybot turn clockwise.
 * @author Caining Eric Bryan Thein
 * @param sensor_data The cybot sensor information
 * @param ang The degree the cybot turns
 * @date 12/8/2017
 */
void right(oi_t *sensor_data, int ang)
{
    oi_update(sensor_data);
    int angl = 0;
    oi_setWheels(-100, 100);
    while (angl < ang)
    {
        oi_update(sensor_data);
        angl -= (sensor_data->angle);
    }
    oi_setWheels(0, 0);
}
/**
 * This method will be used to make cybot turn counter clockwise.
 * @author Caining Eric Bryan Thein
 * @param sensor_data The cybot sensor information
 * @param ang The degree the cybot turns
 * @date 12/8/2017
 */
void left(oi_t *sensor_data, int ang)
{
    oi_update(sensor_data);
    int angl = 0;
    oi_setWheels(100, -100);
    while (angl < ang)
    {
        oi_update(sensor_data);
        angl += sensor_data->angle;
    }
    oi_setWheels(0, 0);
}
/**
 * This method will be used to check if cybotrun to a white line or a whole.
 * @author Caining Eric Bryan Thein
 * @param sensor_data The cybot sensor information
 * @return detector Equals 0 if nothing detected returns 1 otherwise.
 * @date 12/8/2017
 */
int linedetector(oi_t *sensor)
{
    int detector = 0;
    oi_update(sensor);
    uint16_t FL_data = sensor->cliffFrontLeftSignal;
    uint16_t FR_data = sensor->cliffFrontRightSignal;
    uint16_t L_data = sensor->cliffLeftSignal;
    uint16_t R_data = sensor->cliffRightSignal;

    //If it hits a line
    if ((FL_data > 2750 && FL_data < 2900)
            || (FR_data > 2750 && FR_data < 2900))
    {
        detector = 1;
        if ((FL_data > 2750 && FL_data < 2900)
                && (FR_data > 2750 && FR_data < 2900))
        {
            send_helper("Front line detected\n\r");
            lcd_printf("White line warning!!");
        }
        else if ((FL_data > 2750 && FL_data < 2900) && (FR_data < 2750))
        {
            send_helper("Left line detected\n\r");
            lcd_printf("White line warning!!");
        }
        else if ((FR_data > 2750 && FR_data < 2900) && (FL_data < 2750))
        {
            send_helper("Right line detected\n\r");
            lcd_printf("White line warning!!");
        }
        else if (FL_data > 2750 && FL_data < 2900)
        {
            send_helper("Front Left line detected\n\r");
        }
        else if (FR_data > 2750 && FR_data < 2900)
        {
            send_helper("Front Right line detected\n\r");
        }

    }

    //If it hits the hole
    else if (FL_data < 800 || FR_data < 900 || L_data < 750 || R_data < 750)
    {
        detector = 1;
        if (FL_data < 800 && FR_data < 900)
        {
            send_helper("Front cliff detected\n\r");
            lcd_printf("Black hole warning!!");
        }
        else if (L_data < 750 || FL_data < 800)
        {
            send_helper("Left cliff detected\n\r");
            lcd_printf("Black hole warning!!");
        }
        else if (R_data < 750 || FR_data < 900)
        {
            send_helper("Right cliff detected\n\r");
            lcd_printf("Black hole warning!!");
        }
    }
    return detector;
}
