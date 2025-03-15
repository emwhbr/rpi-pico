#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"


#include "debug_io.h"

///////////////////////////////////////////////////////////////////////////

#define USR_LED_ON()   cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1)
#define USR_LED_OFF()  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0)

#define TASK_NAME_LED "LED"
#define TASK_NAME_BLE "BLE"


///////////////////////////////////////////////////////////////////////////

TaskHandle_t g_led_task_handle;
TaskHandle_t g_ble_task_handle;

///////////////////////////////////////////////////////////////////////////

static void led_task_entry(void* arg)
{
   // Loop forever
   while (true)
   {
      // Blink LED
      USR_LED_ON();
      vTaskDelay(pdMS_TO_TICKS(100));
      USR_LED_OFF();
      vTaskDelay(pdMS_TO_TICKS(900));
   }
}

///////////////////////////////////////////////////////////////////////////

static void ble_task_entry(void* arg)
{
   // Initialize CYW43439
   printf("Initialize CYW43\n");
   debug_io_pin_on(false, true, false, false); // DEBUG_IO_PIN_2: high during init
   if (0 != cyw43_arch_init())
   {
      while (true)
      {
         printf("*** ERROR: CYW43 init failed\n");
         vTaskDelay(pdMS_TO_TICKS(1000));
      }
   }
   debug_io_pin_off(false, true, false, false); 

   // Loop forever
   uint32_t loop_counter = 0;
   while (true)
   {
      printf("Task-%s: Alive %u\n", TASK_NAME_BLE, loop_counter++);
      vTaskDelay(pdMS_TO_TICKS(1000));
   }
}

///////////////////////////////////////////////////////////////////////////

int main()
{
   // Initialize chosen serial port
   stdio_init_all();

   // Initialize the project support library
   debug_io_init();
   debug_io_pin(4, 2, 0, 0);                   // Toggle all debug pins
   debug_io_code(0x55);                        // DEBUG_IO_PIN_3 & 4: SPI debug code
   debug_io_pin_on(true, false, false, false); // DEBUG_IO_PIN_1: high during startup

   BaseType_t status = pdPASS;

   // Create the task that will blink the LED
   if (pdPASS == status)
   {
      status = xTaskCreate(led_task_entry, 
                           TASK_NAME_LED, 
                           512,
                           NULL, 
                           1, 
                           &g_led_task_handle);
   }
   printf("Create task: %s [%s]\n", TASK_NAME_LED, status ? "OK" : "FAIL");

   // Create the task that will handle BLE
   if (pdPASS == status)
   {
      status = xTaskCreate(ble_task_entry, 
                           TASK_NAME_BLE, 
                           1024,
                           NULL,
                           2,
                           &g_ble_task_handle);
   }
   printf("Create task: %s [%s]\n", TASK_NAME_BLE, status ? "OK" : "FAIL");

   if (pdPASS != status)
   {
      while (true)
      {
         printf("*** ERROR: FreeRTOS create task failed\n");
         sleep_ms(1000);
      }
   }

   // Start the scheduler
   printf("Start FreeRTOS scheduler\n");
   debug_io_pin_off(true, false, false, false);
   vTaskStartScheduler();

   // Will not get here unless there is insufficient RAM
   while (true)
   {
      printf("*** ERROR: FreeRTOS start scheduler failed\n");
      sleep_ms(1000);
   }
}
