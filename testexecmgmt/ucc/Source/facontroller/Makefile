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
include /home/penuser/3GNetworkEmulator/Makefile.conf

TARGET=../../bin/facontroller
INCLUDES= -I../include/
OBJECTS=		CSFacontroller.o 	\
			CSvcFacontroller.o	\
			facontroller_svc.o	\
			facontroller_svc_stub_impl.o	\
			facontroller_xdr.o

LIBS=-L../../BinInternal -lproc -ldynamicsconfig -lalias -lintalloc -ldynwrap

##########################################################################
#
# Rules
#
##########################################################################
$(TARGET):	$(OBJECTS) ../../BinInternal/libproc.a ../../BinInternal/libdynamicsconfig.a ../../BinInternal/libalias.a ../../BinInternal/libintalloc.a ../../BinInternal/libdynwrap.a
	g++ $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o:	%.c
	gcc $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o:	%.cpp
	g++ $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -r -f $(TARGET) *.o *~ dyntmp*
