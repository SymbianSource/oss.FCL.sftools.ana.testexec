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

BINDIR=		../../bin/
TARGET=		../../bin/uuservice
INCLUDES= 	-I../include

CPSOURCE_PERL=	$(wildcard *.pl)
CPTARGET_PERL=	$(patsubst %.pl,$(BINDIR)/%.pl,$(CPSOURCE_PERL))
CPSOURCE=	$(CPSOURCE_PERL) Load.Nistnet
CPTARGET=	$(CPTARGET_PERL) $(BINDIR)/Load.Nistnet

OBJECTS=	CSUuinterface.o		\
		CSvcUuinterface.o		\
		UUInterface_svc.o		\
		uuinterface_svc_stub_impl.o	\
		UUInterface_xdr.o		


LIBS=		-L../../BinInternal -lproc

##########################################################################
#
# Rules
#
##########################################################################
all:	$(TARGET) $(CPTARGET)

$(TARGET):	$(OBJECTS) ../../BinInternal/libproc.a
	g++ -gstabs -Wall -o $(TARGET) $(OBJECTS) $(LIBS)

$(CPTARGET):	$(CPSOURCE)
	echo $(CPTARGET)
	cp $(CPSOURCE) $(BINDIR)

%.o:	%.c
	gcc -gstabs -Wall $(INCLUDES) -c $< -o $@

%.o:	%.cpp
	g++ -gstabs -Wall $(INCLUDES) -c $< -o $@

clean:
	rm -r -f $(TARGET) *.o *~ dyntmp* $(CPTARGET)
