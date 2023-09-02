#include "relay.h"
#include "variables.h"

enum{
	Fault_Check,
	Relay_Toggle,
	Weld_Check,
	Fault_State
}state_t;
rly_status_t relay_status;
uint8_t relay;
extern flags_t evse_flags;
extern Alarms_t Alarms;
void Relay_task(void){
	static uint8_t state = 0;
	
	switch(state)	
	{
		case Fault_Check:
			state = relay_fault()?Relay_Toggle:Fault_State;
			break;
		
		case Relay_Toggle:
			if(!evse_flags.relay)
			{
				state = Fault_Check; 
				break;
			}
			gpio_bit_toggle(N_RELAY_PORT,N_RELAY_PIN);
			relay_status.N_Relay=~relay_status.N_Relay;
			gpio_bit_toggle(L_RELAY_PORT,L_RELAY_PIN);
			relay_status.L_Relay=~relay_status.L_Relay;
		
		case Weld_Check:
			relay_status.N_feedback = gpio_output_bit_get(N_RELAY_FEEDBACK_PORT,N_RELAY_FEEDBACK_PIN);
			relay_status.L_feedback = gpio_output_bit_get(L_RELAY_FEEDBACK_PORT,N_RELAY_FEEDBACK_PIN);
			state = Fault_Check;
			break;
		
		case Fault_State:
			gpio_bit_reset(N_RELAY_PORT,N_RELAY_PIN);
		  relay_status.N_Relay = 0;
			gpio_bit_reset(L_RELAY_PORT,L_RELAY_PIN);
			relay_status.L_Relay = 0;
		  state = Fault_Check;
			break;
			
			
	}
	
}
bool relay_fault()
{
		Alarms.L_rw = relay_status.L_feedback == relay_status.L_Relay ? 1:0;
		Alarms.N_rw = relay_status.N_feedback == relay_status.N_Relay ? 1:0;
		if(Alarms.L_rw&&Alarms.N_rw)
			return false;
		return true;
	
}


