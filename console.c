#include <stdio.h>
#include <stdint.h>
#include "fifo.h"
#include "console.h"
#include "uart_driver.h"
#include <string.h>
static char Hist[MAX_HISTORY_ENTRIES][HISTORY_BUFFER_ENTRY_SIZE];
static uint8_t ConsoleTxBuffer[CONSOLE_TX_FIFO_LEN];
static uint8_t ConsoleRxBuffer[CONSOLE_RX_FIFO_LEN];
static char *argv[10];
static int8_t HistoryIndex;
static int8_t PrevHistoryIndex;
char **ptr4;
extern CliCmd_t CmdList[];
char Buffer[MAX_LINE_LENGTH];
FIFO_BUFFER Uart_Rx,Uart_Tx;
const char PromptStr[]= "\n\rCLI>";
char *PtrHistory;
/*  UNCOMMENT WHEN IN CONTROLLER */
void _putchar(char character) {
 	SendChar(character);
 }

bool SendChar(int ch) {
 	return (FIFO_Put(&Uart_Tx, ch));
}

void InitConsole(void) {
	FIFO_Init(&Uart_Tx, ConsoleTxBuffer, CONSOLE_TX_FIFO_LEN);
	FIFO_Init(&Uart_Rx, ConsoleRxBuffer, CONSOLE_RX_FIFO_LEN);
}

bool ConsoleWrite(void) {
	uint16_t length = 8, i = 0;
	int8_t data;
	/*check the number of byte available in fifo*/
	i = FIFO_Count(&Uart_Tx);
	if (i < length) {
		length = i;
	}
	for (i = 0; i < length; ++i) {
		/*Write Length Bytes of data to the TxFifo*/
		if (FIFO_Get(&Uart_Tx, (uint8_t *)&data) == true) {
			Uart_Transmit(data);
		} else {
			return false;
		}
	}
	return true;
}

void GetCliData(void) {
	static int i = 0, insert = 0, length = 0;
	static unsigned char state = 0;
	static char *pbuffer = NULL;
	unsigned char c = 0;
    pbuffer =Buffer;
	do {

		if (FIFO_Get(&Uart_Rx,&c) == false) {
			return;
		}

		switch (state) {

			case 0: {
				// not an <ESC>
				if (c != '\033') {
					break;
				}
				state = 1;
			}
			break;

			case 1: {
				// not an <ESC> [
				if (c != '[') {
					state = 0;
					break;
				}
				state = 2;
			}
			break;

			case 2: {
				state = 0;
				switch (c) {
					case 'A': {
						// cursor up key......previous
						c = '';
					}
					break;

					case 'B': {
						// cursor down key....next
						c = '';
					}
					break;

					case 'C': {
						// cursor right key...forward
						c = '';
					}
					break;

					case 'D': {
						// cursor left key....backward
						c = '';
					}
					break;

					default: {
						// not an <ESC> [ C|D
						// kill the <ESC>, return the rest
						c = '[';
					}
					break;

				} // End switch (c).
			}
			break;

			default: {
				printf("\n\rIllegal State");  //printf_error function
				break;
			} // End switch (state).
		}
	} while (state != 0); //end do-while here
	switch (c)
	{
		case CTRL('C'):
				break;

		case CTRL('A'): {
			// move cursor to the beginning of the line
			if (insert != 0) {
				printf("\033[%dD", insert);
				insert = 0;
			} else {
				printf("%c", CTRL('G'));
			}
		}
		break;

		case CTRL('B'): {
			// move cursor left
			if (insert != 0) {
				printf("\033[D");
				insert--;
			} else {
				printf("%c", CTRL('G'));
			}
		}
		break;

		case CTRL('E'): {
			// move cursor to the end of the line
			if (insert != length) {
				printf("\033[%dC", length - insert);
				insert = length;
			} else {
				printf("%c", CTRL('G'));
			}
		}
		break;

		case CTRL('F'): {
			// move cursor right
			if (insert != length) {
				printf("\033[C");
				insert++;
			} else {
				printf("%c", CTRL('G'));
			}
		}
		break;

		case CTRL('H'): {
			// delete charactor to the left of the cursor
			if (insert != 0) {
				// adjust insert point
				insert--;
				for (i = insert; i < length; i++)   // shift text in
				{
					*(pbuffer + i) = *(pbuffer + i + 1);    //  buffer
				}
				// backup one, delete under cursor
				printf("\033[D\033[P");
				// adjust line length
				length--;
			} else {
				printf("%c", CTRL('G'));
			}
		}
		break;

		case CTRL('D'):
		case '\177': {
			// delete charactor under the cursor
			if (insert < length) {
				for (i = insert; i < length; i++)   // shift text in
				{
					*(pbuffer + i) = *(pbuffer + i + 1);    //  buffer
				}
				// delete under the cursor
				printf("\033[P");
				// adjust line length
				length--;
			} else {
				printf("%c", CTRL('G'));
			}
		}
		break;

		case CTRL('U'): {
			// kill the line
			if (insert != 0) {
				*pbuffer = '\0';
				printf("\033[%dD\033[K", insert);
				length = 0;
				insert = 0;
			}
		}
		break;

		case '\t': {
			if (length) {
				length = searchCliCmd((uint8_t *)pbuffer);
				insert = length;
				printf(PromptStr);
				printf("%s", pbuffer);
			}
		}
		break;

		case '\r':
		case '\n': {
			// Enter
			if (length == 0) {
				*pbuffer = '\0';
			} else {
				AddHistory(pbuffer);
				Shell(pbuffer);
			}
			printf(PromptStr);
			insert = 0;
			length = 0;
		}
		break;

		case CTRL('P'): {
			// get the previous command from history
			PtrHistory = GetHistoryPrevious();
			if (PtrHistory != NULL) {
				strcpy(pbuffer, PtrHistory);
				// move the curser to beginning of line
				if (insert != 0) {
					printf("\033[%dD", insert);
				}
				// clear line, print new line
				printf("\033[K%s", pbuffer);
				length = strlen(pbuffer);
				insert = length;
			} else {
				printf("%c", CTRL('G'));
			}
		}
		break;

		case CTRL('N'): {
			// get the next command from history
			PtrHistory = GetHistoryNext();
			if (PtrHistory != NULL) {
				strcpy(pbuffer, PtrHistory);
				// move the curser to beginning of line
				if (insert != 0) {
					printf("\033[%dD", insert);
				}
				// clear line, print new line
				printf("\033[K%s", pbuffer);
				length = strlen(pbuffer);
				insert = length;
			} else
				printf("%c", CTRL('G'));
		}
		break;

		default: {

			// Insert a character.
			if (length == (MAX_LINE_LENGTH - 1)) {
				printf("%c", CTRL('G'));
			} else if (insert == length) {
				length++;
				*(pbuffer + insert) = c;
				insert++;
				*(pbuffer + insert) = '\0';
				printf("%c", c);
			} else {
				for (i = length; i >= insert; i--) {
					*(pbuffer + i + 1) = *(pbuffer + i);
				}
				*(pbuffer + insert) = c;
				printf("%s\033[%dD", pbuffer + insert, length - insert);
				insert++;
				length++;
			}
		}
		break;

	} // End switch (c).

	fflush(stdout);

}


bool Parse(char **line, int *argc, char **argv) {
	char *cp = *line;
	char *pp;
	int indx = 0;
	int semi = 0;

	while (*cp) {
		// Skip leading spaces
		while (*cp && *cp == ' ')
			cp++;
		if (!*cp) {
			break;  // Line ended with a string of spaces
		}

		if (*cp == ';') {
			*cp = '\0';
			semi = 1;
			break;
		}

		if (indx < MAX_ARGV) {
			argv[indx++] = cp;
		} else {
			printf("\nERR: Parse- Too many argumnts\n\r");
			break;
		}

		while (*cp) {
			if (*cp == ' ') {
				*cp++ = '\0';
				break;
			} else if (*cp == ';') {
				break;
			} else if (*cp == '"') {
				// Swallow quote, scan till following one
				if (argv[indx - 1] == cp) {
					argv[indx - 1] = ++cp;
				}
				pp = cp;
				while (*cp && *cp != '"') {
					if (*cp == '\\') {
						// Skip over escape - allows for escaped '"'
						cp++;
					}
					// Move string to swallow escapes
					*pp++ = *cp++;
				}
				if (!*cp) {
					printf("\nERR: Parse- Unbalanced str!\n\r");
					break;
				} else {
					if (pp != cp)
						*pp = '\0';
					*cp++ = '\0';
					break;
				}
			} else {
				cp++;
			}
		}
	}
	if (semi) {
		*line = cp + 1;
	} else {
		*line = cp;
	}
	*argc = indx;
	return true;
}
void AddHistory(char *buffer) {
	uint8_t i = 0, len1 = 0, len2 = 0;

	len2 = strlen(buffer);

	for (i = 0; i < HistoryIndex; ++i) {
		len1 = strlen(Hist[i]);
		if ((strncmp(Hist[i], buffer, len2) == 0) && (len1 == len2)) {
			PrevHistoryIndex = i;
			return;
		}
	}
	strncpy(Hist[HistoryIndex], buffer, HISTORY_BUFFER_ENTRY_SIZE);
	if (PrevHistoryIndex < MAX_HISTORY_ENTRIES) {
		PrevHistoryIndex = HistoryIndex;
	}
	if (++HistoryIndex >= MAX_HISTORY_ENTRIES) {
		HistoryIndex = 0;
	}
}

char* GetHistoryPrevious(void) {
	if (PrevHistoryIndex < 0) {
		PrevHistoryIndex = HistoryIndex ? (HistoryIndex - 1) : 0;
	}
	return (Hist[PrevHistoryIndex--]);
}

char* GetHistoryNext(void) {
	if (PrevHistoryIndex >= HistoryIndex) {
		PrevHistoryIndex = 0;
	} else if (PrevHistoryIndex < 0) {
		PrevHistoryIndex = HistoryIndex ? (HistoryIndex - 1) : 0;
	}
	return (Hist[PrevHistoryIndex++]);
}


uint32_t searchCliCmd(uint8_t *buf) {
	uint8_t len, flag = 0;
	uint32_t pos, count, i;

	len = strlen((char *)buf);
	for (i = 0, count = 0; CmdList[i].CmdName != NULL; ++i) {
		if (strncmp((char *)buf, (char *)CmdList[i].CmdName, len) == 0) /* command search through a list of commands*/
		{
            count++;
            if (count != 1) {
                printf("\n\r %s", CmdList[pos].CmdName);
            }
            pos = i;
        }
    }
	
	if(count) {
		if (count == 1) {
			strcpy((char *)buf, (char *)CmdList[pos].CmdName);
			strcat((char *)buf, " ");
		} else {
			printf("\n\r %s", CmdList[pos].CmdName);
		}
	}

	return (strlen((char *)buf));
}


bool Shell(char *pbuffer) {
	static char **ptr3;
	int argc, i, flag1 = 0, flag2 = 0;
	char *cmd;

	ptr4 = argv;

	ptr3 = &pbuffer;

	/* call parse function */
	Parse(ptr3, &argc, ptr4);

	/* process the command received */
	cmd = *(ptr4);
	for (i = 0; CmdList[i].CmdName != NULL; i++) {
		if (strcmp((char *)cmd, (char *)CmdList[i].CmdName) == 0) /* command search through a list of commands*/
		{
			break;
		}
	}
	/* If command not found in the list */
	if (CmdList[i].CmdName == NULL) {
		printf("\n\rERR: Inv Cmd");
		flag1 = 1;
	}
	
	/*Here we can Change the State for Console Access*/
//	ConsoleIdleTime = 900;		/*This is the time after which we will make console idle again and any further access will raise the trap*/
	/* If any error-flag is set then return false*/

	if (flag1 == 1 || flag2 == 1) {
		flag1 = 0;
		flag2 = 0;
		return false;
	}
    if (CmdList[i].FuncPtr(ptr4, argc) != false) {
        flag2 = 1;
    }
	return true;
}