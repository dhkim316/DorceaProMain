/***************************************************************************//**
 * @file
 * @brief Application interface provided to main().
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

#ifndef APP_H
#define APP_H
#include "sl_sleeptimer.h"
#include "stdint.h"
#include "sl_bgapi.h"

/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
void app_init(void);

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
void app_process_action(void);

extern const char cIntensity[3][3];
extern const char cStartStop[2][5];

typedef struct {
    unsigned char busy;
    unsigned char index;
    sl_sleeptimer_timer_handle_t timer;
    sl_sleeptimer_timer_handle_t longTimer;
} Button_t;

extern Button_t mPowerBtn;
extern Button_t mIntensityBtn;
extern Button_t mStartStopBtn;

#define SIGNAL_POWER_BUTTON_PRESSED         70
#define SIGNAL_INTENSITY_BUTTON_PRESSED     71
#define SIGNAL_STARTSTOP_BUTTON_PRESSED     72

#define SIGNAL_STOP_SONIC 73
#define SIGNAL_PLAY_SONIC 74

void powerBtn(sl_sleeptimer_timer_handle_t *handle, int trigger);
void intensityBtn(sl_sleeptimer_timer_handle_t *handle, int trigger);
void startStopBtn(sl_sleeptimer_timer_handle_t *handle, int trigger);

typedef enum
{
  // Connection States (CS)
  CS_CONNECTED,
  CS_CONNECTING,
  CS_CLOSED
} conn_state_t;

/*------------CHANGE THE ABOVE ENUM USING THIS ONE-----------------*/
typedef enum {
  scanning,
  opening,
  discover_services,
  discover_characteristics,
  enable_indication,
  running
} conn_state;
/*----------------------------------------------------------------*/

typedef enum
{
  // Connection Roles (CR)
  CR_PERIPHERAL,
  CR_CENTRAL
} conn_role_t;

/* Struct to store the connecting device address, our device role in the connection, and connection state*/
typedef struct
{
  bd_addr address;
  uint8_t address_type;
  conn_role_t conn_role;
  conn_state_t conn_state;
  uint8_t conn_handle;
  uint16_t characteristicHandle;
} device_info_t;

#define MAX_CONNECTIONS    4

bool found_device(bd_addr bd_address);
uint8_t get_dev_index(uint8_t handle);
void print_bd_addr(bd_addr bd_address);
const char* get_conn_state(uint8_t state);

void sl_app_log_stats(void);
void selectIntensity(uint8_t index, uint8_t duty);

typedef struct {
    unsigned char playStop;
    int remainingTime;
    int timeStep;
    sl_sleeptimer_timer_handle_t timer1sec;
} OpTime_t;

extern OpTime_t op;

#define SONIC_MAX_TIME 180 //second
#define SONIC_TIME_STEP 9

void startStop(sl_sleeptimer_timer_handle_t *handle);

void clearOpLed(void);
void setOpLed(int step);
void calculateTimeStep(void);
void displayTimeStep(int step);

void timerCallBack(sl_sleeptimer_timer_handle_t *handle, void *data);

void sendIntensity(void);
void sendStartStop(void);

typedef struct {
    unsigned char upDown;
    int duty;
    sl_sleeptimer_timer_handle_t timer;
} Dimming_t;
#define DIMMING_STEP 10 //ms
#define MAX_DIMMING_DUTY 50
#define MIN_DIMMING_DUTY 0

void dimmingTimeStep(sl_sleeptimer_timer_handle_t *handle, int trigger);
void dimmingIntensity(sl_sleeptimer_timer_handle_t *handle, int trigger);

uint32_t getBondingCount(void);

#define MAX_BONDED_DEVICES 4
#define ADDR_BYTE_LENGTH 6

int is_device_bonded(unsigned char* device_addr);
int get_bonded_devices(unsigned char devices[][ADDR_BYTE_LENGTH]);

typedef struct {
    char pitch[16];
    unsigned int holdTime[16];
    unsigned char len;
    unsigned char index;
    sl_sleeptimer_timer_handle_t timer;
} Melody_t;

extern Melody_t mMelody;

void inSound(void);
void outSound(void);

void sound(char *s);

#endif // APP_H
