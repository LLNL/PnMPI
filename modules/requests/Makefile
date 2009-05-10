include $(PNMPIBASE)/common/Makefile.common

MOD    = requests.so
HEADER = requests.h

CFLAGS += -I$(PNMPI_INC_PATH) -fPIC

all: $(MOD) install


.SUFFIXES: .c .o .so

.o.so: 
	$(CXX) -o $@ $(SFLAGS) $<

.c.o:
	$(MPICC) -c $(CFLAGS) $<

.cxx.o:
	$(MPIXX) -c $(CFLAGS) $<

install: $(MOD)
	for mymod in $(MOD); do (../../patch/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod ); done
	for myheader in $(HEADER); do ( cp $$myheader $(PNMPI_INC_PATH)/$$myheader ); done

clean:
	rm -f $(MOD) *.o

clobber: clean
	rm -f *~
	for mymod in $(MOD); do ( rm $(PNMPI_LIB_PATH)/$$mymod ); done

