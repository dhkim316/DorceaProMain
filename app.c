/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
//tbr -- to be remove
//mbr -- must be remove
#include "em_common.h"
#include "app_assert.h"
#include "app_log.h"
#include "sl_bluetooth.h"
#include "gatt_db.h"
#include "app.h"
#include "sl_simple_button_instances.h"
#include "sl_simple_led_instances.h"
#include "em_gpio.h"
#include "sl_pwm_instances.h"

#define sl_app_log app_log_info
// #define app_log app_log_info
// #define sl_app_log_stats app_log_info

//for multi connect topology
static uint8_t advertising_set_handle = 0xff;

static const char string_central[] = "CENTRAL";
static const char string_peripheral[] = "PERIPHERAL";

static uint8_t connecting_handle = 0x00;
static bd_addr new_device_id = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

static uint32_t serviceHandle = 0xFFFFFFFF;
static uint16_t characteristicHandle = 0xFFFF;
static bool connecting = false;
static bool enabling_indications = false;
static bool discovering_service = false;
static bool discovering_characteristic = false;
static uint8_t numOfActiveConn = 0; // number of active connections <= MAX_CONNECTIONS
static device_info_t device_list[MAX_CONNECTIONS];

sl_bt_gap_phy_type_t scanning_phy = sl_bt_gap_1m_phy;
sl_bt_gap_phy_type_t connection_phy = sl_bt_gap_1m_phy;
uint8_t dev_index;

sl_sleeptimer_timer_handle_t msTimer;

/***************************************************************************************************
 Local Macros and Definitions
 **************************************************************************************************/
/* Set here the operation mode of the SPP device: */
#define SPP_SERVER_MODE 1
#define SPP_CLIENT_MODE 0

// #define SPP_OPERATION_MODE SPP_SERVER_MODE
#define SPP_OPERATION_MODE SPP_CLIENT_MODE

/*Main states */
#define DISCONNECTED  0
#define SCANNING      1
#define FIND_SERVICE  2
#define FIND_CHAR     3
#define ENABLE_NOTIF  4
#define DATA_MODE     5
#define DISCONNECTING 6

#define STATE_ADVERTISING 1
#define STATE_CONNECTED   2
#define STATE_SPP_MODE    3

// SPP service UUID: 4880c12c-fdcb-4077-8920-a450d7f9b907
const uint8_t serviceUUID[16] = {0x07,
                                 0xb9,
                                 0xf9,
                                 0xd7,
                                 0x50,
                                 0xa4,
                                 0x20,
                                 0x89,
                                 0x77,
                                 0x40,
                                 0xcb,
                                 0xfd,
                                 0x2c,
                                 0xc1,
                                 0x80,
                                 0x48};

// SPP data UUID: fec26ec4-6d71-4442-9f81-55bc21d658d6
const uint8_t charUUID[16] = {0xd6,
                              0x58,
                              0xd6,
                              0x21,
                              0xbc,
                              0x55,
                              0x81,
                              0x9f,
                              0x42,
                              0x44,
                              0x71,
                              0x6d,
                              0xc4,
                              0x6e,
                              0xc2,
                              0xfe};


/* maximum number of iterations when polling UART RX data before sending data over BLE connection
 * set value to 0 to disable optimization -> minimum latency but may decrease throughput */
#define UART_POLL_TIMEOUT  5000

/*Bookkeeping struct for storing amount of received/sent data  */
typedef struct
{
  uint32_t num_pack_sent;
  uint32_t num_bytes_sent;
  uint32_t num_pack_received;
  uint32_t num_bytes_received;
  uint32_t num_writes; /* Total number of send attempts */
} tsCounters;

/* Function is only used in SPP client mode */
#if (SPP_OPERATION_MODE == SPP_CLIENT_MODE)
static bool process_scan_response(sl_bt_evt_scanner_scan_report_t *pResp);
#endif

/* Common local functions*/
static void printStats(tsCounters *psCounters);
static void reset_variables();
static void send_spp_data();

/***************************************************************************************************
 Local Variables
 **************************************************************************************************/
#if (SPP_OPERATION_MODE == SPP_SERVER_MODE)
// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;
#endif

static uint8_t _conn_handle = 0xFF;
static uint8_t _main_state;
static uint32_t _service_handle;
static uint16_t _char_handle;

tsCounters _sCounters;

/* Default maximum packet size is 20 bytes. This is adjusted after connection is opened based
 * on the connection parameters */
static uint8_t _max_packet_size = 20;
static uint8_t _min_packet_size = 20;  // Target minimum bytes for one packet

static void reset_variables()
{
  _conn_handle = 0xFF;
  _main_state = STATE_ADVERTISING;
  _service_handle = 0;
  _char_handle = 0;
  _max_packet_size = 20;

  memset(&_sCounters, 0, sizeof(_sCounters));
}

// #define SWCLK_ENABLE

void swclkEnable(uint8_t enableDisable) 
{
  #ifdef SWCLK_ENABLE
    GPIO_DbgSWDClkEnable(1);  //ENABLE SWCLK
  #else
    GPIO_DbgSWDClkEnable(enableDisable);  //ENABLE SWCLK
  #endif
}
// GPIO_DbgSWDClkEnable(0);  //DISABLE SWCLK
// GPIO_DbgSWDClkEnable(1);  //ENABLE SWCLK

/**************************************************************************
 Application Init code
 ***************************************************************************/
SL_WEAK void app_init(void)
{
  GPIO_PinModeSet(gpioPortB, 4, gpioModePushPull, 1); //power on;
  GPIO_PinModeSet(gpioPortB, 3, gpioModeInputPull,1); //power sw detect

  GPIO_PinModeSet(gpioPortC, 1, gpioModeInputPull,1); //pull up RXD

  // GPIO_DbgSWDClkEnable(0);  //DISABLE SWCLK
  GPIO_PinModeSet(gpioPortA, 1, gpioModeInputPull,1); //Button
  GPIO_PinModeSet(gpioPortA, 2, gpioModeInputPull,1); //Button


  //intensity led
  GPIO_PinModeSet(gpioPortB, 0, gpioModePushPull, 0); 
  GPIO_PinModeSet(gpioPortB, 1, gpioModePushPull, 0); 
  GPIO_PinModeSet(gpioPortB, 2, gpioModePushPull, 0); 

  //time led
  GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 0); 
  GPIO_PinModeSet(gpioPortA, 4, gpioModePushPull, 0); 
  GPIO_PinModeSet(gpioPortA, 5, gpioModePushPull, 0); 

  GPIO_PinModeSet(gpioPortA, 6, gpioModePushPull, 0); 
  GPIO_PinModeSet(gpioPortA, 7, gpioModePushPull, 0); 
  GPIO_PinModeSet(gpioPortA, 8, gpioModePushPull, 0); 
  GPIO_PinModeSet(gpioPortD, 1, gpioModePushPull, 0); 
  GPIO_PinModeSet(gpioPortD, 2, gpioModePushPull, 0); 
  GPIO_PinModeSet(gpioPortD, 3, gpioModePushPull, 0); 

  //POGO POWER CONTROL
  GPIO_PinModeSet(gpioPortC, 6, gpioModePushPull, 1); 
  GPIO_PinModeSet(gpioPortC, 5, gpioModePushPull, 1); 
  GPIO_PinModeSet(gpioPortC, 4, gpioModePushPull, 1); 
  GPIO_PinModeSet(gpioPortC, 3, gpioModePushPull, 1); 

  selectIntensity(mIntensityBtn.index, MAX_DIMMING_DUTY);
  calculateTimeStep();
  displayTimeStep(op.timeStep);
  sl_sleeptimer_start_periodic_timer(&msTimer, sl_sleeptimer_ms_to_tick(1), timerCallBack, (void *)NULL, 0, 0);
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  // if(STATE_SPP_MODE == _main_state){
  //   send_spp_data();
  // }
  if (sl_button_get_state(&sl_button_power) == SL_SIMPLE_BUTTON_PRESSED) {
    // GPIO_DbgSWDClkEnable(1);  //enable SWCLK
    swclkEnable(1);
  }
  else {
    swclkEnable(0);
    // GPIO_DbgSWDClkEnable(0);  //DISABLE SWCLK
  }
}

#if (SPP_OPERATION_MODE == SPP_SERVER_MODE)
/**************************************************************************//**
 * Bluetooth stack event handler for SPP Server mode
 *
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  uint16_t max_mtu_out;

  switch (SL_BT_MSG_ID(evt->header)) {

    case sl_bt_evt_system_boot_id:
      sl_app_log("SPP Role: SPP Server\r\n");
      reset_variables();
      sl_bt_gatt_server_set_max_mtu(247, &max_mtu_out);
      sl_bt_advertiser_create_set(&advertising_set_handle);
      sl_bt_advertiser_set_timing( advertising_set_handle,
                                160, // min. adv. interval (milliseconds * 1.6)
                                160, // max. adv. interval (milliseconds * 1.6)
                                0,   // adv. duration
                                0);  // max. num. adv. events

      // Start  advertising and enable connections
      sl_bt_advertiser_start(advertising_set_handle,
                                  advertiser_general_discoverable,
                                  advertiser_connectable_scannable);
      break;

    case sl_bt_evt_connection_opened_id:
      _conn_handle = evt->data.evt_connection_opened.connection;
      sl_app_log("Connection opened\r\n");
      _main_state = STATE_CONNECTED;

      /* Request connection parameter update.
       * conn.interval min 20ms, max 40ms, slave latency 4 intervals,
       * supervision timeout 2 seconds
       * (These should be compliant with Apple Bluetooth Accessory Design Guidelines, both R7 and R8) */
      sl_bt_connection_set_parameters(_conn_handle, 24, 40, 0, 200, 0, 0xFFFF);
      break;

    case sl_bt_evt_connection_parameters_id:
      sl_app_log("Conn.parameters: interval %u units, txsize %u\r\n", evt->data.evt_connection_parameters.interval, evt->data.evt_connection_parameters.txsize);
      break;

    case sl_bt_evt_gatt_mtu_exchanged_id:
      /* Calculate maximum data per one notification / write-without-response, this depends on the MTU.
       * up to ATT_MTU-3 bytes can be sent at once  */
      _max_packet_size = evt->data.evt_gatt_mtu_exchanged.mtu - 3;
      _min_packet_size = _max_packet_size; /* Try to send maximum length packets whenever possible */
      sl_app_log("MTU exchanged: %d\r\n", evt->data.evt_gatt_mtu_exchanged.mtu);
      break;

    case sl_bt_evt_connection_closed_id:
      printStats(&_sCounters);
      if (STATE_SPP_MODE == _main_state){
         sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
      }
      reset_variables();
      // Restart advertising after client has disconnected.
      sl_bt_advertiser_start(advertising_set_handle,
                                  advertiser_general_discoverable,
                                  advertiser_connectable_scannable);
      break;

    case sl_bt_evt_gatt_server_characteristic_status_id:
      {
        sl_bt_evt_gatt_server_characteristic_status_t char_status;
        char_status = evt->data.evt_gatt_server_characteristic_status;

        if (char_status.characteristic == gattdb_spp_data) {
          if (char_status.status_flags == gatt_server_client_config) {
            // Characteristic client configuration (CCC) for spp_data has been changed
            if (char_status.client_config_flags == gatt_notification) {
              _main_state = STATE_SPP_MODE;
              sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
              sl_app_log("SPP Mode ON\r\n");
              }
            else {
              sl_app_log("SPP Mode OFF\r\n");
              _main_state = STATE_CONNECTED;
              sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
              }
            }
          }
      }
      break;

    case sl_bt_evt_gatt_server_attribute_value_id:
    {
       for(uint8_t i = 0; i < evt->data.evt_gatt_server_attribute_value.value.len; i++) {
           sl_iostream_putchar(sl_iostream_vcom_handle, evt->data.evt_gatt_server_attribute_value.value.data[i]);
       }
       _sCounters.num_pack_received++;
       _sCounters.num_bytes_received += evt->data.evt_gatt_server_attribute_value.value.len;
    }
    break;

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}

#else
/**************************************************************************//**
 * Bluetooth stack event handler for SPP Client mode
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  uint16_t max_mtu_out;
  sl_status_t status, sc;

  switch (SL_BT_MSG_ID(evt->header)) {
    case sl_bt_evt_system_boot_id:
      sl_app_log("SPP Role: SPP client\r\n");
      reset_variables();
      if(sl_button_get_state(&sl_button_startstop) == SL_SIMPLE_BUTTON_PRESSED) {
        sc = sl_bt_sm_delete_bondings();
        app_assert_status(sc);
      }
#if 1
      // Bonding configuration
      sc = sl_bt_sm_configure(0x0E, sl_bt_sm_io_capability_noinputnooutput);
      app_assert_status(sc);

      uint32_t bc = getBondingCount();

      if(bc < 4) sc = sl_bt_sm_set_bondable_mode(1);
      else      sc = sl_bt_sm_set_bondable_mode(0);
      app_assert_status(sc);
      app_log("bonding Count = %d\r\n", bc);
      // Maximum allowed bonding count: 4
    // New bonding will overwrite the bonding that was used the longest time ago
      
      sc = sl_bt_sm_store_bonding_configuration(4, 0x2);
      app_assert_status(sc);
#endif

      sl_bt_gatt_server_set_max_mtu(247, &max_mtu_out);
      sc = sl_bt_scanner_start(sl_bt_gap_1m_phy, sl_bt_scanner_discover_generic);
      app_assert_status_f(sc, "Failed to start discovery #1\n");

      _main_state = SCANNING;

      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      // Set advertising interval to 100ms.
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        160, // min. adv. interval (milliseconds * 1.6)
        160, // max. adv. interval (milliseconds * 1.6)
        0,   // adv. duration
        0);  // max. num. adv. events
      app_assert_status(sc);
      // Start general advertising and enable connections.
      sc = sl_bt_advertiser_start(
        advertising_set_handle,
        advertiser_general_discoverable,
        advertiser_connectable_scannable);
      app_assert_status(sc);
      break;

#if 1
    case sl_bt_evt_sm_confirm_bonding_id:
      sl_app_log("Bonding confirm\r\n");
      sc = sl_bt_sm_bonding_confirm(_conn_handle, 1);
      app_assert_status(sc);
      break;

    // Event raised when bonding is successful
    case sl_bt_evt_sm_bonded_id:
      sl_app_log("Bonded\r\n\n");
      break;

    // Event raised when bonding failed
    case sl_bt_evt_sm_bonding_failed_id:
      sl_app_log("Bonding failed\r\n\n");
      // sc = sl_bt_sm_increase_security(_conn_handle);
      // app_assert_status(sc);
      break;
#endif

    case sl_bt_evt_scanner_scan_report_id:
      /* Exit event if max connection is reached */
      if (numOfActiveConn == MAX_CONNECTIONS)
        break;

      /* Exit if device is in connection process (processing another scan response),
       * or service, or characterstics discovery */
      if (connecting || enabling_indications || discovering_service || discovering_characteristic)
        break;

      /* Exit event if service is not in the scan response*/
      if(!process_scan_response(&evt->data.evt_scanner_scan_report))
        break;

      /* Exit event if the scan response is triggered by a device already in the connection list. */
      if (found_device(evt->data.evt_scanner_scan_report.address))
        break;

      /* Max connection isn't reached, device is not in a connection process, new SPP service is found.
       * Continue ...*/

      /* Initiate connection */
      if(is_device_bonded(evt->data.evt_scanner_scan_report.address.addr) || (getBondingCount() < 4)) {
        connecting = true;
        sc = sl_bt_connection_open(evt->data.evt_scanner_scan_report.address, evt->data.evt_scanner_scan_report.address_type, connection_phy, &connecting_handle);
        app_assert_status(sc);
      }
      else
        return;

      /* Update device list. If connection doesn't succeed (due to timeout) the device will be removed from the list in connection closed event handler*/
      device_list[numOfActiveConn].address = evt->data.evt_scanner_scan_report.address;
      device_list[numOfActiveConn].address_type = evt->data.evt_scanner_scan_report.address_type;
      device_list[numOfActiveConn].conn_handle = connecting_handle;
      _conn_handle = connecting_handle;
      // device_list[numOfActiveConn].characteristicHandle = characteristicHandle;
      device_list[numOfActiveConn].conn_role = CR_PERIPHERAL; // connection role of the remote device
      device_list[numOfActiveConn].conn_state = CS_CONNECTING;
      /* Increment numOfActiveConn */
      numOfActiveConn++;

      break;

    case sl_bt_evt_connection_opened_id:
      sl_app_log("Connection opened!\r\n");
      if(evt->data.evt_connection_opened.master == CR_CENTRAL) {
        _main_state = FIND_SERVICE;

        // sc = sl_bt_gatt_discover_primary_services_by_uuid(_conn_handle, 16, serviceUUID);
        sc = sl_bt_gatt_discover_primary_services_by_uuid(evt->data.evt_connection_opened.connection, 16, serviceUUID);
        app_assert_status(sc);

        discovering_service = true;
        /* connection process completed. */
        connecting = false;

        /* Update device connection state. common for both master and slave roles*/
        device_list[numOfActiveConn - 1].conn_state = CS_CONNECTING;

        /* Advertising stops when connection is opened. Re-start advertising */
        if (numOfActiveConn == MAX_CONNECTIONS){
            app_log("Maximum number of allowed connections reached.\r\n");
            app_log("Stop scanning but continue advertising in non-connectable mode.\r\n");
            sc = sl_bt_scanner_stop();
            app_assert_status(sc);
        }
      }
      break;

    case sl_bt_evt_connection_closed_id:

      app_log("\r\nCONNECTION CLOSED \r\n");
      // handle of the closed connection
      uint8_t closed_handle = evt->data.evt_connection_closed.connection;
      dev_index = get_dev_index(closed_handle);
      app_log("Device ");
      print_bd_addr(device_list[dev_index].address);
      app_log(" left the connection::0x%04X\r\n", evt->data.evt_connection_closed.reason);

      uint8_t i;
      for (i = dev_index; i < numOfActiveConn - 1; i++){
          device_list[i] = device_list[i+1];
      }

      if (numOfActiveConn > 0){
          numOfActiveConn--;
      }

      // print list of remaining connections
      sl_app_log_stats();

      app_log("Number of active connections ...: %d\r\n", numOfActiveConn);
      app_log("Available connections ..........: %d\r\n", MAX_CONNECTIONS - numOfActiveConn);

      /* If we have one less available connection than the maximum allowed...*/
      if (numOfActiveConn == MAX_CONNECTIONS - 1){
          // start scanning,
          sc = sl_bt_scanner_start(gap_1m_phy, scanner_discover_generic);
          app_assert_status_f(sc, "Failed to start discovery #1\n");
          app_log("Scanning restarted.\r\n");
      }
      break;

    case sl_bt_evt_connection_parameters_id:
      //  sl_app_log("Conn.parameters: interval %u units, txsize %u\r\n",
      //                             evt->data.evt_connection_parameters.interval,
      //                             evt->data.evt_connection_parameters.txsize);
      dev_index = get_dev_index(evt->data.evt_connection_parameters.connection);
      device_list[dev_index].conn_state = CS_CONNECTED;

      /* If new connection is not reported ... */
      if(memcmp(&new_device_id, &device_list[dev_index].address,  sizeof(bd_addr)) !=0){

          memcpy(&new_device_id, &device_list[dev_index].address, sizeof(bd_addr));
          app_log("\r\nNEW CONNECTION ESTABLISHED \r\n");
          app_log("Device ID .................: ");
          print_bd_addr(device_list[numOfActiveConn-1].address);
          app_log("\r\n");
          app_log("Role ......................: %s\r\n", (device_list[dev_index].conn_role == CR_PERIPHERAL) ? string_peripheral : string_central);
          app_log("Handle ....................: %d\r\n", device_list[dev_index].conn_handle);
          app_log("Number of connected devices: %d\r\n", numOfActiveConn);
          app_log("Available connections .....: %d\r\n", MAX_CONNECTIONS - numOfActiveConn);

          /* Print connection summary*/
          sl_app_log_stats();

      }

      break;

    case sl_bt_evt_gatt_mtu_exchanged_id:
       /* Calculate maximum data per one notification / write-without-response, this depends on the MTU.
        * up to ATT_MTU-3 bytes can be sent at once  */
       _max_packet_size = evt->data.evt_gatt_mtu_exchanged.mtu - 3;
       _min_packet_size = _max_packet_size; /* Try to send maximum length packets whenever possible */
       sl_app_log("MTU exchanged: %d\r\n", evt->data.evt_gatt_mtu_exchanged.mtu);
       break;

    case sl_bt_evt_gatt_service_id:
      if (evt->data.evt_gatt_service.uuid.len == 16) {
          if (memcmp(serviceUUID, evt->data.evt_gatt_service.uuid.data, 16) == 0) {
            sl_app_log("Service discovered\r\n");
            _service_handle = evt->data.evt_gatt_service.service; // tbr
            serviceHandle = evt->data.evt_gatt_service.service;
          }
        }
      break;

    case sl_bt_evt_gatt_procedure_completed_id:
          /* if service discovery completed */
          if (discovering_service) {
            discovering_service = false;
            sc = sl_bt_gatt_discover_characteristics(evt->data.evt_gatt_procedure_completed.connection, serviceHandle);
            app_assert_status(sc);
            discovering_characteristic = true;
          }
          /* if characteristic discovery completed */
          else if (discovering_characteristic) {
            discovering_characteristic = false;
            sc = sl_bt_gatt_set_characteristic_notification(evt->data.evt_gatt_procedure_completed.connection, characteristicHandle, gatt_indication);
            app_assert_status(sc);
            enabling_indications = true;
            //for bonding
            sc = sl_bt_sm_increase_security(_conn_handle);
            app_assert_status(sc);

          }
          else if (enabling_indications) {
            enabling_indications = 0;
          }
        break;

      case sl_bt_evt_gatt_characteristic_id:
        if (evt->data.evt_gatt_characteristic.uuid.len == 16) {
          if (memcmp(charUUID, evt->data.evt_gatt_characteristic.uuid.data, 16) == 0) {
            sl_app_log("Char discovered\r\n");
            // _char_handle = evt->data.evt_gatt_characteristic.characteristic;  //tbr
            characteristicHandle = evt->data.evt_gatt_characteristic.characteristic;
            device_list[numOfActiveConn-1].characteristicHandle = characteristicHandle; //dhkim
            // sl_app_log_stats();
          }
        }
        break;

      case sl_bt_evt_gatt_characteristic_value_id:
        if (evt->data.evt_gatt_characteristic_value.characteristic == _char_handle) {
           for(uint8_t i = 0; i < evt->data.evt_gatt_server_attribute_value.value.len; i++) {
               sl_iostream_putchar(sl_iostream_vcom_handle, evt->data.evt_gatt_server_attribute_value.value.data[i]);
           }
           _sCounters.num_pack_received++;
           _sCounters.num_bytes_received += evt->data.evt_gatt_server_attribute_value.value.len;
        }
        break;
    case  sl_bt_evt_system_external_signal_id:
    {
      switch(evt->data.evt_system_external_signal.extsignals) {
        case SIGNAL_POWER_BUTTON_PRESSED:
          // GPIO_DbgSWDClkEnable(1);  //enable SWCLK
          swclkEnable(1);
          op.remainingTime = 0;
          mStartStopBtn.index = 0;
          sendStartStop();
          displayTimeStep(0);
          selectIntensity(99,MIN_DIMMING_DUTY);
          GPIO_PinModeSet(gpioPortB, 4, gpioModePushPull, 0); //power off;
          while(sl_button_get_state(&sl_button_power) == SL_SIMPLE_BUTTON_PRESSED);
          while(1);
        break;
        case SIGNAL_INTENSITY_BUTTON_PRESSED:
          sendIntensity();
          selectIntensity(mIntensityBtn.index, MAX_DIMMING_DUTY);
          char s[2];
          s[0] = 'c'+mIntensityBtn.index;
          s[1] = 0;
          sound(s);

        break;
        case SIGNAL_STARTSTOP_BUTTON_PRESSED:
          sendStartStop();
          startStop(NULL);
          if(op.playStop==1) {
            dimmingTimeStep(NULL, 1);
            dimmingIntensity(NULL,1);
            inSound();
          }
          else {
            displayTimeStep(op.timeStep);
            selectIntensity(mIntensityBtn.index, MAX_DIMMING_DUTY);
            outSound();
          }
        break;
        case SIGNAL_STOP_SONIC:
          mStartStopBtn.index = 0;
          sendStartStop();
          outSound();

        break;
        case SIGNAL_PLAY_SONIC:
          sendStartStop();
        break;
      }
    }
      break;
    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}

static bool process_scan_response(sl_bt_evt_scanner_scan_report_t *pResp)
{
  // Decoding advertising packets is done here. The list of AD types can be found
  // at: https://www.bluetooth.com/specifications/assigned-numbers/Generic-Access-Profile

  uint8_t i = 0, ad_len, ad_type;
  bool ad_match_found = false;

  char name[32];

  while (i < (pResp->data.len - 1)) {

    ad_len = pResp->data.data[i];
    ad_type = pResp->data.data[i + 1];

    if (ad_type == 0x08 || ad_type == 0x09) {
      // Type 0x08 = Shortened Local Name
      // Type 0x09 = Complete Local Name
      memcpy(name, &(pResp->data.data[i + 2]), ad_len - 1);
      name[ad_len - 1] = 0;
      // sl_app_log("%s\r\n", name);
    }

    // 4880c12c-fdcb-4077-8920-a450d7f9b907
    if (ad_type == 0x06 || ad_type == 0x07) {
      // Type 0x06 = Incomplete List of 128-bit Service Class UUIDs
      // Type 0x07 = Complete List of 128-bit Service Class UUIDs
      if (memcmp(serviceUUID, &(pResp->data.data[i + 2]), 16) == 0) {
        sl_app_log("Found SPP device\r\n");
        ad_match_found = true;
      }
    }
    // Jump to next AD record
    i = i + ad_len + 1;
  }

  return ad_match_found;
}

#endif //SPP_OPERATION_MODE


static void printStats(tsCounters *psCounters)
{
  sl_app_log("Outgoing data:\r\n");
  sl_app_log(" bytes/packets sent: %lu / %lu ", psCounters->num_bytes_sent, psCounters->num_pack_sent);
  sl_app_log(", num writes: %lu\r\n", psCounters->num_writes);
  sl_app_log("(RX buffer overflow is not tracked)\r\n");
  sl_app_log("Incoming data:\r\n");
  sl_app_log(" bytes/packets received: %lu / %lu\r\n", psCounters->num_bytes_received, psCounters->num_pack_received);

  return;
}

const char cIntensity[3][3] = {"IL\0","IM\0","IH\0"};
const char cStartStop[2][5] = {"STOP\0","PLAY\0"};

void sl_button_on_change(const sl_button_t *handle)
{
  // if(_conn_handle == 0xFF) return;

  if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED){
    if(handle == &sl_button_intensity) {
      intensityBtn(&mIntensityBtn.timer,1);
    }
    if(handle == &sl_button_startstop) {
      startStopBtn(&mStartStopBtn.timer,1);
    }
    if(handle == &sl_button_power) {
      powerBtn(&mPowerBtn.timer,1);
    }
  }
}

static int powerPressedTime=0;
void timerCallBack(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)data;
  // powerBtn(handle,0);
  intensityBtn(handle,0);
  startStopBtn(handle,0);
  startStop(handle);
  dimmingTimeStep(handle, 0);
  dimmingIntensity(handle,0);

  if(handle == &msTimer) {
    sl_simple_button_poll_step(&sl_button_power);
    if(sl_button_get_state(&sl_button_power) == SL_SIMPLE_BUTTON_PRESSED) {
      powerPressedTime++;
      if(powerPressedTime == 2000)
          sl_bt_external_signal(SIGNAL_POWER_BUTTON_PRESSED);
    }
    else {
      powerPressedTime = 0;
    }
  }

  pitchPlay(handle);

}

#define BUTTON_HOLD_TIME 30
Button_t mPowerBtn = {.busy=0, .index=0 };
Button_t mIntensityBtn = {.busy=0, .index=0 };
Button_t mStartStopBtn =  {.busy=0, .index=0 };

void powerBtn(sl_sleeptimer_timer_handle_t *handle, int trigger)
{
  if(handle == &mPowerBtn.timer) {
    if((trigger == 1) && (mPowerBtn.busy==0)) {
      sl_sleeptimer_start_timer(handle, sl_sleeptimer_ms_to_tick(BUTTON_HOLD_TIME), timerCallBack, (void *)NULL, 0, 0);
      mPowerBtn.busy = 1;
    }
    else {
      if(sl_button_get_state(&sl_button_power) == SL_SIMPLE_BUTTON_PRESSED) {
          mPowerBtn.busy = 0;
          sl_bt_external_signal(SIGNAL_POWER_BUTTON_PRESSED);
      }   
    }
  }
}

void intensityBtn(sl_sleeptimer_timer_handle_t *handle, int trigger)
{
  if(sl_button_get_state(&sl_button_power) == SL_SIMPLE_BUTTON_PRESSED) return; //protect power button press

  if(handle == &mIntensityBtn.timer) {
    if((trigger == 1) && (mIntensityBtn.busy==0)) {
      sl_sleeptimer_start_timer(handle, sl_sleeptimer_ms_to_tick(BUTTON_HOLD_TIME), timerCallBack, (void *)NULL, 0, 0);
      mIntensityBtn.busy = 1;
    }
    else if((trigger == 0) && (mIntensityBtn.busy==1)) {
      if(sl_button_get_state(&sl_button_intensity) == SL_SIMPLE_BUTTON_PRESSED) {
          mIntensityBtn.index++;
          mIntensityBtn.index %= 3;
          sl_bt_external_signal(SIGNAL_INTENSITY_BUTTON_PRESSED);
      }   
      mIntensityBtn.busy = 0;
    }
  }
}

void startStopBtn(sl_sleeptimer_timer_handle_t *handle, int trigger)
{
  if(handle == &mStartStopBtn.timer) {
    if((trigger == 1) && (mStartStopBtn.busy==0)) {
      sl_sleeptimer_start_timer(handle, sl_sleeptimer_ms_to_tick(BUTTON_HOLD_TIME), timerCallBack, (void *)NULL, 0, 0);
      mStartStopBtn.busy = 1;
    }
    else if((trigger == 0) && (mStartStopBtn.busy==1)){
      if(sl_button_get_state(&sl_button_startstop) == SL_SIMPLE_BUTTON_PRESSED) {
          sl_bt_external_signal(SIGNAL_STARTSTOP_BUTTON_PRESSED);
          mStartStopBtn.index++;
          mStartStopBtn.index %= 2;
      }   
      mStartStopBtn.busy = 0;
    }
  }
}


/* returns true if the remote device address is found in the list of connected device list */
bool found_device(bd_addr bd_address)
{
  int i;

  for (i = 0; i < numOfActiveConn; i++) {
    if (memcmp(&device_list[i].address, &bd_address, sizeof(bd_addr)) == 0) {
      return true; // Found
    }
  }

  return false; // Not found
}

uint8_t get_dev_index(uint8_t handle)
{
  uint8_t index;

  for (index = 0; index < numOfActiveConn; index++) {
    if (device_list[index].conn_handle == handle) {
      return index;
    }
  }
  return 0xFF;
}

uint32_t getBondingCount(void) {
  uint32_t num_bondings;
  uint8_t bondings[4];
  uint32_t bonding_len;
  sl_bt_sm_get_bonding_handles(0, &num_bondings, 4, &bonding_len, bondings);
  return num_bondings;
}

void sl_app_log_stats(void)
{
  app_log("\r\n--------------- LIST of CONNECTED DEVICES ----------------\r\n");
  app_log("==========================================================\r\n");
  static bool print_header = true;

     //print header
     if(print_header == true){
         app_log("ADDRESS            ROLE          HANDLE        STATE\r\n");
     }
     app_log("==========================================================\r\n");

  int i;
  for (i = 0; i < numOfActiveConn; i++) {

    print_bd_addr(device_list[i].address);
    app_log("  %-14s%-14d%-10s\r\n",
            (device_list[i].conn_role == 0) ? string_peripheral : string_central,
             device_list[i].conn_handle,
             get_conn_state(device_list[i].conn_state));
  }

  app_log("\r\n");
  uint32_t num_bondings;
  bd_addr address;
  uint8_t address_type, security_mode, key_size;

  num_bondings = getBondingCount();
  app_log("Bondings: %d\r\n", num_bondings);
  for(int i=0; i<num_bondings; i++) {
    sl_bt_sm_get_bonding_details(i, &address, &address_type, &security_mode, &key_size);
    app_log("addr%02X%02X%02X%-02X%02X%02X\r\n", address.addr[5], address.addr[4], address.addr[3],address.addr[2], address.addr[1], address.addr[0]);
  }
}

/* print bd_addr */
void print_bd_addr(bd_addr bd_address)
{
  int i;

  for (i = 5; i >= 0; i--) {
    app_log("%02X", bd_address.addr[i]);

    if (i > 0)
      app_log(":");
  }
}

const char* get_conn_state(uint8_t state){
  switch(state){
    case CS_CONNECTED:
      return "CONNECTED";
    case CS_CONNECTING:
      return "CONNECTING";
    case CS_CLOSED:
      return "CLOSED";
    default:
      return "UNKNOWN";
  }
}

void selectIntensity(uint8_t index, uint8_t duty){
  switch(index) {
    case 0:
      sl_pwm_set_duty_cycle(&sl_pwm_intensityLow, duty);
      sl_pwm_start(&sl_pwm_intensityLow);    
      sl_pwm_set_duty_cycle(&sl_pwm_intensityMid, 0);
      sl_pwm_stop(&sl_pwm_intensityMid);    
      sl_pwm_set_duty_cycle(&sl_pwm_intensityHi, 0);
      sl_pwm_stop(&sl_pwm_intensityHi);    
    break;
    case 1:
      sl_pwm_set_duty_cycle(&sl_pwm_intensityLow, 0);
      sl_pwm_stop(&sl_pwm_intensityLow);    
      sl_pwm_set_duty_cycle(&sl_pwm_intensityMid, duty);
      sl_pwm_start(&sl_pwm_intensityMid);    
      sl_pwm_set_duty_cycle(&sl_pwm_intensityHi, 0);
      sl_pwm_stop(&sl_pwm_intensityHi);    
    break;
    case 2:
      sl_pwm_set_duty_cycle(&sl_pwm_intensityLow, 0);
      sl_pwm_stop(&sl_pwm_intensityLow);    
      sl_pwm_set_duty_cycle(&sl_pwm_intensityMid, 0);
      sl_pwm_stop(&sl_pwm_intensityMid);    
      sl_pwm_set_duty_cycle(&sl_pwm_intensityHi, duty);
      sl_pwm_start(&sl_pwm_intensityHi);    
    break;
    case 99:
      sl_pwm_set_duty_cycle(&sl_pwm_intensityLow, 0);
      sl_pwm_stop(&sl_pwm_intensityLow);    
      sl_pwm_set_duty_cycle(&sl_pwm_intensityMid, 0);
      sl_pwm_stop(&sl_pwm_intensityMid);    
      sl_pwm_set_duty_cycle(&sl_pwm_intensityHi, 0);
      sl_pwm_stop(&sl_pwm_intensityHi);    
    break;
  }
}

OpTime_t op = {.playStop=0, .remainingTime = SONIC_MAX_TIME};

void calculateTimeStep(void) {
    int secondsPerStep = SONIC_MAX_TIME / SONIC_TIME_STEP;
    
    int step = op.remainingTime / secondsPerStep;
    if (op.remainingTime % secondsPerStep != 0) {
        step++;
    }
    if(step > 9) step = 9;
    op.timeStep = step;
}

void startStop(sl_sleeptimer_timer_handle_t *handle)
{
  if(mStartStopBtn.index == 0) {  //stop
    sl_sleeptimer_stop_timer(&op.timer1sec); 
    op.playStop = 0;
  }
  else {
    if(op.playStop == 0) {
      sl_sleeptimer_start_periodic_timer(&op.timer1sec, sl_sleeptimer_ms_to_tick(1000), timerCallBack, (void *)NULL, 0, 0);
      op.playStop = 1;
    }
  }
  if(handle == &op.timer1sec) {
    op.remainingTime--;
    if(op.remainingTime <= 0) {
      sl_sleeptimer_stop_timer(&op.timer1sec);
      op.playStop = 0;
      op.remainingTime = SONIC_MAX_TIME;  //RE-FILL THE TIME
      sl_bt_external_signal(SIGNAL_STOP_SONIC);
    }
    else{
      sl_bt_external_signal(SIGNAL_PLAY_SONIC); //계속 동작중임을 알림
    }
    calculateTimeStep();
    displayTimeStep(op.timeStep);
  }
  calculateTimeStep();
}

const sl_pwm_instance_t *sl_pwm_op_time[] = {
  &sl_pwm_op_time1,
  &sl_pwm_op_time2,
  &sl_pwm_op_time3,
  &sl_pwm_op_time4,
  &sl_pwm_op_time5,
  &sl_pwm_op_time6,
  &sl_pwm_op_time7,
  &sl_pwm_op_time8,
  &sl_pwm_op_time9,
};

void clearOpLed(void) {
  for(int i=0; i < 9; i++) {
    sl_pwm_set_duty_cycle(sl_pwm_op_time[i], 0);
    sl_pwm_stop(sl_pwm_op_time[i]);    
  }
}

void setOpLed(int step) {
  for(int i=0; i < step; i++) {
    sl_pwm_set_duty_cycle(sl_pwm_op_time[i], 50);
    sl_pwm_start(sl_pwm_op_time[i]);    
  }
}

void displayTimeStep(int step)
{
  clearOpLed();
  if(step) setOpLed(step);
}

void sendIntensity(void)
{
  int sent_len;
  for(int i=0; i < 4; i++)
    sl_bt_gatt_write_characteristic_value_without_response(device_list[i].conn_handle, device_list[i].characteristicHandle, 3, &cIntensity[mIntensityBtn.index], &sent_len);
}

void sendStartStop(void)
{
  int sent_len;
  for(int i=0; i < 4; i++)
    sl_bt_gatt_write_characteristic_value_without_response(device_list[i].conn_handle, device_list[i].characteristicHandle, 5, &cStartStop[mStartStopBtn.index], &sent_len);
}

void setOpLedWithDuty(int step, uint8_t duty) {
  for(int i=0; i < step; i++) {
    sl_pwm_set_duty_cycle(sl_pwm_op_time[i], duty);
    sl_pwm_start(sl_pwm_op_time[i]);    
  }
}

Dimming_t mDimmingTime = {.upDown=1, .duty=0};

void dimmingTimeStep(sl_sleeptimer_timer_handle_t *handle, int trigger)
{
  if(op.playStop == 0) {
    sl_sleeptimer_stop_timer(&mDimmingTime.timer); 
  }
  else {
    if (trigger) {  //timer start
      sl_sleeptimer_start_periodic_timer(&mDimmingTime.timer, sl_sleeptimer_ms_to_tick(DIMMING_STEP), timerCallBack, (void *)NULL, 0, 0);
    }
    else {
      if(handle == &mDimmingTime.timer) {
        if(mDimmingTime.upDown==1) {
          mDimmingTime.duty++;
          if(mDimmingTime.duty >= MAX_DIMMING_DUTY) {
            mDimmingTime.duty = MAX_DIMMING_DUTY;
            mDimmingTime.upDown = 0;
          }
        }
        else {
          mDimmingTime.duty--;
          if(mDimmingTime.duty <= MIN_DIMMING_DUTY) {
            mDimmingTime.duty = MIN_DIMMING_DUTY;
            mDimmingTime.upDown = 1;
          }
        }
        setOpLedWithDuty(op.timeStep, mDimmingTime.duty);
      }
    }
  }
}

Dimming_t mDimmingIntensity = {.upDown=1, .duty=0};

void dimmingIntensity(sl_sleeptimer_timer_handle_t *handle, int trigger)
{
  if(op.playStop == 0) {
    sl_sleeptimer_stop_timer(&mDimmingIntensity.timer); 
  }
  else {
    if (trigger) {  //timer start
      sl_sleeptimer_start_periodic_timer(&mDimmingIntensity.timer, sl_sleeptimer_ms_to_tick(DIMMING_STEP), timerCallBack, (void *)NULL, 0, 0);
    }
    else {
      if(handle == &mDimmingIntensity.timer) {
        if(mDimmingIntensity.upDown==1) {
          mDimmingIntensity.duty++;
          if(mDimmingIntensity.duty >= MAX_DIMMING_DUTY) {
            mDimmingIntensity.duty = MAX_DIMMING_DUTY;
            mDimmingIntensity.upDown = 0;
          }
        }
        else {
          mDimmingIntensity.duty--;
          if(mDimmingIntensity.duty <= MIN_DIMMING_DUTY) {
            mDimmingIntensity.duty = MIN_DIMMING_DUTY;
            mDimmingIntensity.upDown = 1;
          }
        }
        selectIntensity(mIntensityBtn.index, mDimmingIntensity.duty);
      }
    }
  }
}

int is_device_bonded(unsigned char* device_addr) {
    unsigned char bonded_devices[MAX_BONDED_DEVICES][ADDR_BYTE_LENGTH];
    int num_bonded_devices = get_bonded_devices(bonded_devices);

    for (int j = 0; j < num_bonded_devices; j++) {
        if (memcmp(device_addr, bonded_devices[j], ADDR_BYTE_LENGTH) == 0) {
            return 1; // 주어진 디바이스 주소가 본딩 목록에 있음
        }
    }

    return 0; // 주어진 디바이스 주소가 본딩 목록에 없음
}

int get_bonded_devices(unsigned char devices[][ADDR_BYTE_LENGTH]) {
  uint32_t num_bondings;
  uint8_t address_type, security_mode, key_size;

  memset(devices, 0, sizeof(devices[0]) * MAX_BONDED_DEVICES);
  // ... (디바이스 목록 가져오기)

  num_bondings = getBondingCount();

  for(int i=0; i<num_bondings; i++) {
    sl_bt_sm_get_bonding_details(i, &devices[i], &address_type, &security_mode, &key_size);
  }
  return num_bondings;
}

Melody_t mMelody;

// const int cFreq[16] = {1047, 1175, 1319, 1397,1568,1760,1980,2093,0,};
//                     a     b     c     d     e     f     g     h(C) 
const unsigned int cFreq[16] = {1760, 1980, 1047, 1175, 1319, 1397, 1568, 2093,0,};
#include "sl_pwm_init_buzzer_config.h"
void pitchPlay(sl_sleeptimer_timer_handle_t *handle)
{
  sl_pwm_config_t pwm_buzzer_config = {
    .frequency = cFreq[mMelody.pitch[mMelody.index]-'a'],
    .polarity = SL_PWM_BUZZER_POLARITY,
  };

  if(handle == &mMelody.timer) {
    if(mMelody.index != 0) sl_pwm_stop(&sl_pwm_buzzer);
    if(mMelody.index < mMelody.len) {
      sl_pwm_init(&sl_pwm_buzzer, &pwm_buzzer_config);
      sl_pwm_set_duty_cycle(&sl_pwm_buzzer, 50);
      sl_pwm_start(&sl_pwm_buzzer);
      sl_sleeptimer_start_timer(&mMelody.timer, sl_sleeptimer_ms_to_tick(mMelody.holdTime[mMelody.index]), timerCallBack, (void *)NULL, 0, 0);
      mMelody.index++;
    }
  }
}

void pitchSet(const char *p, unsigned int holdTime)
{
  int len = 0;
  memset(mMelody.pitch, 0, 16);
  for(int i=0; *p != 0; i++) {
    mMelody.pitch[i] = *p;
    mMelody.holdTime[i] = holdTime;
    p++;
    len++;
  }
  mMelody.len = len;
  mMelody.index = 0;
}

void inSound(void) {
  pitchSet("cde", 100);
  pitchPlay(&mMelody.timer);
}

void outSound(void) {
  pitchSet("edc", 100);
  pitchPlay(&mMelody.timer);
}

void sound(char *s)
{
  pitchSet(s, 150);
  pitchPlay(&mMelody.timer);
}
