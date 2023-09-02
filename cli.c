#include "cli.h"
#include "console.h"
extern CliCmd_t CmdList[];
extern uint8_t Relay_status;
bool helpCmdList(char **argv, uint8_t argc) {
	static uint32_t i = 0;
	uint32_t a = 0;
	while (1) {
		if (CmdList[++i].CmdName == NULL) {
			printf("\n\r  ...DONE");
			i = 0;
			return true;
		}
		
        if (argc >= 2) {
            if (*CmdList[i].CmdName == *argv[1]) {
                printf("\n\r  %s", CmdList[i].CmdName);
                a++;
            }
        } else {
            printf("\n\r  %s", CmdList[i].CmdName);
            a++;
        }
		if (a >= 10) {
			printf("\n\r  ...type help for more\n\r");
			return true;
		}
	}
}


bool cliReset(char **argv, uint8_t argc) {
//	HAL_NVIC_SystemReset();
	return true;
}
bool cliGetSwInfo(char **argv, uint8_t argc) {
    return true;
}
bool cliInfo(char **argv, uint8_t argc) {
	return true;
}
bool cliDebugParam(char **argv, uint8_t argc) {
	return true;
};
bool statusRelay(char **argv, uint8_t argc){
    if(argc==1){
            printf("\r\nRelay 1 : %d\r\n",Relay_status&1);
            return 1;
    }

}
