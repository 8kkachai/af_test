#include "af.h"

int 
af_state_wait_proxy(int messageType, EC_UTILS* utils, EQX_MSG *emsg, AF_INSTANCE *instance, char* err)
{	
	SFLOG_INFO("In state wait proxy ...... ");
	SFLOG_INFO( "===============================================");
	if (messageType == MY_AF_EVENT_GOT_MATH_SERVER) {
		SFLOG_INFO( "     INCOMING ADDR: %s", instance->incoming_addr);
		SFLOG_INFO( "            INVOKE: %s", instance->invoke);
		SFLOG_INFO( "NUMBER FROM CLIENT: %d", instance->number_from_client);
		SFLOG_INFO( "       MATH SYMBOL: %s", instance->math_symbol);
		SFLOG_INFO( " NUMBER FROM PROXY: %d\n\n\n\n\n", instance->number_from_dbproxy);
		
		int result_value = 0;
		
		if (strstr(instance->math_symbol, MY_AF_MATH_PLUS) != NULL) 
		{
			result_value = instance->number_from_client + instance->number_from_client;
		}
		else if (strstr(instance->math_symbol, MY_AF_MATH_MINUS) != NULL) 
		{
			result_value = instance->number_from_client - instance->number_from_client;
		}
		else if (strstr(instance->math_symbol, MY_AF_MATH_MULTIPLY) != NULL) 
		{
			result_value = instance->number_from_client * instance->number_from_client;
		}
		else if (strstr(instance->math_symbol, MY_AF_MATH_DIVIDE) != NULL) 
		{
			result_value = instance->number_from_client / instance->number_from_client;
		}
		
		sprintf(instance->output, "%d %s %d = %d", instance->number_from_client, instance->math_symbol, instance->number_from_client, result_value);
		SFLOG_INFO(instance->output);
		instance->command = MY_AF_EVENT_GOT_MATH_SERVER;	
	}

	return MY_AF_STATE_IDLE;
}

