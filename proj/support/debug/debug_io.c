#include "pico/stdlib.h"

#include "debug_io.h"

typedef enum
{
   DEBUG_IO_PIN_1 = 0,
   DEBUG_IO_PIN_2,
   DEBUG_IO_PIN_3,
   DEBUG_IO_PIN_4,
   DEBUG_IO_PIN_MAX
} debug_io_pin_id_t;

#define DEBUG_IO_SPI_DATA  DEBUG_IO_PIN_3  // SPI MOSI
#define DEBUG_IO_SPI_CLK   DEBUG_IO_PIN_4  // SPI CLK

static uint8_t s_debug_io_pin_gpio[DEBUG_IO_PIN_MAX] =
{
   10, // DEBUG_IO_PIN_1, GP10, Pin14
   11, // DEBUG_IO_PIN_2, GP11, Pin15
   12, // DEBUG_IO_PIN_3, GP12, Pin16
   13, // DEBUG_IO_PIN_4, GP13, Pin17
};


///////////////////////////////////////////////////////////////////////////

void debug_io_init()
{
   // Initialize all debug pins as GPIO, outputs, all off
   for (uint8_t i=0; i < DEBUG_IO_PIN_MAX; i++)
   {
      gpio_init(s_debug_io_pin_gpio[i]);           // GPIO
      gpio_set_dir(s_debug_io_pin_gpio[i], true);  // Output
      gpio_put(s_debug_io_pin_gpio[i], false);     // Clear / off
   }

}

///////////////////////////////////////////////////////////////////////////

void debug_io_pin(uint8_t count_1, uint8_t count_2, uint8_t count_3, uint8_t count_4)
{
   while (0 != count_1)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_1], true);
      sleep_us(1);
      count_1--;
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_1], false);
      sleep_us(1);
   }

   while (0 != count_2)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_2], true);
      sleep_us(1);
      count_2--;
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_2], false);
      sleep_us(1);
   }

   while (0 != count_3)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_3], true);
      sleep_us(1);
      count_3--;
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_3], false);
      sleep_us(1);
   }

   while (0 != count_4)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_4], true);
      sleep_us(1);
      count_4--;
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_4], false);
      sleep_us(1);
   }
}

///////////////////////////////////////////////////////////////////////////

void debug_io_pin_on(bool pin_1, bool pin_2, bool pin_3, bool pin_4)
{
   if (true == pin_1)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_1], true);
   }

   if (true == pin_2)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_2], true);
   }

   if (true == pin_3)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_3], true);
   }

   if (true == pin_4)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_4], true);
   }
}

///////////////////////////////////////////////////////////////////////////

void debug_io_pin_off(bool pin_1, bool pin_2, bool pin_3, bool pin_4)
{
   if (true == pin_1)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_1], false);
   }

   if (true == pin_2)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_2], false);
   }

   if (true == pin_3)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_3], false);
   }

   if (true == pin_4)
   {
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_PIN_4], false);
   }
}

///////////////////////////////////////////////////////////////////////////

void debug_io_code(uint8_t code)
{
   // SPI: 8bit, MSb first, CPOL=0, CPHA=0
   uint8_t clk = 8U;

   while (0 != clk)
   {
      // CLK low
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_SPI_CLK], false);
      sleep_us(1);

      // DATA
      if (code & 0x80)
      {
         gpio_put(s_debug_io_pin_gpio[DEBUG_IO_SPI_DATA], true);
      }
      else
      {
         gpio_put(s_debug_io_pin_gpio[DEBUG_IO_SPI_DATA], false);
      }

      code = (uint8_t)(code << 1U);

      // CLK high
      gpio_put(s_debug_io_pin_gpio[DEBUG_IO_SPI_CLK], true);
      sleep_us(1);
      clk--;
   }

   gpio_put(s_debug_io_pin_gpio[DEBUG_IO_SPI_CLK], false);
   gpio_put(s_debug_io_pin_gpio[DEBUG_IO_SPI_DATA], false);
}
