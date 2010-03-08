##########################################################################
#
# Makefile for ...
#
##########################################################################
include /home/penuser/3GNetworkEmulator/Makefile.conf

##########################################################################
#
# Definitions
#
##########################################################################
TARGET=../../bin/mncontroller
CFLAGS=		-Wall -gstabs #-ftest-coverage -fprofile-arcs
INCLUDES= -I../include/
OBJECTS=	CSMncontroller.o		\
		CSvcMncontroller.o		\
		mncontroller_svc.o		\
		mncontroller_svc_stub_impl.o	\
		mncontroller_xdr.o		

LIBS=-L../../BinInternal/ -lproc -ldynamicsconfig -ldynwrap

##########################################################################
#
# Rules
#
##########################################################################
$(TARGET):	$(OBJECTS) ../../BinInternal/libproc.a ../../BinInternal/libdynamicsconfig.a ../../BinInternal/libdynwrap.a
	g++ $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o:	%.c
	gcc $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o:	%.cpp
	g++ $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -r -f $(TARGET) *.o *~ dyntmp* *.bb *.bbg *.gcov
