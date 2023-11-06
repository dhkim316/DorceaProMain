#ifndef PWM_INIT_OP_TIME5_CONFIG_H
#define PWM_INIT_OP_TIME5_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h>PWM configuration

// <o SL_PWM_OP_TIME5_FREQUENCY> PWM frequency [Hz]
// <i> Default: 10000
#define SL_PWM_OP_TIME5_FREQUENCY       10000

// <o SL_PWM_OP_TIME5_POLARITY> Polarity
// <PWM_ACTIVE_HIGH=> Active high
// <PWM_ACTIVE_LOW=> Active low
// <i> Default: PWM_ACTIVE_HIGH
#define SL_PWM_OP_TIME5_POLARITY        PWM_ACTIVE_HIGH
// </h> end pwm configuration

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <timer channel=OUTPUT> SL_PWM_OP_TIME5
// $[TIMER_SL_PWM_OP_TIME5]
#define SL_PWM_OP_TIME5_PERIPHERAL              TIMER4
#define SL_PWM_OP_TIME5_PERIPHERAL_NO           4

#define SL_PWM_OP_TIME5_OUTPUT_CHANNEL          1
#define SL_PWM_OP_TIME5_OUTPUT_PORT             gpioPortA
#define SL_PWM_OP_TIME5_OUTPUT_PIN              4
// [TIMER_SL_PWM_OP_TIME5]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // PWM_INIT_OP_TIME5_CONFIG_H
