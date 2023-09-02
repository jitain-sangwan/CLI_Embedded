#ifndef VARIABLES_H_
#define VARIABLES_H_
#include <stdint.h>

typedef struct{
	uint8_t relay:1;
	uint8_t dummy:7;
}flags_t;



typedef struct{
	uint16_t N_rw:1;
	uint16_t L_rw:1;
}Alarms_t;

#endif