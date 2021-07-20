include $(PNMPIBASE)/common/Makefile.common

MOD    = pbdriver.so \
	 pb_datatype1.so pb_datatype2.so \
	 pb_twomsg1.so pb_twomsg2.so \
	 pb_pack1.so pb_pack2.so\
         pb_copy1.so pb_copy2.so
HEADER = pb_mod.h 

CFLAGS += -I$(PNMPI_INC_PATH) 
CCFLAGS += -I$(PNMPI_INC_PATH)

all: $(MOD) install

pb_dt.o: 

.SUFFIXES: .c .o .so

.o.so: 
	$(CROSSLDXX) -o $@ $(SFLAGS) $< -ma

.c.o:
	$(MPICC) -c $(CFLAGS) $<

.cpp.o:
	$(MPIXX) -c $(CFLAGS) $<

pb_datatype1.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_DATATYPE1 $<
	mv piggyback.o pb_datatype1.o

pb_datatype2.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_DATATYPE2 $<
	mv piggyback.o pb_datatype2.o

pb_twomsg1.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_TWOMSG1 $<
	mv piggyback.o pb_twomsg1.o

pb_twomsg2.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_TWOMSG2 $<
	mv piggyback.o pb_twomsg2.o

pb_pack1.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_PACK1 $<
	mv piggyback.o pb_pack1.o

pb_pack2.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_PACK2 $<
	mv piggyback.o pb_pack2.o

pb_copy1.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_COPY1 $<
	mv piggyback.o pb_copy1.o

pb_copy2.o: piggyback.c
	$(MPICC) -c $(CFLAGS) -DPB_COPY2 $<
	mv piggyback.o pb_copy2.o


install: $(MOD)
	for mymod in $(MOD); do ($(LOCAL)/pnmpi/bin/patch $$mymod $(PNMPI_LIB_PATH)/$$mymod ); done
	for myheader in $(HEADER); do ( cp $$myheader $(PNMPI_INC_PATH)/$$myheader ); done

clean:
	rm -f $(MOD) *.o

clobber: clean
	rm -f *~
	for mymod in $(MOD); do ( rm $(PNMPI_LIB_PATH)/$$mymod ); done

