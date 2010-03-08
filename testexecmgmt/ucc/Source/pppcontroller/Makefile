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
TARGET=../../bin/pppcontroller
INCLUDES=	-I../include
OBJECTS=	CSPppcontroller.o		\
		CSvcPppcontroller.o		\
		pppcontroller_svc.o		\
		pppcontroller_svc_stub_impl.o	\
		pppcontroller_xdr.o		

LIBS= -L../../BinInternal/ -L../../ExternalSource/binaries/ -lproc -ldynamicsconfig

##########################################################################
#
# Rules
#
##########################################################################
$(TARGET):	$(OBJECTS) ../../BinInternal/libproc.a ../../BinInternal/libdynamicsconfig.a
	g++ -Wall -gstabs -o $(TARGET) $(OBJECTS) $(LIBS)

%.o:	%.c
	gcc -Wall -gstabs $(INCLUDES) -c $< -o $@

%.o:	%.cpp
	g++ -Wall -gstabs $(INCLUDES) -c $< -o $@

clean:
	rm -r -f $(TARGET) *.o *~ dyntmp*
