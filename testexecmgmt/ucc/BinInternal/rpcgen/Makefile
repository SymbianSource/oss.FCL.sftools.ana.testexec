##########################################################################
#
# Makefile for ...
#
##########################################################################

##########################################################################
#
# Definitions
#
##########################################################################
TARGET=		../../../../BuildTools/rpcgen.3gne.linux
CFLAGS=		-g -Wall -DWIN32_TARGET
INCLUDES=	-I..\rpc
OBJECTS=		rpc_clnt.o	\
			rpc_cout.o	\
			rpc_hout.o	\
			rpc_main.o	\
			rpc_pars.o	\
			rpc_scan.o	\
			rpc_svco.o	\
			rpc_util.o	\
			ae_component_rpc.o

LIBS= 		-lpthread

##########################################################################
#
# Rules
#
##########################################################################
$(TARGET):	$(OBJECTS)
	gcc $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o:	%.c
	gcc $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o:	%.cpp
	g++ $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -r -f $(TARGET) *.o *~
