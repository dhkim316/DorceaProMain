#ifndef PWM_INIT_OP_TIME4_CONFIG_H
#define PWM_INIT_OP_TIME4_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h>PWM configuration

// <o SL_PWM_OP_TIME4_FREQUENCY> PWM frequency [Hz]
// <i> Default: 10000
#define SL_PWM_OP_TIME4_FREQUENCY       10000

// <o SL_PWM_OP_TIME4_POLARITY> Polarity
// <PWM_ACTIVE_HIGH=> Active high
// <PWM_ACTIVE_LOW=> Active low
// <i> Default: PWM_ACTIVE_HIGH
#define SL_PWM_OP_TIME4_POLARITY        PWM_ACTIVE_HIGH
// </h> end pwm configuration

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <timer channel=OUTPUT> SL_PWM_OP_TIME4
// $[TIMER_SL_PWM_OP_TIME4]
#define SL_PWM_OP_TIME4_PERIPHERAL              TIMER4
#define SL_PWM_OP_TIME4_PERIPHERAL_NO           4

#define SL_PWM_OP_TIME4_OUTPUT_CHANNEL          0
#define SL_PWM_OP_TIME4_OUTPUT_PORT             gpioPortA
#define SL_PWM_OP_TIME4_OUTPUT_PIN              5
// [TIMER_SL_PWM_OP_TIME4]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // PWM_INIT_OP_TIME4_CONFIG_H
