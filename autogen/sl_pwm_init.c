/***************************************************************************//**
 * @file
 * @brief PWM Driver Instance Initialization
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include "sl_pwm.h"

#include "sl_pwm_init_buzzer_config.h"

#include "sl_pwm_init_intensityHi_config.h"

#include "sl_pwm_init_intensityLow_config.h"

#include "sl_pwm_init_intensityMid_config.h"

#include "sl_pwm_init_op_time1_config.h"

#include "sl_pwm_init_op_time2_config.h"

#include "sl_pwm_init_op_time3_config.h"

#include "sl_pwm_init_op_time4_config.h"

#include "sl_pwm_init_op_time5_config.h"

#include "sl_pwm_init_op_time6_config.h"

#include "sl_pwm_init_op_time7_config.h"

#include "sl_pwm_init_op_time8_config.h"

#include "sl_pwm_init_op_time9_config.h"


#include "em_gpio.h"


sl_pwm_instance_t sl_pwm_buzzer = {
  .timer = SL_PWM_BUZZER_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_BUZZER_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_BUZZER_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_BUZZER_OUTPUT_PIN),
#if defined(SL_PWM_BUZZER_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_BUZZER_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_intensityHi = {
  .timer = SL_PWM_INTENSITYHI_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_INTENSITYHI_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_INTENSITYHI_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_INTENSITYHI_OUTPUT_PIN),
#if defined(SL_PWM_INTENSITYHI_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_INTENSITYHI_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_intensityLow = {
  .timer = SL_PWM_INTENSITYLOW_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_INTENSITYLOW_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_INTENSITYLOW_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_INTENSITYLOW_OUTPUT_PIN),
#if defined(SL_PWM_INTENSITYLOW_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_INTENSITYLOW_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_intensityMid = {
  .timer = SL_PWM_INTENSITYMID_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_INTENSITYMID_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_INTENSITYMID_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_INTENSITYMID_OUTPUT_PIN),
#if defined(SL_PWM_INTENSITYMID_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_INTENSITYMID_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_op_time1 = {
  .timer = SL_PWM_OP_TIME1_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_OP_TIME1_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_OP_TIME1_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_OP_TIME1_OUTPUT_PIN),
#if defined(SL_PWM_OP_TIME1_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_OP_TIME1_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_op_time2 = {
  .timer = SL_PWM_OP_TIME2_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_OP_TIME2_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_OP_TIME2_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_OP_TIME2_OUTPUT_PIN),
#if defined(SL_PWM_OP_TIME2_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_OP_TIME2_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_op_time3 = {
  .timer = SL_PWM_OP_TIME3_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_OP_TIME3_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_OP_TIME3_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_OP_TIME3_OUTPUT_PIN),
#if defined(SL_PWM_OP_TIME3_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_OP_TIME3_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_op_time4 = {
  .timer = SL_PWM_OP_TIME4_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_OP_TIME4_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_OP_TIME4_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_OP_TIME4_OUTPUT_PIN),
#if defined(SL_PWM_OP_TIME4_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_OP_TIME4_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_op_time5 = {
  .timer = SL_PWM_OP_TIME5_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_OP_TIME5_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_OP_TIME5_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_OP_TIME5_OUTPUT_PIN),
#if defined(SL_PWM_OP_TIME5_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_OP_TIME5_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_op_time6 = {
  .timer = SL_PWM_OP_TIME6_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_OP_TIME6_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_OP_TIME6_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_OP_TIME6_OUTPUT_PIN),
#if defined(SL_PWM_OP_TIME6_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_OP_TIME6_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_op_time7 = {
  .timer = SL_PWM_OP_TIME7_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_OP_TIME7_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_OP_TIME7_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_OP_TIME7_OUTPUT_PIN),
#if defined(SL_PWM_OP_TIME7_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_OP_TIME7_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_op_time8 = {
  .timer = SL_PWM_OP_TIME8_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_OP_TIME8_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_OP_TIME8_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_OP_TIME8_OUTPUT_PIN),
#if defined(SL_PWM_OP_TIME8_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_OP_TIME8_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_op_time9 = {
  .timer = SL_PWM_OP_TIME9_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_OP_TIME9_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_OP_TIME9_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_OP_TIME9_OUTPUT_PIN),
#if defined(SL_PWM_OP_TIME9_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_OP_TIME9_OUTPUT_LOC),
#endif
};


void sl_pwm_init_instances(void)
{

  sl_pwm_config_t pwm_buzzer_config = {
    .frequency = SL_PWM_BUZZER_FREQUENCY,
    .polarity = SL_PWM_BUZZER_POLARITY,
  };

  sl_pwm_init(&sl_pwm_buzzer, &pwm_buzzer_config);

  sl_pwm_config_t pwm_intensityHi_config = {
    .frequency = SL_PWM_INTENSITYHI_FREQUENCY,
    .polarity = SL_PWM_INTENSITYHI_POLARITY,
  };

  sl_pwm_init(&sl_pwm_intensityHi, &pwm_intensityHi_config);

  sl_pwm_config_t pwm_intensityLow_config = {
    .frequency = SL_PWM_INTENSITYLOW_FREQUENCY,
    .polarity = SL_PWM_INTENSITYLOW_POLARITY,
  };

  sl_pwm_init(&sl_pwm_intensityLow, &pwm_intensityLow_config);

  sl_pwm_config_t pwm_intensityMid_config = {
    .frequency = SL_PWM_INTENSITYMID_FREQUENCY,
    .polarity = SL_PWM_INTENSITYMID_POLARITY,
  };

  sl_pwm_init(&sl_pwm_intensityMid, &pwm_intensityMid_config);

  sl_pwm_config_t pwm_op_time1_config = {
    .frequency = SL_PWM_OP_TIME1_FREQUENCY,
    .polarity = SL_PWM_OP_TIME1_POLARITY,
  };

  sl_pwm_init(&sl_pwm_op_time1, &pwm_op_time1_config);

  sl_pwm_config_t pwm_op_time2_config = {
    .frequency = SL_PWM_OP_TIME2_FREQUENCY,
    .polarity = SL_PWM_OP_TIME2_POLARITY,
  };

  sl_pwm_init(&sl_pwm_op_time2, &pwm_op_time2_config);

  sl_pwm_config_t pwm_op_time3_config = {
    .frequency = SL_PWM_OP_TIME3_FREQUENCY,
    .polarity = SL_PWM_OP_TIME3_POLARITY,
  };

  sl_pwm_init(&sl_pwm_op_time3, &pwm_op_time3_config);

  sl_pwm_config_t pwm_op_time4_config = {
    .frequency = SL_PWM_OP_TIME4_FREQUENCY,
    .polarity = SL_PWM_OP_TIME4_POLARITY,
  };

  sl_pwm_init(&sl_pwm_op_time4, &pwm_op_time4_config);

  sl_pwm_config_t pwm_op_time5_config = {
    .frequency = SL_PWM_OP_TIME5_FREQUENCY,
    .polarity = SL_PWM_OP_TIME5_POLARITY,
  };

  sl_pwm_init(&sl_pwm_op_time5, &pwm_op_time5_config);

  sl_pwm_config_t pwm_op_time6_config = {
    .frequency = SL_PWM_OP_TIME6_FREQUENCY,
    .polarity = SL_PWM_OP_TIME6_POLARITY,
  };

  sl_pwm_init(&sl_pwm_op_time6, &pwm_op_time6_config);

  sl_pwm_config_t pwm_op_time7_config = {
    .frequency = SL_PWM_OP_TIME7_FREQUENCY,
    .polarity = SL_PWM_OP_TIME7_POLARITY,
  };

  sl_pwm_init(&sl_pwm_op_time7, &pwm_op_time7_config);

  sl_pwm_config_t pwm_op_time8_config = {
    .frequency = SL_PWM_OP_TIME8_FREQUENCY,
    .polarity = SL_PWM_OP_TIME8_POLARITY,
  };

  sl_pwm_init(&sl_pwm_op_time8, &pwm_op_time8_config);

  sl_pwm_config_t pwm_op_time9_config = {
    .frequency = SL_PWM_OP_TIME9_FREQUENCY,
    .polarity = SL_PWM_OP_TIME9_POLARITY,
  };

  sl_pwm_init(&sl_pwm_op_time9, &pwm_op_time9_config);

}
