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
TARGET=		../../bin/mobster
INCLUDES=	-I../MobileTermination/ -I../include/
OBJECTS=	mobster_client_server_protocol.o

RPCGENOBJECTS=	CSMobster.o 			\
		CSvcMobster.o			\
		mobster_svc.o			\
		mobster_svc_stub_impl.o		\
		mobster_xdr.o	

LIBS=  		-L../../BinInternal -lthread -lmt -lrpcsvc -lpthread

##########################################################################
#
# Rules
#
##########################################################################
$(TARGET):	$(RPCGENOBJECTS) $(OBJECTS) ../../BinInternal/libmt.a ../../BinInternal/libthread.a
	g++ -gstabs -Wall $(INCLUDES) $(RPCGENOBJECTS) $(OBJECTS) $(LIBS) -o $(TARGET) 

%.o:	%.c
	gcc -gstabs -Wall $(INCLUDES) -c $< -o $@

%.o:	%.cpp
	g++ -gstabs -Wall $(INCLUDES) -c $< -o $@

clean:
	rm -r -f $(TARGET) *.o *~ 
