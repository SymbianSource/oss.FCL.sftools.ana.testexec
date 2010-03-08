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
TARGET=		../../bin/hostexecute
INCLUDES= 	-I../include
OBJECTS=	CSHostexecute.o		\
		CSvcHostexecute.o	\
		hostexecute_svc.o	\
		hostexecute_svc_stub_impl.o	\
		hostexecute_xdr.o



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
