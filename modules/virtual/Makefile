# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

include $(PNMPIBASE)/common/Makefile.common

MOD    = virtual.so

MPISPEC = mpi_pnmpi
#MPISPEC = mpi_def
WRAPDIR = ../../wrapper

PROTOFILE = $(WRAPDIR)/$(MPISPEC)_proto
FCTFILE   = $(WRAPDIR)/$(MPISPEC)_fct

WRAPPERC = wrapper_c.w
WRAPPERH = wrapper_h.w

WR = ../../wrappergen/wrappergen

CFLAGS += -I$(PNMPI_INC_PATH) -fPIC
CCFLAGS += -I$(PNMPI_INC_PATH) -fPIC

all: $(MOD) install


virtual.so: virtual.o
	$(CROSSLD) -o $@ $(SFLAGS) $<

virtual.o: virtual.c virtual.h
	$(MPICC) -c $(CFLAGS) $<

virtual.h: virtual.w
	$(WR) -p $(PROTOFILE) -f $(FCTFILE) -w $< -o $@

install: $(MOD)
	for mymod in $(MOD); do (../../patch/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod ); done

clean:
	rm -f $(MOD) *.o virtual.h

clobber: clean
	rm -f *~
