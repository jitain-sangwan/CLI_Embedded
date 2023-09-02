#ifndef	_CONSOLE_H_
#define _CONSOLE_H_

#include <stdint.h>
#include <stdbool.h>
#include "fifo.h"


#define CONSOLE_TX_FIFO_LEN		       1024
#define CONSOLE_RX_FIFO_LEN		       256
#define CTRL(s)         		         ((s)-'@')
#define MAX_ARGV				             5
#define MAX_LINE_LENGTH 		         60
#define MAX_HISTORY_ENTRIES 		     8
#define HISTORY_BUFFER_ENTRY_SIZE 	 MAX_LINE_LENGTH

typedef bool (*CmdFuncPtr)(char **, uint8_t );
typedef	struct {
	char *CmdName;
	CmdFuncPtr FuncPtr;
} CliCmd_t;

;
void InitConsole(void);
void GetCliData(void);
bool ConsoleWrite(void);
bool Parse(char **line, int *argc, char **argv);
void AddHistory(char *buffer);
char* GetHistoryPrevious(void);
char* GetHistoryNext(void);
uint32_t searchCliCmd(uint8_t *buf);
bool Shell(char *pbuffer);
bool SendChar(int ch);
#endif