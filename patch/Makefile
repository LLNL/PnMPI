# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

include $(PNMPIBASE)/common/Makefile.common

TARGET = patch

all: $(TARGET)

ifeq ($(findstring AIX,$(OS)),AIX)
CFLAGS += -I/usr/global/tools/gnu/binutils/aix_5_ll/binutils-2.15-64bit/include/ 
CFLAGS += -D$(OS)
#LFLAGS += -L/usr/global/tools/gnu/binutils/aix_5_ll/binutils-2.15-64bit/lib/
else
CFLAGS += -D$(OS)
endif


DYNLFLAGS += $(LFLAGS)
ifeq ($(findstring Linux,$(OS)),Linux)
CFLAGS += -DBFD_COPY
DYNLFLAGS += -lbfd -liberty
else
CFLAGS += -DDIRECT_COPY
endif

patch.o: patch.c
	$(CC) -c $(CFLAGS) $< 

patch: patch.o
	$(LD) -o $@ $< $(DYNLFLAGS)

clean:
	rm -f $(TARGET) *.o

clobber: clean
	rm -f *~
