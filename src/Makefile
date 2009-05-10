include $(PNMPIBASE)/common/Makefile.common

ifeq ($(findstring AIX,$(OS)),AIX)
TARGET = libpnmpi.so libpnmpi.a libpnmpif.so libpnmpif.a forcempi.a
else
TARGET = libpnmpi.so libpnmpi.a libpnmpif.so libpnmpif.a 
endif

OBJS   = core.o wrapper.o debug.o services.o
FOBJS   = core.o wrapperf.o debug.o services.o
INCS   = -I../include # -DNOSTATUS
DEPS   = pnmpi.h pnmpi_int.h wrapper.h Makefile

HEADER = pnmpi.h pnmpimod.h

all: $(TARGET) install

#MPICC = purify gcc-3.4.4
ifeq ($(findstring AIX,$(OS)),AIX)
CFLAGS += -fPIC -G
else
CFLAGS += -fPIC
endif

core.o: core.h wrapper.h pnmpimod.h debug.h
debug.o: debug.h
wrapper.o: core.h ../wrapper/wrapper.c ../wrapper/wrapper.h debug.h ../wrapper/f77symbols.h ../wrapper/wrapper_f77.c
wrapperf.o: core.h ../wrapper/wrapper.c ../wrapper/wrapper.h debug.h ../wrapper/f77symbols.h ../wrapper/wrapper_f77.c
	$(MPICC) -c $(CFLAGS) $(INCS) wrapper.c -o wrapperf.o
debug.o: core.h debug.h
services.c: pnmpimod.h core.h debug.h

vargcall.c: create-vargcall.py
	python create-vargcall.py > vargcall.c

../wrapper/wrapper.h: ../wrapper/wrapper_h.w
	cd ../wrapper ; $(MAKE) ; cd -

../wrapper/wrapper.c ../wrapper/f77symbols.h ../wrapper/wrapper_f77.c: ../wrapper/wrapper_c.w vargcall.c
	cd ../wrapper ; $(MAKE) ; cd -

.c.o: $(DEPS)
	$(MPICC) -c $(CFLAGS) $(INCS) $<

ifeq ($(findstring AIX,$(OS)),AIX)
libpnmpi.so: $(OBJS)
	$(MPIXX) -o $@ $(SFLAGS) $(OBJS)

forcempi.o: forcempi.c
	$(CC) -c $<

forcempi.a: forcempi.o
	$(AR) $(ARFLAGS) $@ $<
else
ifdef EXPERIMENTAL_UNWIND
libpnmpi.so: $(OBJS)
	$(MPIXX) $(OBJS) -o $@ $(SFLAGS) -lunwind -ldl
else
libpnmpi.so: $(OBJS)
	$(MPIXX) $(OBJS) -o $@ $(SFLAGS) -ldl
endif
endif

ifeq ($(findstring AIX,$(OS)),AIX)
libpnmpif.so: $(FOBJS)
	$(MPIF77) -o $@ $(SFLAGS) $(FOBJS)

forcempi.o: forcempi.c
	$(CC) -c $<

forcempi.a: forcempi.o
	$(AR) $(ARFLAGS) $@ $<
else
ifdef EXPERIMENTAL_UNWIND
libpnmpif.so: $(FOBJS)
	$(MPIF77) $(FOBJS) -o $@ $(SFLAGS) -lunwind -ldl
else
libpnmpif.so: $(FOBJS)
	$(MPIF77) $(FOBJS) -o $@ $(SFLAGS) -ldl
endif
endif

libpnmpi.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

libpnmpif.a: $(FOBJS)
	$(AR) $(ARFLAGS) $@ $(FOBJS)

install: $(TARGET)
	cp $(TARGET) $(PNMPI_LIB_PATH)
	for myheader in $(HEADER); do ( cp $$myheader $(PNMPI_INC_PATH)/$$myheader ); done

clean:
	rm -f $(TARGET) *.o

clobber: clean
	rm -f *~

