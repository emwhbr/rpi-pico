#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "debug_io.h"


#define USR_LED_ON()   cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1)
#define USR_LED_OFF()  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0)

///////////////////////////////////////////////////////////////////////////

static void led_task_entry(void* arg)
{
   debug_io_pin_off(false, true, false, false); //  DEBUG_IO_PIN_2: task started

   // Loop forever
   uint32_t loop_counter = 0;
   while (true)
   {
      // Blink LED
      printf("Task: Blink LED %u\n", loop_counter++);
      debug_io_code((uint8_t)loop_counter);       // DEBUG_IO_PIN_3 & 4: SPI debug code
      debug_io_pin_on(true, false, false, false); // DEBUG_IO_PIN_1: high durin LED on
      USR_LED_ON();
      vTaskDelay(pdMS_TO_TICKS(500));
      debug_io_pin_off(true, false, false, false);
      debug_io_pin_on(false, true, false, false); // DEBUG_IO_PIN_2: hih during LED off
      USR_LED_OFF();
      vTaskDelay(pdMS_TO_TICKS(500));
      debug_io_pin_off(false, true, false, false);
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

   // Initialize CYW43439
   printf("\nInitialize CYW43439\n");
   if (0 != cyw43_arch_init())
   {
      while (true)
      {
         printf("*** ERROR: CYW43439 init failed\n");
         sleep_ms(1000);
      }
   }

   USR_LED_ON();
   sleep_ms(100);
   USR_LED_OFF();

   // Create the task that will blink the LED
   printf("Create LED task\n");
   TaskHandle_t led_task_handle;
   BaseType_t status = xTaskCreate(led_task_entry, 
                                   "LED", 
                                   512, 
                                   NULL, 
                                   1, 
                                   &led_task_handle);
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
   debug_io_pin_off(true, false, false, false); // DEBUG_IO_PIN_1: startup done
   debug_io_pin_on(false, true, false, false);  // DEBUG_IO_PIN_2: high until task starts
   vTaskStartScheduler();

   // Will not get here unless there is insufficient RAM
   while (true)
   {
      printf("*** ERROR: FreeRTOS start scheduler failed\n");
      sleep_ms(1000);
   }
}
