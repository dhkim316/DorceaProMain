#ifndef PWM_INIT_INTENSITYMID_CONFIG_H
#define PWM_INIT_INTENSITYMID_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h>PWM configuration

// <o SL_PWM_INTENSITYMID_FREQUENCY> PWM frequency [Hz]
// <i> Default: 10000
#define SL_PWM_INTENSITYMID_FREQUENCY       10000

// <o SL_PWM_INTENSITYMID_POLARITY> Polarity
// <PWM_ACTIVE_HIGH=> Active high
// <PWM_ACTIVE_LOW=> Active low
// <i> Default: PWM_ACTIVE_HIGH
#define SL_PWM_INTENSITYMID_POLARITY        PWM_ACTIVE_HIGH
// </h> end pwm configuration

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <timer channel=OUTPUT> SL_PWM_INTENSITYMID
// $[TIMER_SL_PWM_INTENSITYMID]
#define SL_PWM_INTENSITYMID_PERIPHERAL              TIMER1
#define SL_PWM_INTENSITYMID_PERIPHERAL_NO           1

#define SL_PWM_INTENSITYMID_OUTPUT_CHANNEL          1
#define SL_PWM_INTENSITYMID_OUTPUT_PORT             gpioPortB
#define SL_PWM_INTENSITYMID_OUTPUT_PIN              1
// [TIMER_SL_PWM_INTENSITYMID]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // PWM_INIT_INTENSITYMID_CONFIG_H
