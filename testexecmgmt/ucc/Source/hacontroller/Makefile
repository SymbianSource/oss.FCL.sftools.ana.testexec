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

TARGET=../../bin/hacontroller
INCLUDES= -I../include
OBJECTS=	CSHacontroller.o 		\
		CSvcHacontroller.o		\
		hacontroller_svc.o		\
		hacontroller_svc_stub_impl.o	\
		hacontroller_xdr.o		\
		CNetworkPartitionManager.o	

LIBS=-L../../BinInternal/ -lalias -ldynamicsconfig -lintalloc -lproc -ldynwrap

##########################################################################
#
# Rules
#
##########################################################################
$(TARGET):	$(OBJECTS) ../../BinInternal/libalias.a ../../BinInternal/libdynamicsconfig.a ../../BinInternal/libintalloc.a ../../BinInternal/libproc.a ../../BinInternal/libdynwrap.a
	g++ $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o:	%.c
	gcc $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o:	%.cpp
	g++ $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -r -f $(TARGET) *.o *~ dyntmp*
