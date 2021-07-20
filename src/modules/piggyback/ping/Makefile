include $(PNMPIBASE)/common/Makefile.common

#TARGET = local loader
TARGET = pingpong pingpong-pn pingpongdt pingpong1dt datacreate

ifeq ($(findstring AIX,$(OS)),AIX)
PNMPILIB = $(PNMPI_LIB_PATH)/libpnmpi.so $(PNMPI_LIB_PATH)/forcempi.a
LFLAGS += -brtl
else
PNMPILIB = $(PNMPI_LIB_PATH)/libpnmpi.so
#LFLAGS += -lunwind
endif

HERE   = $(PWD)

all: $(TARGET)

pingpong: pingpong.o
	$(MPICC) -o $@ $(LFLAGS) $< 

pingpongdt: pingpongdt.o
	$(MPICC) -o $@ $(LFLAGS) $< 

pingpong1dt: pingpong1dt.o
	$(MPICC) -o $@ $(LFLAGS) $< 

datacreate: datacreate.o
	$(MPICC) -o $@ $(LFLAGS) $< 

pingpong-pn: $(PNMPILIB) pingpong.o
	$(MPICC) -o $@  $(LFLAGS) pingpong.o -L$(PNMPI_LIB_PATH) -lpnmpi

pingpong.o: pingpong.c
	$(MPICC) -c $(CFLAGS) $<

pingpongdt.o: pingpongdt.c
	$(MPICC) -c $(CFLAGS) $<

pingpong1dt.o: pingpong1dt.c
	$(MPICC) -c $(CFLAGS) $<

datacreate.o: datacreate.c
	$(MPICC) -c $(CFLAGS) $<

nocore: 
	rm -f *.core *v3breakpoints

clean: nocore
	rm -f $(TARGET) $(MPIPTARGET) *.o

clobber: clean
	rm -f *~ *.?.mpiP *core
