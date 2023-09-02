#ifndef _CLI_H_
#define _CLI_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>



bool helpCmdList(char **argv, uint8_t argc);
bool cliReset(char **argv, uint8_t argc);
bool cliGetSwInfo(char **argv, uint8_t argc);
bool cliInfo(char **argv, uint8_t argc);
bool cliDebugParam(char **argv, uint8_t argc);
bool statusRelay(char **argv, uint8_t argc);
#endif