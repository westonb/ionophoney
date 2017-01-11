// Define to prevent recursive inclusion -------------------------------------
#ifndef __MAIN_H
#define __MAIN_H
#define STM32F334K8
#include "stm32f30x.h"

#define PERIOD_6_78MHZ 320
#define BUCK_PWM_PERIOD 10000
#define BUCK_PWM_MAX 9200
#define BUCK_PWM_MIN 800
#define DT_FALLING 30 //*250ps
#define DT_RISING 30
#define MOD_DC_OFFSET 5500
#define MOD_UPDATE_RATE 0
#define ADC_SAMPLE_TIME 100

//exported functions

extern uint32_t output_current, output_voltage, supply_voltage, audio_voltage;
//extern int32_t audio_filtered = 0; //signed 

#endif // __MAIN_H 

