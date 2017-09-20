#ifndef AF_H
#define	AF_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/resource.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "common.h"
#include "sflib.h"
#include "sfxml.h"
#include "sflog.h"
#include "sfconfig.h"

#include "EC00.h"

#define MY_ERR_BUFFER_SIZE         		256

#define _CALC_LINE_NO(_s,_c,_l)         \
{                                       \
   register char *_p_ = (char *)(_s);   \
   register int _i_ = (int)(_c);        \
   register int _lno_ = 1;              \
   while (--_i_ >= 0)                   \
   {                                    \
      if (_p_[_i_] == '\n')             \
         ++_lno_;                       \
   }                                    \
   (_l) = _lno_;                        \
}

#define MY_NAME							"AF"
#define MY_VERSION           			"3.0.1"

#define MY_AF_RET_NORMAL      			EQX_RET_NORMAL
#define MY_AF_RET_END         			EQX_RET_END
#define MY_AF_RET_ERROR       			EQX_RET_ERROR
#define MY_AF_RET_REJECT      			EQX_RET_REJECT
#define MY_AF_RET_ABORT       			EQX_RET_ABORT
#define MY_AF_RET_TIMEOUT     			EQX_RET_TIMEOUT

#define MY_AF_EVENT_ANY					0
#define MY_AF_EVENT_GOT_REQUEST_CLIENT  1

#define MY_AF_EVENT_GOT_MATH_SERVER     2
#define MY_AF_EVENT_GOT_REQUEST_DBPROXY 3

#define MY_AF_EVENT_GET_NUMBER_CLIENT	3
#define MY_AF_EVENT_GET_NUMBER_SERVER   4
#define MY_AF_EVENT_ERROR               5
#define MY_AF_EVENT_ERROR_NOSUCH        6
#define MY_AF_EVENT_REJECT              7
#define MY_AF_EVENT_ABORT               8
#define MY_AF_EVENT_TIMEOUT             9
#define MY_AF_EVENT_FINISH               10
#define MY_AF_EVENT_UNKNOWN				-1000

#define MY_AF_SEND_NOTHING				0
#define MY_AF_SEND_ANY					100

#define MY_AF_STATE_IDLE				0
#define MY_AF_STATE_W_PROXY				1

#define MY_AF_MATH_PLUS					"+"
#define MY_AF_MATH_MINUS				"-"
#define MY_AF_MATH_MULTIPLY				"*"
#define MY_AF_MATH_DIVIDE				"/"

typedef struct _configuration_
{
	int version;
	char name[256];
	char service[256];
} AF_CONFIG;

typedef struct _instance_
{
	char incoming_addr[EQX_RAWDATA_ORIG_SIZE];
	char invoke[EQX_RAWDATA_INVOKE_SIZE];
	int  number_from_client;
	char math_symbol[1];
	int  number_from_dbproxy;
	
	int command;
	char output[100];
	
} AF_INSTANCE;

int af_state_idle(int messageType, EC_UTILS* utils, EQX_MSG *emsg, AF_INSTANCE *instance, char* err);
int af_state_wait_proxy(int messageType, EC_UTILS* utils, EQX_MSG *emsg, AF_INSTANCE *instance, char* err);

extern AF_CONFIG af_config;

//#include "af_interface.h"

#endif	/* EC00_H */
