#include <stdio.h>

#include "btstack.h"

#include "ble_support.h"
#include "debug_io.h"

///////////////////////////////////////////////////////////////////////////

#define BLE_SUPPORT_INQUIRY_INTERVAL  (5)

enum STATE {STATE_INIT, STATE_ACTIVE} ;
enum STATE g_state = STATE_INIT;

static btstack_packet_callback_registration_t g_hci_event_callback_registration;

///////////////////////////////////////////////////////////////////////////

static void ble_support_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
static void ble_support_start_scan();

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

   // TBD: below
   if (true == status)
   {
      printf("JOE: hci_set_inquiry_mode\n");
      hci_set_inquiry_mode(INQUIRY_MODE_RSSI);

      g_hci_event_callback_registration.callback = &ble_support_packet_handler;
      hci_add_event_handler(&g_hci_event_callback_registration);

      printf("JOE: hci_power_control\n");
      if (0 != hci_power_control(HCI_POWER_ON))
      {
         status = false;
      }
   }

   return status;
}

///////////////////////////////////////////////////////////////////////////

static void ble_support_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
   if (packet_type != HCI_EVENT_PACKET)
   {
      printf("BLE-SUPP: *** ERR: Unknown packet_type\n");
      return;
   }

   bd_addr_t bd_addr = {0, 0, 0, 0, 0, 0};
   uint32_t cod = 0;
   int8_t rssi = 0;

   uint8_t event = hci_event_packet_get_type(packet);

   printf("BLE-SUPP: event=0x%02x\n", event);

   switch(g_state)
   {
      // In STATE_INIT: inquiry scan is started, and transits to STATE_ACTIVE
      case STATE_INIT:
         switch(event)
         {
            case BTSTACK_EVENT_STATE:
               uint8_t hci_state =  btstack_event_state_get_state(packet);
               printf("BLE-SUPP: hci_state=0x%02x\n", hci_state);
               if (HCI_STATE_WORKING == hci_state)
               {
                  ble_support_start_scan();
                  g_state = STATE_ACTIVE;
               }
               break;

            default:
               break;
         }
         break;

      // In STATE_ACTIVE: Report inquiry results and restart scan when needed
      case STATE_ACTIVE:
         switch(event)
         {
            case GAP_EVENT_INQUIRY_RESULT:
            {
               // Retrieve information from inquiry result
               gap_event_inquiry_result_get_bd_addr(packet, bd_addr);
               cod = gap_event_inquiry_result_get_class_of_device(packet);
               if (0 != gap_event_inquiry_result_get_rssi_available(packet))
               {
                  rssi = (int8_t)gap_event_inquiry_result_get_rssi(packet);
               }

               // Report information
               printf("BLE-SUPP: Found device => bdaddr=%s, cod=0x%06x, rssi=%d\n",
                      bd_addr_to_str(bd_addr),
                      cod,
                      rssi);
            }
            break;

            case GAP_EVENT_INQUIRY_COMPLETE:
            {
               ble_support_start_scan();
            }
            break;

            default:
               break;
         }
         break;

      default:
         break;
   }
}

///////////////////////////////////////////////////////////////////////////

static void ble_support_start_scan()
{
   printf("BLE_SUPP: Starting inquiry scan..\n");
   gap_inquiry_start(BLE_SUPPORT_INQUIRY_INTERVAL);
}
