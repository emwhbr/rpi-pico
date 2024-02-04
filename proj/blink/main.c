#include <stdio.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

int main()
{
   // Initialize chosen serial port
   stdio_init_all();

   // Initialize CYW43439
   if (0 != cyw43_arch_init())
   {
      while (true)
      {
         printf("*** ERROR: CYW43439 init failed\n");
         sleep_ms(1000);
      }
    }

   // Loop forever
   uint32_t loop_counter = 0;
   while (true)
   {
      // Blink LED
      printf("Blink LED %u\n", loop_counter++);
      cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
      sleep_ms(1000);
      cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
      sleep_ms(1000);
   }
}
