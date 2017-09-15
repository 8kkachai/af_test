/***************************************************************************

    MIMO Ltd
    AF Template

    af_template - Equinox AF Template

    started              : Mon June 25 2012
    copyright            : (C) 2012 by Cronus
    email                : thawatcb@ais.co.th

    Revision history
    3.0.1    Add function af_get_version for EC00 verify.
    3.0.0    Re-design software structure
			 Support utilities
    2.0.0    Use new EQX_MSG structure.
    1.0.0    Initialize.

 ***************************************************************************/

#include "af.h"

char build_version[] = "@(#"MY_NAME") Version -> "MY_VERSION;

AF_CONFIG af_config;

/*
 * Associated Functions
 */
int
get_xml_tag_value(char *message, char *xml_tag, char *value)
{
	char *p, *q;

	strcpy(value, "");
	p = strstr(message, xml_tag);
	if (p!=NULL) {
		p += strlen(xml_tag);
		q = strstr(p+1, "\"");
		if (q!=NULL)
		{
			memcpy(value, p, q-p);
			value[q-p] = 0;
		}
	}
	
	if (value[0]==0)
		return -1;
	else
		return 0;
}

/*
 * Configuration Handling
 */

int
af_config_verify(SF_CONFIG_MEM *config)
{
	SF_CONFIG_MEM cold, warm, tmp;
	SF_XML xml;

	if (config == NULL) return -1;
	if (SF_config_find_element(config, "configuration", &tmp, NULL, 1) != 0) return -1;
	if (SF_config_find_element(&tmp, "cold", &cold, NULL, 1) != 0) return -1;
	if (SF_config_find_element(&tmp, "warm", &warm, NULL, 1) != 0) return -1;

	// Verify and get cold config
	cold.curr = 0;
	if (SF_config_find_element(&cold, "sf", NULL, &xml, 1) != 0) return -1;
	if (SF_config_find_attr(&xml, "group", af_config.name, sizeof(af_config.name), 1) != 0) return -1;
	if (SF_config_find_attr(&xml, "service", af_config.service, sizeof(af_config.service), 1) != 0) return -1;
	
	// Verify and get warm config
	warm.curr = 0;

	return 0;
}

void
af_config_reload (SF_CONFIG_MEM *config)
{
	SF_CONFIG_MEM cold, warm, tmp;

	if (config == NULL) return;
	if (SF_config_find_element(config, "configuration", &tmp, NULL, 1) != 0) return;
	if (SF_config_find_element(&tmp, "cold", &cold, NULL, 1) != 0) return;
	if (SF_config_find_element(&tmp, "warm", &warm, NULL, 1) != 0) return;

	// Get cold config
	cold.curr = 0;

	// Get warm config
	warm.curr = 0;
	
}

void
af_get_version (char *ec00_version, char *common_version)
{
    strcpy(ec00_version, MY_EC00_VERSION);
    strcpy(common_version, MY_COMMON_VERSION);
}


/*
 * Primary Actions
 */

static int
extract_instance(char* raw_instance, AF_INSTANCE* instance)
{
	char *p, *_p;
	char temp_buff[20];
	char *instance_buff;
	
	instance_buff = (char *) malloc (EQX_INSTANCE_SIZE);
	(void) memset (instance_buff, 0, EQX_INSTANCE_SIZE);
	strncpy(instance_buff, raw_instance, EQX_INSTANCE_SIZE);
	p = instance_buff;
	_p = strstr(instance_buff,"|");
	if(_p==NULL) {
		memset(instance->incoming_addr, 0, sizeof(instance->incoming_addr));
		instance->timestamp_sec = 0;
		instance->timestamp_usec = 0;
		
		instance->command = MY_AF_SEND_NOTHING;
	} else {
		//first element
		p = instance_buff;
		*_p='\0';
		strcpy(instance->incoming_addr,p);
		//next element
		p = _p + 1;
		_p = strstr(p,"|");
		*_p='\0';
		strcpy(temp_buff,p);
		instance->timestamp_sec = atoi(temp_buff);
		//next element
		p = _p + 1;
		_p = strstr(p,"|");
		*_p='\0';
		strcpy(temp_buff,p);
		instance->timestamp_usec = atoi(temp_buff);
	}
	free(instance_buff);
	return 0;
}

static int
compose_instance(AF_INSTANCE* instance, char* raw_instance)
{
	char temp_buff[20];
	strcpy(raw_instance,"");
	strcat(raw_instance,instance->incoming_addr);strcat(raw_instance,"|");
	sprintf(temp_buff, "%d", instance->timestamp_sec);
	strcat(raw_instance,temp_buff);strcat(raw_instance,"|");
	sprintf(temp_buff, "%d", instance->timestamp_usec);
	strcat(raw_instance,temp_buff);strcat(raw_instance,"|");

	return 0;
}

static int
extract_raw_data(AF_INSTANCE* instance, EQX_MSG* emsg, int* eventType, char *err)
{
	*eventType = MY_AF_EVENT_ANY;
	
	return 0;
}

static int
construct_raw_data(AF_INSTANCE* instance, EQX_MSG* emsg)
{
	if (instance->command == MY_AF_SEND_ANY)
	{
		sprintf(emsg->raw[0].to, "%s", emsg->raw[0].orig);
		sprintf(emsg->raw[0].udef[UDEF_ATTR_ECODE], "%d", 200);		
		sprintf(emsg->raw[0].type, "%s", "response");
	}
	else if (instance->command == MY_AF_SEND_FINISH)
	{
		sprintf(emsg->raw[0].to, "%s", emsg->raw[0].orig);
		sprintf(emsg->raw[0].udef[UDEF_ATTR_ECODE], "%d", 200);
		sprintf(emsg->raw[0].type, "%s", "response");
	}
	return 0;
}

/*
 * Action Process
 */

int
af_action_process(EC_UTILS* utils, EQX_MSG* emsg, int data_entry, char* state, char* err)
{
	int r=0;
	int af_state = atoi(state);
	int af_next_state = MY_AF_STATE_IDLE;
	int af_return_code = atoi(emsg->ret);
	int messageEvent = 0;
	AF_INSTANCE *instance;
	
	SFLOG_DEBUG("Start AF Business Logic");
	/* (void) ec_alarm_raise (utils, "ALARMNAME", "", ALARM_SEVERITY_MAJOR, ALARM_CATEGORY_APPLICATION, ALARM_TYPE_NOMAL);
	(void) ec_stat_increment (utils, "STATNAME");
	(void) ec_log_write (utils, "LOGNAME", "Start AF Business Logic"); */

	strcpy(err, "");
	SFLOG_INFO("Raw data is [%s]", emsg->raw[0].data);
	
	/*
	 * handle platform error
	 */
	switch(af_return_code) {
		case MY_AF_RET_NORMAL:
		case MY_AF_RET_TIMEOUT:
			break;

		case MY_AF_RET_ERROR:
		case MY_AF_RET_REJECT:
		case MY_AF_RET_ABORT:
		case MY_AF_RET_END:

		default:
			sprintf(err,"AF does not handle platform error");
			return -1;
	}
	
	/*
	 * Extract raw_instance data element
	 */
	instance = (AF_INSTANCE *) malloc (sizeof(AF_INSTANCE));
	(void) memset (instance, 0, sizeof(AF_INSTANCE));
	r = extract_instance(emsg->instance, instance);
	if (r!=0)
	{
		sprintf(err,"raw instance collapsed");
		return -1;
	}

	/*
	 * extract message
	 */
	messageEvent = MY_AF_EVENT_UNKNOWN;
	if(emsg->raw_entry>1)
	{
		sprintf(err,"not support multi-raw data");
		return -1;
	}
	r = extract_raw_data(instance, emsg, &messageEvent, err);
	if(r!=0)
	{
		sprintf(err,"cannot get message events");
		return -1;
	}
    //emsg->raw[0].data
	
    /*
	 * distribute process to action code modules
	 */
	if(af_return_code == MY_AF_RET_NORMAL) {
		switch(af_state) {
			case MY_AF_STATE_IDLE:
				af_next_state = af_state_idle(messageEvent, utils, emsg, instance, err);
				break;
			case MY_AF_STATE_W_PROXY:
				af_next_state = af_state_wait_proxy(messageEvent, utils, emsg, instance, err);
				break;		
			default:
				sprintf(err,"wrong state found [%d]",af_state);
				return -1;
		}
	} else { //MY_AF_RET_TIMEOUT
		af_next_state = MY_AF_STATE_IDLE;
	}
	
	if(r!=0){
		sprintf(err,"processing error [%d]",af_state);
		return -1;
	}
	
	r = construct_raw_data(instance, emsg);
	if (r!=0)
	{
		sprintf(err,"cannot get message events");
		return -1;
	}
	
	r = compose_instance(instance, emsg->instance);
    free(instance);
	if (r!=0)
	{
		sprintf(err,"cannot compose instance");
		return -1;
	}
	sprintf(state, "%d", af_next_state);
	
	return 0;
}

