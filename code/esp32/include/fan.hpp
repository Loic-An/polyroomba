#include "driver/ledc.h"

#define LEDC_HS_TIMER LEDC_TIMER_0
#define LEDC_HS_MODE LEDC_LOW_SPEED_MODE
#define LEDC_HS_CH0_GPIO (18)
#define LEDC_HS_CH0_CHANNEL LEDC_CHANNEL_0

#define DUTY_RESOLUTION LEDC_TIMER_10_BIT // Duty resolution
#define PWM_FREQUENCY 25000               // Frequency in Hz for 4-pin fans