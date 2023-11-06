#ifndef PWM_INIT_OP_TIME7_CONFIG_H
#define PWM_INIT_OP_TIME7_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h>PWM configuration

// <o SL_PWM_OP_TIME7_FREQUENCY> PWM frequency [Hz]
// <i> Default: 10000
#define SL_PWM_OP_TIME7_FREQUENCY       10000

// <o SL_PWM_OP_TIME7_POLARITY> Polarity
// <PWM_ACTIVE_HIGH=> Active high
// <PWM_ACTIVE_LOW=> Active low
// <i> Default: PWM_ACTIVE_HIGH
#define SL_PWM_OP_TIME7_POLARITY        PWM_ACTIVE_HIGH
// </h> end pwm configuration

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <timer channel=OUTPUT> SL_PWM_OP_TIME7
// $[TIMER_SL_PWM_OP_TIME7]
#define SL_PWM_OP_TIME7_PERIPHERAL              TIMER3
#define SL_PWM_OP_TIME7_PERIPHERAL_NO           3

#define SL_PWM_OP_TIME7_OUTPUT_CHANNEL          0
#define SL_PWM_OP_TIME7_OUTPUT_PORT             gpioPortD
#define SL_PWM_OP_TIME7_OUTPUT_PIN              1
// [TIMER_SL_PWM_OP_TIME7]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // PWM_INIT_OP_TIME7_CONFIG_H
