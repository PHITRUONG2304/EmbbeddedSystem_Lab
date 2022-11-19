#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "sdkconfig.h"

TaskHandle_t idleTask;
TaskHandle_t task2;

void task1Function( void *pvParameters )
{
	while(1){
		vTaskDelay(130);
		printf("---------------------Task 1 is running!------------------\n");
	}
}

void task2Function( void *pvParameters )
{
	while(1){
		printf("Task 2 is running!\n");
		vTaskResume( idleTask );
		vTaskSuspend( task2 );
	}
}

void idleTaskFunction( void *pvParameters )
{
	while(1){
		printf("Idle task is running!\n");
		vTaskResume( task2 );
		vTaskSuspend( idleTask );
	}
}


void app_main(void)
{
    xTaskCreate(task1Function, "Task 1", 4096, NULL, 2, NULL);

    xTaskCreate(idleTaskFunction, "Idle task", 4096, NULL, 0, &idleTask);
    xTaskCreate(task2Function, "Task 2", 4096, NULL, 0, &task2);

    vTaskSuspend( task2 );

//    vTaskStartScheduler();
}
