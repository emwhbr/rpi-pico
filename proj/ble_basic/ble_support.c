#include <stdio.h>

#include "ble_support.h"
#include "debug_io.h"

///////////////////////////////////////////////////////////////////////////

bool ble_support_init(void)
{
   bool status = true;

   // Initialize CYW43439
   printf("Initialize CYW43\n");
   debug_io_pin_on(false, true, false, false); // DEBUG_IO_PIN_2: high during init
   if (0 != cyw43_arch_init())
   {
      status = false;
   }
   debug_io_pin_off(false, true, false, false);


   return status;
}
