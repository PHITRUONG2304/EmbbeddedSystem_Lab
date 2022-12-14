#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "time.h"

enum TaskType{t01, t02, t03};

struct DataContainer_t{
    enum TaskType task_ID;
    uint8_t dataValue_ldu8;
    uint32_t duration;
};

static xQueueHandle dataQueue = NULL;
bool isTask1Running = false;
bool isTask2Running = false;
bool isTask3Running = false;


void receptionTask();
void functionalTask_1();
void functionalTask_2();
void functionalTask_3();
void controller();

struct DataContainer_t getTask();

void app_main(void)
{
    dataQueue = xQueueCreate(3, sizeof(struct DataContainer_t));
    if (dataQueue == 0) // if there is some error while creating queue
        printf("Unable to create STRUCTURE Queue\n\n");
    else
        printf("STRUCTURE Queue created successfully\n\n");
    
    xTaskCreate(receptionTask, "Reception Task", 56000, NULL, 3, NULL);
    xTaskCreate(controller, "Controller", 56000, NULL, 3, NULL);

}


void receptionTask(){
    while(1){
        struct DataContainer_t dataToSend = getTask();
        const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
        // if TickType_t xTicksToWait=portMAX_DELAY the system block if the queue is already full.
        // TickType_t xTicksToWait=0 the system doesn't block if the queue is already full.

        if(xQueueSend(dataQueue, &dataToSend, ( TickType_t ) 0) == pdPASS)
			printf("Successfully add request to the queue\n");
        else {
            printf("Failed add request to the queue\n");
        }
        vTaskDelay(xDelay);
    }
}


struct DataContainer_t getTask(){
    struct DataContainer_t dataToSend;
    dataToSend.task_ID = rand()%3;
    dataToSend.dataValue_ldu8 = rand()%255;
    dataToSend.duration = rand()%20 * 500000; 
    return dataToSend;
}

void controller(){
    while(1){
        if((uint8_t)uxQueueMessagesWaiting(dataQueue) > 0){
            struct DataContainer_t dataToReceive;
            xQueuePeek(dataQueue, &dataToReceive, portMAX_DELAY);

            switch(dataToReceive.task_ID)
            {
            case t01:
                if(!isTask1Running){
                    xQueueReceive(dataQueue, &dataToReceive, portMAX_DELAY);
                    xTaskCreate(functionalTask_1, "Functional Task 1", 8192, (void *) &dataToReceive, 2, NULL);
                }
                break;

            case t02:
                if(!isTask2Running){
                    xQueueReceive(dataQueue, &dataToReceive, portMAX_DELAY);
                    xTaskCreate(functionalTask_2, "Functional Task 2", 8192, (void *) &dataToReceive, 2, NULL);
                }
                break;

            case t03:
                if(!isTask3Running){
                    xQueueReceive(dataQueue, &dataToReceive, portMAX_DELAY);
                    xTaskCreate(functionalTask_3, "Functional Task 3", 8192, (void *) &dataToReceive, 2, NULL);
                }
                break;

            default:
                xQueueReceive(dataQueue, &dataToReceive, portMAX_DELAY);
                printf("No functional task receives the request\n\n");
                vTaskDelay(pdMS_TO_TICKS(INT64_MAX));
                break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void functionalTask_1(void *parameter){

    struct DataContainer_t *pcTaskParameter;
    pcTaskParameter = (struct DataContainer_t *) parameter;

    printf("Task 1 is started\n");
    isTask1Running = true;
	int64_t start = esp_timer_get_time();
    uint32_t counter = 0;
	while(1){
		counter++;
		if (counter >= pcTaskParameter->duration) {
			int64_t end = esp_timer_get_time();
			// printf("Task 1 during: %lld ms\n", (end - start)/1000);
			printf("----------Task 1 end, and executed in about: %lldms------------\n\n", (end - start)/1000);
            isTask1Running = false;
			vTaskDelete(NULL);
		}
	}

}

void functionalTask_2(void *parameter){

    struct DataContainer_t *pcTaskParameter;
    pcTaskParameter = (struct DataContainer_t *) parameter;

    printf("Task 2 is started\n");
    isTask2Running = true;
	int64_t start = esp_timer_get_time();
    uint32_t counter = 0;
	while(1){
		counter++;
		if (counter >= pcTaskParameter->duration) {
			int64_t end = esp_timer_get_time();
			// printf("Task 1 during: %lld ms\n", (end - start)/1000);
			printf("----------Task 2 end, and executed in about: %lldms------------\n\n", (end - start)/1000);
            isTask2Running = false;
            vTaskDelete(NULL);
		}
	}

}

void functionalTask_3(void *parameter){

    struct DataContainer_t *pcTaskParameter;
    pcTaskParameter = (struct DataContainer_t *) parameter;

    printf("Task 3 is started\n");
    isTask3Running = true;
	int64_t start = esp_timer_get_time();
    uint32_t counter = 0;
	while(1){
		counter++;
		if (counter >= pcTaskParameter->duration) {
			int64_t end = esp_timer_get_time();
			// printf("Task 1 during: %lld ms\n", (end - start)/1000);
			printf("----------Task 3 end, and executed in about: %lldms------------\n\n", (end - start)/1000);
            isTask3Running = false;
			vTaskDelete(NULL);
		}
	}

}
