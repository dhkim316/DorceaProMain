/***************************************************************************//**
 * @file
 * @brief Simple Button Driver Instances
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "sl_simple_button.h"
#include "sl_simple_button_intensity_config.h"
#include "sl_simple_button_power_config.h"
#include "sl_simple_button_startStop_config.h"

sl_simple_button_context_t simple_intensity_context = {
  .state = 0,
  .history = 0,
  .port = SL_SIMPLE_BUTTON_INTENSITY_PORT,
  .pin = SL_SIMPLE_BUTTON_INTENSITY_PIN,
  .mode = SL_SIMPLE_BUTTON_INTENSITY_MODE,
};

const sl_button_t sl_button_intensity = {
  .context = &simple_intensity_context,
  .init = sl_simple_button_init,
  .get_state = sl_simple_button_get_state,
  .poll = sl_simple_button_poll_step,
  .enable = sl_simple_button_enable,
  .disable = sl_simple_button_disable,
};
sl_simple_button_context_t simple_power_context = {
  .state = 0,
  .history = 0,
  .port = SL_SIMPLE_BUTTON_POWER_PORT,
  .pin = SL_SIMPLE_BUTTON_POWER_PIN,
  .mode = SL_SIMPLE_BUTTON_POWER_MODE,
};

const sl_button_t sl_button_power = {
  .context = &simple_power_context,
  .init = sl_simple_button_init,
  .get_state = sl_simple_button_get_state,
  .poll = sl_simple_button_poll_step,
  .enable = sl_simple_button_enable,
  .disable = sl_simple_button_disable,
};
sl_simple_button_context_t simple_startstop_context = {
  .state = 0,
  .history = 0,
  .port = SL_SIMPLE_BUTTON_STARTSTOP_PORT,
  .pin = SL_SIMPLE_BUTTON_STARTSTOP_PIN,
  .mode = SL_SIMPLE_BUTTON_STARTSTOP_MODE,
};

const sl_button_t sl_button_startstop = {
  .context = &simple_startstop_context,
  .init = sl_simple_button_init,
  .get_state = sl_simple_button_get_state,
  .poll = sl_simple_button_poll_step,
  .enable = sl_simple_button_enable,
  .disable = sl_simple_button_disable,
};

// the table of buttons and button count are generated as a
// convenience for the application
const sl_button_t *sl_simple_button_array[] = {
  &sl_button_intensity, 
  &sl_button_power, 
  &sl_button_startstop
};
const uint8_t simple_button_count = 3;

void sl_simple_button_init_instances(void)
{
  sl_button_init(&sl_button_intensity);
  sl_button_init(&sl_button_power);
  sl_button_init(&sl_button_startstop);
}

void sl_simple_button_poll_instances(void)
{
  sl_button_poll_step(&sl_button_intensity);
  sl_button_poll_step(&sl_button_power);
  sl_button_poll_step(&sl_button_startstop);
}
