# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

include $(PNMPIBASE)/common/Makefile.common

MOD    = datatype.so dtprint.so
HEADER = datatype.h

CFLAGS += -I$(PNMPI_INC_PATH)
CCFLAGS += -I$(PNMPI_INC_PATH)

all: $(MOD) install


.SUFFIXES: .c .o .so .cpp

.o.so: 
	$(CROSSLDXX) $(SFLAGS) -o $@ $<

.c.o:
	$(MPICC) -c $(CFLAGS) $<

.cpp.o:
	$(MPIXX) -c $(CFLAGS) $<

install: $(MOD)
	for mymod in $(MOD); do (../../patch/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod ); done
	for myheader in $(HEADER); do ( cp $$myheader $(PNMPI_INC_PATH)/$$myheader ); done

clean:
	rm -f $(MOD) *.o

clobber: clean
	rm -f *~
	for mymod in $(MOD); do ( rm $(PNMPI_LIB_PATH)/$$mymod ); done

