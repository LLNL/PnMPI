# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

include $(PROJECTS)/common/Makefile.common

MOD    = switch-matrix.so

MPISPEC = mpi_pnmpi
#MPISPEC = mpi_def
WRAPDIR = ../../wrapper

PROTOFILE = $(WRAPDIR)/$(MPISPEC)_proto
FCTFILE   = $(WRAPDIR)/$(MPISPEC)_fct

WRAPPERC = wrapper_c.w
WRAPPERH = wrapper_h.w

WR = ../../wrappergen/wrappergen

CFLAGS += -I../../include

all: $(MOD) install


switch-matrix.so: switch-matrix.o
	$(CROSSLD) -o $@ $(SFLAGS) $<

switch-matrix.o: switch-matrix.c switch-matrix.h
	$(MPICC) -c $(CFLAGS) $<

switch-matrix.h: switch-matrix.w
	$(WR) -p $(PROTOFILE) -f $(FCTFILE) -w $< -o $@

install: $(MOD)
	for mymod in $(MOD); do (../../patch/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod ); done

clean:
	rm -f $(MOD) *.o switch-matrix.h

clobber: clean
	rm -f *~
