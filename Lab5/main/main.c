#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "time.h"
#include "freertos/timers.h"

#define NUM_TIMERS 2
#define TIMER1_MAXCYCLE 10
#define TIMER2_MAXCYCLE 5
#define TIMER_1_ID 10
#define TIMER_2_ID 20

TimerHandle_t xTimersList[NUM_TIMERS];

void timerCallback(TimerHandle_t xTimers)
{
    static uint32_t timer1CallCount = 0;
    static uint32_t timer2CallCount = 0;
    if ((uint32_t)pvTimerGetTimerID(xTimers) == TIMER_1_ID)
    {
        if (timer1CallCount < TIMER1_MAXCYCLE - 1)
            xTimerReset(xTimers, 0);
        printf("ahihi\n");
        timer1CallCount++;
        
    }
    else if ((uint32_t)pvTimerGetTimerID(xTimers) == TIMER_2_ID)
    {
        if (timer2CallCount <  TIMER2_MAXCYCLE - 1)
            xTimerReset(xTimers, 0);
        printf("ihaha\n");
        timer2CallCount++;

    }
    
}

void createTimers()
{
    xTimersList[0] = xTimerCreate("Timer_1", pdMS_TO_TICKS(2000), false, (void *)TIMER_1_ID, timerCallback);
    xTimersList[1] = xTimerCreate("Timer_2", pdMS_TO_TICKS(3000), false, (void *)TIMER_2_ID, timerCallback);
    for (int i = 0; i < NUM_TIMERS; i++)
    {
        if (xTimersList[i] != NULL)
        {
            if (xTimerStart(xTimersList[i], 0) != pdPASS)
            {
                printf("Can not start timer!");
            }
        }
        else printf("Can not create timer!");
    }
}
void app_main(void)
{
    createTimers();
}