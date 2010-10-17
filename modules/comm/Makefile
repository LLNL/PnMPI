# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

include $(PNMPIBASE)/common/Makefile.common

PREFIX = commsub

SUBMOD = print checksum empty

OFILES = $(addsuffix .o, $(addprefix $(PREFIX)-,$(SUBMOD)))
CFILES = $(addsuffix .cpp, $(addprefix $(PREFIX)-,$(SUBMOD)))
MOD    = $(addsuffix .so,$(addprefix $(PREFIX)-,$(SUBMOD)))


CFLAGS += -I$(PNMPI_INC_PATH) -I$(PNMPIBASE)/modules/comm
CCFLAGS += -I$(PNMPI_INC_PATH) -I$(LOCAL)/include -I$(OPENSS_BASE)/include -I$(PNMPIBASE)/modules/comm


all: $(MOD) install

$(OFILES): $(PNMPIBASE)/modules/comm/comm.o

.SUFFIXES: .c .o .so .cpp

.o.so: $(PNMPIBASE)/modules/comm/comm.o
	$(CROSSLDXX) -o $@ $< comm.o $(SFLAGS)

.c.o:
	$(MPICC) -c $(CFLAGS) -DSUBMODNAME=\"$(basename $(subst $(PREFIX)-,,$@))\" $<

.cpp.o:
	$(MPIXX) -c $(CCFLAGS) -DSUBMODNAME=\"$(basename $(subst $(PREFIX)-,,$@))\" $<

$(PNMPIBASE)/modules/comm/comm.o: $(PNMPIBASE)/modules/comm/comm.cpp
	$(MPIXX) -c $(CCFLAGS) $< -o $@

install: $(MOD)
	for mymod in $(MOD); do (../../patch/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod ); done
#	for myheader in $(HEADER); do ( cp $$myheader $(PNMPI_INC_PATH)/$$myheader ); done

clean:
	rm -f $(MOD) *.o

clobber: clean
	rm -f *~
	for mymod in $(MOD); do ( rm $(PNMPI_LIB_PATH)/$$mymod ); done

