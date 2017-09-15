#include "af.h"

int 
af_state_wait_proxy(int messageType, EC_UTILS* utils, EQX_MSG *emsg, AF_INSTANCE *instance, char* err)
{	
	SFLOG_INFO("In state wait proxy");
	instance->command = MY_AF_SEND_ANY;
	(void) ec_log_write (utils, "LOGNAME", "Waiting Proxy");

	strcpy(emsg->diag, "in state wait proxy");
	
	return 0;
}

