#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "sdkconfig.h"

TaskHandle_t  idleTask;
TaskHandle_t  task2;

void task1Function( void *pvParameters )
{
	while(1){
		printf("---------------------Task 1 is running!------------------\n");
		vTaskDelay(10);

		printf("---------------------Task 1 is running!------------------\n");
		vTaskDelay(100);
	}
}

void task2Function( void *pvParameters )
{
	while(1){
		printf("Task 2 is running!\n");
	}
}

void idleTaskFunction( void *pvParameters )
{
	while(1){
		printf("Idle task is running!\n");
	}
}


void app_main(void)
{
    xTaskCreate(task2Function, "Task 2", 8192, NULL, 0, NULL);
    xTaskCreate(idleTaskFunction, "Idle task", 8192, NULL, 0, NULL);

	vTaskDelay(100);
    xTaskCreate(task1Function, "Task 1", 4096, NULL, 3, NULL);
}
