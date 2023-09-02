#ifndef RELAY_H_
#define RELAY_H_
#include <stdio.h>
#include <stdint.h>
#include "gd32e23x_gpio.h"
#include "stdbool.h"
#include "variables.h"

#define N_RELAY_PIN     					GPIO_PIN_5 //Define Neutral Relay Pin
#define L_RELAY_PIN     					GPIO_PIN_4 //Define Line Relay Pin
#define N_RELAY_PORT    					GPIOA      //Define Neutral Relay Port
#define L_RELAY_PORT							GPIOA      //Define Line Relay Port

#define N_RELAY_FEEDBACK_PIN			GPIO_PIN_6 //Define Neutral Relay Pin
#define L_RELAY_FEEDBACK_PIN			GPIO_PIN_7 //Define Line Relay Pin
#define N_RELAY_FEEDBACK_PORT			GPIOA      //Define Neutral Relay Port
#define L_RELAY_FEEDBACK_PORT			GPIOA      //Define Line Relay Port



typedef struct{
	uint8_t N_Relay:1;
	uint8_t L_Relay:1;
	uint8_t N_feedback:1;
	uint8_t L_feedback:1;
	uint8_t dummy:4;
}rly_status_t;



void Relay_task(void);

bool relay_fault();


#endif