// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "driver/gpio.h"

// #define LED_GPIO GPIO_NUM_2
// #define BUTTON_GPIO GPIO_NUM_17


// void printID(void *pvParameter)
// {
// 	while(1)
// 	{
// 	    printf("1915749\n");
// 	    vTaskDelay(1000/portTICK_RATE_MS);
// 	}
// }

// void button_task(void *pvParameter)
// {
//     gpio_pad_select_gpio(BUTTON_GPIO);

//     gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
//     gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
//     bool release = true;
//     while(1) {
//         if(gpio_get_level(BUTTON_GPIO) == 0) {
//             if(release){
//                 printf("ESP32\n");
//                 release = false;
//             }
//         }
//         else{
//             release = true;
//         }
//         vTaskDelay(10 / portTICK_RATE_MS);
//     }
// }

// void app_main()
// {
//     xTaskCreate(&printID, "ID", 2048, NULL, 5, NULL);
//     xTaskCreate(&button_task, "button_task", 2048, NULL, 5, NULL);
// }

/* Include library */
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define cyclic_task_PRIO    configMAX_PRIORITIES - 1
#define acyclic_task_PRIO     configMAX_PRIORITIES - 1

#define BTN_PRESSED         0
#define BTN_NOT_PRESSED     1
#define BUTTON_GPIO GPIO_NUM_17

#define READ_BTN_PIN_LEVEL() gpio_get_level(BUTTON_GPIO)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void printMSSV(void * pvParameters);
static void handleButtonTask(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
void app_main()
{
	
	xTaskCreate(printMSSV, "print_MSSV_Task", configMINIMAL_STACK_SIZE + 10, NULL, cyclic_task_PRIO, NULL);
	xTaskCreate(handleButtonTask, "handle_Button_Task", configMINIMAL_STACK_SIZE + 10, NULL, acyclic_task_PRIO, NULL);

	// start the scheduler
	vTaskStartScheduler();
	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for (;;);
}

static void printMSSV( void * pvParameters )
{
    TickType_t last_wake_time;
	//Set interval to 1s
	const TickType_t interval_1s = 1000/portTICK_PERIOD_MS;

    // Initialize the last_wake_time variable with the current time
	last_wake_time = xTaskGetTickCount();
    for( ;; )
    {
        vTaskDelayUntil(&last_wake_time, interval_1s);
        printf("1913600\n");
    }
}

/*!
 * @brief Task responsible for reading a button state (0 or 1).
 * 		  The task prints "ESP32" if button is pressed
 */
static void handleButtonTask(void *pvParameters)
{
	TickType_t last_wake_time;
	uint8_t curr_state, prev_state = BTN_NOT_PRESSED; //State for the button
	//Set interval to 10ms
	const TickType_t read_input_interval = 10/portTICK_PERIOD_MS;

	// Initialize the last_wake_time variable with the current time
	last_wake_time = xTaskGetTickCount();

	//Set up pin gpio
	gpio_pad_select_gpio(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);

	for( ;; )
	{
		// Wait for the next cycle.
		vTaskDelayUntil(&last_wake_time, read_input_interval);
		// Get the level on button pin
		curr_state = READ_BTN_PIN_LEVEL();
		if ((curr_state == BTN_PRESSED) && (prev_state == BTN_NOT_PRESSED)) {
			printf("ESP32\n");
		}
		prev_state = curr_state;
	}

}