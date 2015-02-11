# Makefile for phold sample code
NSDIR   = 3.21
NSVER   = 3.21
NS3DIR  = $(HOME)/repos/ns-3-allinone/ns-$(NSDIR)
INCLUDE = $(NS3DIR)/build
LIB     = $(NS3DIR)/build

LIBSDEB = -L$(LIB) \
-lmpi \
-lns$(NSVER)-applications-debug \
-lns$(NSVER)-core-debug \
-lns$(NSVER)-config-store-debug \
-lns$(NSVER)-internet-debug \
-lns$(NSVER)-mobility-debug \
-lns$(NSVER)-netanim-debug \
-lns$(NSVER)-stats-debug \
-lns$(NSVER)-network-debug \
-lns$(NSVER)-mpi-debug

LIBSOPT = -L$(LIB) \
-lmpi \
-lns$(NSVER)-applications-optimized \
-lns$(NSVER)-core-optimized \
-lns$(NSVER)-config-store-optimized \
-lns$(NSVER)-internet-optimized \
-lns$(NSVER)-mobility-optimized \
-lns$(NSVER)-netanim-optimized \
-lns$(NSVER)-stats-optimized \
-lns$(NSVER)-network-optimized \
-lns$(NSVER)-mpi-optimized

all:	optimized

optimized:	phold.o InputImage.o
	mpic++ -g -o phold phold.o InputImage.o $(LIBSOPT)

debug:	phold.o InputImage.o
	mpic++ -g -o phold phold.o InputImage.o $(LIBSDEB)

phold.o:	phold.cc
	mpic++ -c -g -I$(INCLUDE) phold.cc

InputImage.o: InputImage.cc
	mpic++ -c -g InputImage.cc

clean:
	rm -f *.o phold
