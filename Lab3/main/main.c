#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "time.h"

volatile uint32_t task1Counter = 0UL;
volatile uint32_t task2Counter = 0UL;
volatile uint32_t task3Counter = 0UL;


void task1Function( void *pvParameters )
{
	printf("Task 1 is started\n");
	int64_t start = esp_timer_get_time();
	while(1){
		task1Counter++;
		if (task1Counter >= 2000000) {
			int64_t end = esp_timer_get_time();
			printf("Task 1 during: %lld ms\n", (end - start)/1000);
			printf("----------task 1 end------------\n");
			vTaskDelete(NULL);
		}
	}
}

void task2Function( void *pvParameters )
{
	printf("Task 2 is started\n");
	int64_t start = esp_timer_get_time();
	while(1){
		task2Counter++;
		if (task2Counter >= 5000000) {
			int64_t end = esp_timer_get_time();
			printf("Task 2 during: %lld ms\n", (end - start)/1000);
			printf("----------task 2 end------------\n");
			vTaskDelete(NULL);
		}
	}
}

void task3Function( void *pvParameters )
{
	printf("Task 3 is started\n");
	int64_t start = esp_timer_get_time();
	while(1){
		task3Counter++;
		if (task3Counter >= 5000000) {
			int64_t end = esp_timer_get_time();
			printf("Task 3 during: %lld ms\n", (end - start)/1000);
			printf("----------task 3 end------------\n");
			vTaskDelete(NULL);
		}
	}
}


void app_main(void)
{
	// Exercise 1
	// Prioritized Pre-emptive Scheduling (with Time Slicing) use configUSE_TIME_SLICING=1 in FreeRTOS.h
	// xTaskCreatePinnedToCore(&task3Function, "Task 3", 8192, NULL, tskIDLE_PRIORITY + 1, NULL, 0);
	// xTaskCreatePinnedToCore(&task2Function, "Task 2", 8192, NULL, tskIDLE_PRIORITY + 1, NULL, 0);
	// vTaskDelay(pdMS_TO_TICKS(20));
	// xTaskCreatePinnedToCore(&task1Function, "Task 1", 8192, NULL, tskIDLE_PRIORITY + 3, NULL, 0);

	// Exercise 2
	// // Prioritized Pre-emptive Scheduling (without Time Slicing) use configUSE_TIME_SLICING=0 in FreeRTOS.h
	// xTaskCreatePinnedToCore(&task3Function, "Task 3", 8192, NULL, tskIDLE_PRIORITY + 1, NULL, 0);
	// xTaskCreatePinnedToCore(&task2Function, "Task 2", 8192, NULL, tskIDLE_PRIORITY + 1, NULL, 0);
	// vTaskDelay(pdMS_TO_TICKS(20));
	// xTaskCreatePinnedToCore(&task1Function, "Task 1", 8192, NULL, tskIDLE_PRIORITY + 3, NULL, 0);

	// Exercise 3
	// Co-operative Scheduling use configUSE_PREEMPTION=0 in FreeRTOSConfig.h
	xTaskCreatePinnedToCore(&task3Function, "Task 3", 8192, NULL, tskIDLE_PRIORITY + 1, NULL, 0);
	vTaskDelay(pdMS_TO_TICKS(10));
	xTaskCreatePinnedToCore(&task2Function, "Task 2", 8192, NULL, tskIDLE_PRIORITY + 2, NULL, 0);
	vTaskDelay(pdMS_TO_TICKS(20));
	xTaskCreatePinnedToCore(&task1Function, "Task 1", 8192, NULL, tskIDLE_PRIORITY + 3, NULL, 0);
}
