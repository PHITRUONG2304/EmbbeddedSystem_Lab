#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOSConfig.h"
#include <string.h>

uint32_t ulIdleCycleCount = 0UL;

void task1Function(){
    // const TickType_t xDelay250ms = 250*portTICK_PERIOD_MS;
    while(1){
        printf("Task 1 is running with higher priority\n");
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}

void task2Function(void *pvParameters){
    while(1){
        printf("Task 2 is running with lower priority and ulIdleCycleCount= %d\r\n", ulIdleCycleCount);
        // vTaskDelay(pdMS_TO_TICKS(3000));
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

void app_main(void)
{
    xTaskCreate(task2Function, "Task2", 2048, NULL, 2, NULL);
    xTaskCreate(task1Function, "Task1", 2048, NULL, 3, NULL);

    vTaskStartScheduler();
    // not reach
}

void vApplicationIdleHook( void )
{
/* This hook function does nothing but increment a counter . */
    ulIdleCycleCount++;
}
void vApplicationTickHook( void )
{
/* This hook function does nothing but increment a counter . */
}
