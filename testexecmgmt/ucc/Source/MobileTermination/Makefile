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
TARGET=../../BinInternal/libmt.a
INCLUDES=
OBJECTS=	CDatalinkNull.o 		\
		CDatalinkPacketise.o		\
	 	CLog.o				\
		CLogPPPFilter.o			\
		CPhone.o			\
		CPppFrame.o			\
		CTCPTEChannel.o			\
		CUDPAirInterface.o	
LIBS=

##########################################################################
#
# Rules
#
##########################################################################
$(TARGET):	$(OBJECTS)
	ar -rcs $(TARGET) $(OBJECTS)

%.o:	%.c
	gcc -gstabs -Wall $(INCLUDES) -c $< -o $@

%.o:	%.cpp
	g++ -gstabs -Wall $(INCLUDES) -c $< -o $@

clean:
	rm -r -f $(TARGET) *.o *~
