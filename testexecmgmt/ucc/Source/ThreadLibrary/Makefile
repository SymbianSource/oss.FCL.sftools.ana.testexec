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
TARGET=../../BinInternal/libthread.a
INCLUDES=
OBJECTS=	CAThread.o
LIBS=

##########################################################################
#
# Rules
#
##########################################################################
$(TARGET):	$(OBJECTS) 
	ar -rcs $(TARGET) $(OBJECTS)

CAThread.o:	CAThread.cpp CAThread.linux.cpp
	g++ -gstabs -Wall $(INCLUDES) -c $< -o $@

%.o:	%.c
	gcc -gstabs -Wall $(INCLUDES) -c $< -o $@

%.o:	%.cpp
	g++ -gstabs -Wall $(INCLUDES) -c $< -o $@

clean:
	rm -r -f $(TARGET) *.o *~
