include $(PNMPIBASE)/common/Makefile.common

MOD    = empty.so


CFILES = $(MOD:.so=.c)
OFILES = $(MOD:.so=.o)

CFLAGS += -I../../include -fPIC

MPISPEC = mpi_pnmpi
WRAPDIR = ../../wrapper

PROTOFILE = $(WRAPDIR)/$(MPISPEC)_proto
FCTFILE   = $(WRAPDIR)/$(MPISPEC)_fct

WR = ../../wrappergen/wrappergen

TARGET = $(MOD) install-d

all: $(TARGET)


.SUFFIXES: .c .o .so .a

.o.a: 
	$(AR) $(ARFLAGS) $@ $<

.o.so: 
	$(LD) -o $@ $(SFLAGS) $< 

.c.o:
	$(MPICC) -c $(CFLAGS) $<


empty.h: empty.w
	$(WR) -p $(PROTOFILE) -f $(FCTFILE) -w $< -o $@

empty.c: empty.h

install-d: $(MOD)
	for mymod in $(MOD); do (../../patch/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod); done

clean:
	rm -f $(MOD) *.o *.a empty.h

clobber: clean
	rm -f *~
	for mymod in $(TARGET); do ( rm -f $(PNMPI_LIB_PATH)/$$mymod ); done
