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
TARGET=		../../bin/hostexecuteasync
INCLUDES= 	-I../include
OBJECTS=	CSHostexecuteasync.o		\
		CSvcHostexecuteasync.o		\
		hostexecuteasync_svc.o		\
		hostexecuteasync_svc_stub_impl.o	\
		hostexecuteasync_xdr.o		


LIBS=		-L../../BinInternal -lproc

##########################################################################
#
# Rules
#
##########################################################################
$(TARGET):	$(OBJECTS) ../../BinInternal/libproc.a
	g++ -gstabs -Wall -o $(TARGET) $(OBJECTS) $(LIBS)

%.o:	%.c
	gcc -gstabs -Wall $(INCLUDES) -c $< -o $@

%.o:	%.cpp
	g++ -gstabs -Wall $(INCLUDES) -c $< -o $@

clean:
	rm -r -f $(TARGET) *.o *~ dyntmp*
