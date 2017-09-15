#include "af.h"

int 
af_state_idle(int messageType, EC_UTILS* utils, EQX_MSG *emsg, AF_INSTANCE *instance, char* err)
{	
	SFLOG_INFO("In state idle from: %s", emsg->raw[0].type);
	
	instance->command = MY_AF_SEND_ANY;
	(void) ec_log_write (utils, "LOGNAME", "Idle");
	
	char* SLASH_SYMBOL = "/";
	
	if (strcmp(emsg->raw[0].data, SLASH_SYMBOL) == 0) {
		SFLOG_ERROR("HTTP Error: 400 – No parameter []");
		sprintf(err, "HTTP Error: 400 – No parameter []");
		
		return MY_AF_STATE_IDLE;
	}	
	/*
	sprintf(to, "calre.ES04.0.0");
	strcpy(timeout,"10");
	sprintf(ret,"%d",MY_AF_RET_NORMAL);
	sprintf(state,"%d",MY_STATE_W_GFEP_RES);
	sprintf(diagnostic,"waiting on state[%d]", MY_STATE_W_GFEP_RES);
	return 0;
	*/
	
	//return MY_AF_STATE_W_PROXY;
	return MY_AF_STATE_IDLE;
}

