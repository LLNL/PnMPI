include $(PNMPIBASE)/common/Makefile.common

MOD    = timelapse.so
HEADER = timelapse.h

CFLAGS += -I$(PNMPI_INC_PATH)
CCFLAGS += -I$(PNMPI_INC_PATH)

all: $(MOD) install

timelapse.o: timelapse.h

.SUFFIXES: .c .o .so

.o.so: 
	$(CROSSLDXX) -o $@ $(SFLAGS) $<

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

