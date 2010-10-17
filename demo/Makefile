# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

include $(PNMPIBASE)/common/Makefile.common

#TARGET = local loader
TARGET = simple simple-pn simple-pns simple-s1 fdemo fdemo-pn fdemo-pns # fdemo-s1

ifeq ($(findstring AIX,$(OS)),AIX)
PNMPILIB = $(PNMPI_LIB_PATH)/libpnmpi.so $(PNMPI_LIB_PATH)/forcempi.a
LFLAGS += -brtl
else
PNMPILIB = $(PNMPI_LIB_PATH)/libpnmpi.so
PNMPILIBF = $(PNMPI_LIB_PATH)/libpnmpif.so
PNMPILIBS = $(PNMPI_LIB_PATH)/libpnmpi.a
PNMPILIBFS = $(PNMPI_LIB_PATH)/libpnmpif.a
LFLAGS += 
endif

HERE   = $(PWD)

all: $(TARGET)

fdemo.o: fdemo.f
	$(MPIF77) -c $(FFLAGS) $<

fdemo: fdemo.o
	$(MPIF77) -o $@ $(LFLAGS) $<

fdemo-pn: $(PNMPILIB) fdemo.o
#	$(MPIF77) -o $@ $(LFLAGS)  fdemo.o $(PNMPILIBF) -ldl
	$(MPIF77) -o $@ $(LFLAGS)  fdemo.o -L$(PNMPI_LIB_PATH) -lpnmpif 

fdemo-pns: $(PNMPILIB) fdemo.o
	$(MPIF77) -o $@ $(LFLAGS)  fdemo.o $(PNMPILIBFS) -ldl

#fdemo-s1: fdemo.o
#	$(MPIF77) -o $@ $(LFLAGS) $< ../modules/sample/sample1f.so 

simple: simple.o
	$(MPICC) -o $@ $(LFLAGS) $< 

simple-pn: $(PNMPILIB) simple.o
	$(MPICC) -o $@  $(LFLAGS) simple.o -L$(PNMPI_LIB_PATH) -lpnmpi

simple-pns: $(PNMPILIB) simple.o
	$(MPICC) -o $@  $(LFLAGS) simple.o $(PNMPI_LIB_PATH)/libpnmpi.a -ldl

simple-s1: $(PNMPILIB) simple.o
	$(MPICC) -o $@  $(LFLAGS) simple.o ../modules/sample/sample1.so -ldl

simple.o: simple.c
	$(MPICC) -c $(CFLAGS) $<

nocore:
	rm -f *.core

clean: nocore
	rm -f $(TARGET) $(MPIPTARGET) *.o

clobber: clean
	rm -f *~ *.?.mpiP *core
