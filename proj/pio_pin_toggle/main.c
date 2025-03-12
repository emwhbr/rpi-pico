#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "debug_io.h"
#include "pin_toggle.pio.h"

///////////////////////////////////////////////////////////////////////////

#define USR_LED_ON()   cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1)
#define USR_LED_OFF()  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0)

#define TASK_NAME_LED "LED"
#define TASK_NAME_PIO "PIO"

#define PIO_ID        pio0
#define PIO_SM        0
#define PIO_GPIO_PIN  14  // GP14



///////////////////////////////////////////////////////////////////////////

TaskHandle_t g_led_task_handle;
TaskHandle_t g_pio_task_handle;


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

static void pio_task_entry(void* arg)
{
   PIO  pio = PIO_ID;
   uint sm  = PIO_SM;

   int rc = 0;

   // Load the program into the PIO's instruction memory
   int imOffset = pio_add_program(pio, &pin_toggle_program);
   printf("Task-%s: imOffset=%d\n", TASK_NAME_PIO, imOffset);

   // Get state machine config, function is auto generated in "pin_toggle.pio.h"
   pio_sm_config smCfg = pin_toggle_program_get_default_config(imOffset);

   // Configure GPIO to be controlled by PIO
   pio_gpio_init(pio, PIO_GPIO_PIN);

   // Connect pin to SET pin (control with 'set' instruction in PIO program)
   sm_config_set_set_pins(&smCfg, PIO_GPIO_PIN, 1);

   // Configure state machine to run at sysclk/125 --> 1MHz, one cycle=1us
   sm_config_set_clkdiv(&smCfg, 125.0f);

   // Start the state machine
   rc = pio_sm_init(pio, sm, imOffset, &smCfg);
   if (PICO_OK == rc)
   {
      pio_sm_set_enabled(pio, sm, true);
   }
   printf("Task-%s: PIO state machine started[%s]\n", TASK_NAME_PIO, rc == PICO_OK ? "OK" : "FAIL");

   // Loop forever
   uint32_t loop_counter = 0;
   while (true)
   {
      printf("Task-%s: Alive %u\n", TASK_NAME_PIO, loop_counter++);
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

   // Create the task that will control PIO
   if (pdPASS == status)
   {
      status = xTaskCreate(pio_task_entry, 
                           TASK_NAME_PIO, 
                           512, 
                           NULL, 
                           1, 
                           &g_pio_task_handle);
   }
   printf("Create task: %s [%s]\n", TASK_NAME_PIO, status ? "OK" : "FAIL");

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
   vTaskStartScheduler();

   // Will not get here unless there is insufficient RAM
   while (true)
   {
      printf("*** ERROR: FreeRTOS start scheduler failed\n");
      sleep_ms(1000);
   }
}
