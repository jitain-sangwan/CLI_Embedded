#include "console.h"
#include "cli.h"

const CliCmd_t CmdList[] = {
	{"help", 									&helpCmdList},
    {"reset", 								&cliReset},
	{"swinfo", 								&cliGetSwInfo},
	{"status", 								&cliDebugParam},
	{"info",									&cliInfo},
    {"relay_status",                          &statusRelay},

	/* add new command here */
	{(char *)NULL,									(CmdFuncPtr)NULL, 							0}		// do not remove this line, should be the end of this structure
};