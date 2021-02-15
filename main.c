/**
* @file main.c
* @brief this this c file includes the main function of project which gives command to cybot.
*
* @author Caining Eric Bryan Thein
*
* @date 12/8/2017
*/
#include <movement.h>
#include <stdio.h>
#include <uart.h>
#include "lcd.h"
#include "open_interface.h"
#include "timer.h"
#include "ping.h"
#include "ir.h"
#include "WiFi.h"
#include "servo.h"

void scan();
void flash();
void music();
/**
 * This method will be used to initial all registers and give command to cybot.
 * @author Caining Eric Bryan Thein
 * @date 12/8/2017
 */
void main()
{
    //initial all registers
    lcd_init();
    ir_init();
    ping_init();
    servo_init();
    uart_init();
    if (~(WiFi_start("aPassword"))){
        lcd_printf("WiFi start success, waiting for connection.");
    }
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    while (1)
    {
        oi_update(sensor_data);

        //get command through wifi
        char c = uart_recieve();
        //move forward
        if (c == 'w')
        {
            forward(sensor_data, 45);
        }
        //move backward
        else if (c == 's')
        {
            back(sensor_data, 20);
        }
        //turn left
        else if (c == 'a')
        {
            left(sensor_data, 9);
        }
        //turn right
        else if (c == 'd')
        {
            right(sensor_data, 9);
        }
        //scan
        else if (c == 'q')
        {
            scan();
            send_helper("Scan done. Proceed with movement \n\n\r");
        }
        //play music and flash led
        else if (c == 'm')
        {
            music();
            flash();
        }
    }
}
/**
 * This method will be used to scan 180 degree area in front of cybot and get the information of objects.
 * @author Caining Eric Bryan Thein
 * @date 12/8/2017
 */
void scan()
{
    int countdegree = 0;
    int arrayDegrees[90];
    int arrayPing[90];
    int arrayIR[90];
    int i = 0;
    char str[50];
    //scan 180 degree area in front of cybot
    while (countdegree < 180)
    {
        int readingIR = ir_getDistance();
        int readingPing = ping_getDistance();
        if (readingIR <= 70)
        {
            sprintf(str, "\n\rIR(cm): %d    PING(cm): %d      Degree: %d",
                    readingIR, readingPing, countdegree);
            send_helper(str);
        }
        lcd_printf("Scaning...\nIR(cm): %d\nPING(cm): %d\nDegree: %d",
                   readingIR, readingPing, countdegree);

        arrayDegrees[i] = countdegree;
        arrayPing[i] = readingPing;
        arrayIR[i] = readingIR;
        countdegree = countdegree + 2;
        servo_move(countdegree);
        timer_waitMillis(15);
        i++;
    }

    int flag = 0;
    int angularW[10];
    int diss[10];
    int j = 0;
    int degree1;
    int degree2;
    int First_Ping_Dis;
    int Second_Ping_Dis;
    //analysis data and find objects
    for (i = 1; i <= 89; i++)
    {
        if (arrayIR[i - 1] > 70 && arrayIR[i] <= 70 && flag == 0) //hit an object
        {
            j++;
            degree1 = i * 2;
            First_Ping_Dis = arrayPing[i];
            flag = 1;
        }

        if (arrayIR[i - 1] < 70 && arrayIR[i] >= 70 && flag == 1) //moving away from an object
        {
            char buffers[50];
            degree2 = i * 2;
            int center = (i + (degree1 / 2)) / 2;
            diss[j] = arrayPing[center];  //diss is distance in cm from fist edge
            angularW[j] = degree2 - degree1;
            Second_Ping_Dis = arrayPing[i];
            float coss = cos((3.14159 / 180.0) * (double) angularW[j]);

            float Degree = (degree2 - degree1) * 3.1415926536 / 180.0;

            int Size_of_obj = (int) sqrt(
                    (First_Ping_Dis * First_Ping_Dis
                            + Second_Ping_Dis * Second_Ping_Dis)
                            - (2 * First_Ping_Dis * Second_Ping_Dis
                                    * cos(Degree)));
            sprintf(buffers,
                    "Index: %d Dist(cm): %d AngularW: %d WidthCM: %d Degree: %d\n\r",
                    j, diss[j], angularW[j], Size_of_obj,
                    ((degree2 + degree1) / 2));
            send_helper(buffers);
            lcd_printf("Scan done.\n%d objects detected.", j);
            flag = 0;
        }
    }

    servo_move(0);
}
/**
 * This method will be used to flesh power led of cybot.
 * @author Caining Eric Bryan Thein
 * @date 12/8/2017
 */
void flash()
{
    int i = 0;
    for (i = 0; i < 30; i++)
    {
        if (i % 2 == 0)
        {
            oi_setLeds(1, 1, 255, 255);
        }
        else
        {
            oi_setLeds(1, 1, 0, 255);
        }
        timer_waitMillis(200);
    }
}
/**
 * This method will be used to play music on cybot.
 * @author Caining Eric Bryan Thein
 * @date 12/8/2017
 */
void music() {
    unsigned char Darth_Vader_num_note = 19;
    unsigned char Darth_Vader_notes[19] = { 64, 65, 67, 72, 62, 64, 65, 67, 69,
            71, 77, 69, 71, 72, 74, 76, 64, 65, 67 };
    unsigned char Darth_Vader_time_delay[19] = { 33, 11, 25, 45, 33, 11, 50, 33,
            11, 25, 50, 33, 11, 30, 30, 30, 33, 12, 32 };
    oi_loadSong(1, Darth_Vader_num_note, Darth_Vader_notes,
            Darth_Vader_time_delay);
    oi_play_song(1);
    timer_waitMillis(8600);
}
