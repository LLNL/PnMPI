include $(PNMPIBASE)/common/Makefile.common

MOD    = sample1.so sample2.so sample3.so sample4.so sample1f.so

CFILES = $(MOD:.so=.c)
OFILES = $(MOD:.so=.o)

CFLAGS += -I$(PNMPI_INC_PATH) 

TARGET = $(MOD) install-d 


all: $(TARGET)


.SUFFIXES: .c .o .so .a

.o.a: 
	$(AR) $(ARFLAGS) $@ $<

.o.so: 
	$(CROSSLD) -o $@ $(SFLAGS) $< 

.c.o:
	$(MPICC) -c $(CFLAGS) $<

install-d: $(MOD)
	for mymod in $(MOD); do ($(PNMPIBASE)/patch/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod); done

clean:
	rm -f $(MOD) $(SMOD) *.o

clobber: clean
	rm -f *~
	for mymod in $(TARGET); do ( rm -f $(PNMPI_LIB_PATH)/$$mymod ); done

