#ifndef VARIABLES_H_
#define VARIABLES_H_
#include <stdint.h>
typedef struct{
	uint8_t relay:1;
	uint8_t fault:1;
}flags;

#endif