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

CFLAGS += -I../../include -fPIC

all: $(MOD) install


switch-matrix.so: switch-matrix.o
	$(MPICC) -o $@ $(SFLAGS) $<

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
