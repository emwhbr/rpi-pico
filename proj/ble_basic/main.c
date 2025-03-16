#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

#include "pico/stdlib.h"

#include "debug_io.h"
#include "ble_support.h"

///////////////////////////////////////////////////////////////////////////

#define TASK_NAME_LED "LED"
#define TASK_NAME_BLE "BLE"

#define TASK_CORE_AFFINITY_MASK_LED  ((UBaseType_t)2)  // Core1
#define TASK_CORE_AFFINITY_MASK_BLE  ((UBaseType_t)1)  // Core0

///////////////////////////////////////////////////////////////////////////

TaskHandle_t g_led_task_handle;
TaskHandle_t g_ble_task_handle;

///////////////////////////////////////////////////////////////////////////

static void led_task_entry(void* arg)
{
   printf("Task-%s: Started, core %u\n", TASK_NAME_LED, get_core_num());

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
   printf("Task-%s: Started, core %u\n", TASK_NAME_BLE, get_core_num());

   if (false == ble_support_init())
   {
      while (true)
      {
         printf("*** ERROR: BLE init failed\n");
         vTaskDelay(pdMS_TO_TICKS(5000));
      }
   }

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
   vTaskCoreAffinitySet(g_led_task_handle, TASK_CORE_AFFINITY_MASK_LED);
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
   vTaskCoreAffinitySet(g_ble_task_handle, TASK_CORE_AFFINITY_MASK_BLE);
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
