all: CalculatedResultHttp.so

INCLUDE=./ext/include
LIB_DIR=./ext/lib

######################################################################
# PLATEFORM
######################################################################
# LINUX-X86
#PF_CC=gcc
#PF_CCFLAG=-DOS_LINUX -DM_GENERIC_INT32 -march=i586 -fPIC -O -Wall -g
#PF_EXEFLAG=-march=i586
#PF_EXELIB=-lcurl
######################################################################
# LINUX-X86_64
PF_CC=gcc
PF_CCFLAG=-DOS_LINUX -DM_GENERIC_INT32 -m64 -fPIC -O -Wall -gdwarf-2 
PF_EXEFLAG=-m64
PF_EXELIB=
######################################################################
# FREEBSD
#PF_CC=gcc
#PF_CCFLAG=-DOS_FREEBSD -DM_GENERIC_INT32 -march=i586 -fPIC -O -Wall -g
#PF_EXEFLAG=-march=i586
#PF_EXELIB=
######################################################################
# HPUX
#PF_CC=cc
#PF_CCFLAG=-DOS_HPUX -DM_GENERIC_INT32 -Ae +DD64 -Wp,-H100000 -O -g
#PF_EXEFLAG=
#PF_EXELIB=
######################################################################
# SOLARIS-64
#PF_CC=gcc
#PF_CCFLAG=-DOS_SOLARIS -DM_GENERIC_INT32 -m64 -O -Wall -g
#PF_EXEFLAG=-m64
#PF_EXELIB=-lsocket -lnsl
######################################################################
# AIX
#PF_CC=cc
#PF_CCFLAG=-DOS_AIX -DM_GENERIC_INT32 -qlanglvl=ansi -qmaxmem=15000 -O -g
#PF_EXEFLAG=
#PF_EXELIB=
######################################################################
# DECUNIX
#PF_CC=cc
#PF_CCFLAG=-DOS_DECUNIX -DM_GENERIC_INT64 -std1 -O -g
#PF_EXEFLAG=
#PF_EXELIB=

OBJECTS = af_action_process.o \
		  af_state_idle.o \
		  af_state_wait_proxy.o

PROJ_CC=$(PF_CC)
PROJ_CCFLAG=$(PF_CCFLAG) -I$(INCLUDE) 
PROJ_EXEFLAG=$(PF_EXEFLAG)
PROJ_LDFLAG=-G -L$(LIB_DIR) -R/opt/equinox/lib
PROJ_LDLIB=-lsfconfig -lsf -lec00

clean:
	rm -f *.so *.o buildident.*

$(OBJECTS):
	$(PROJ_CC) -c $(@:.o=.c) $(PROJ_CCFLAG) -o $(@:.c=.o)

CalculatedResultHttp.so: $(OBJECTS)
	echo "char build_timestamp[] = \"@(#$@) Last build time -> `date`\";" > buildident.c
	$(PROJ_CC) $(PROJ_EXEFLAG) -c buildident.c
	ld $(PROJ_LDFLAG) $(PROJ_LDLIB) $(OBJECTS) -o $@ buildident.o 
	rm $(OBJECTS) buildident.*
	
