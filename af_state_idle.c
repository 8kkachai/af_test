#include "af.h"

int
af_state_idle_validate_input(EQX_MSG *emsg, AF_INSTANCE *instance, char* err) {
	
	char* SLASH_SYMBOL = "/";
		
	if (strcmp(emsg->raw[0].data, SLASH_SYMBOL) == 0) {
		SFLOG_ERROR("HTTP Error: 400 – No parameter []");
		sprintf(err, "HTTP Error: 400 – No parameter []");
		
		return -1;
	}	
	
	return 0;
}

int 
af_state_idle(int messageType, EC_UTILS* utils, EQX_MSG *emsg, AF_INSTANCE *instance, char* err)
{	
	SFLOG_INFO("In state idle ...");
	
	if (messageType == MY_AF_EVENT_GOT_REQUEST_CLIENT) 
	{
		instance->command = MY_AF_EVENT_GOT_REQUEST_CLIENT;	
		return MY_AF_STATE_W_PROXY;
	}
	else if (messageType == MY_AF_EVENT_GOT_REQUEST_DBPROXY) 
	{
		instance->command = MY_AF_SEND_ANY;	
		return MY_AF_STATE_IDLE;
	}	
	
	instance->command = MY_AF_SEND_ANY;
	return MY_AF_STATE_IDLE;
}

