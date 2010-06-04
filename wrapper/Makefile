include $(PNMPIBASE)/common/Makefile.common

MPISPEC = mpi_pnmpi
#MPISPEC = mpi_def

PROTOFILE = $(MPISPEC)_proto
FCTFILE   = $(MPISPEC)_fct

WRAPPERC = wrapper_c.w
WRAPPERH = wrapper_h.w
NEWSTACK = newstack_h.w

WR = ../wrappergen/wrappergen 

TARGET = wrapper.c wrapper.h newstack.h wrapper_f77.c f77symbols.h

all: $(TARGET)

dummy.fct: dummy.c removeattr
	$(MPICC) -E dummy.c | ./removeattr | gawk -f join_lines | grep -e "PMPI_" | grep -v "_FN" | grep -v f2c | grep -v c2f | sed 's/(/ (/' | grep -v PMPI_Pcontrol | grep -v PMPI_Type_count > dummy.fct

wrapper.c: $(PROTOFILE) $(FCTFILE) $(WRAPPERC)
	$(WR) -p $(PROTOFILE) -f $(FCTFILE) -w $(WRAPPERC) -o wrapper.c

wrapper.h: $(PROTOFILE) $(FCTFILE) $(WRAPPERH)
	$(WR) -p $(PROTOFILE) -f $(FCTFILE) -w $(WRAPPERH) -o wrapper.h

newstack.h: $(PROTOFILE) $(FCTFILE) $(NEWSTACK)
	$(WR) -p $(PROTOFILE) -f $(FCTFILE) -w $(NEWSTACK) -o newstack.h
	cp newstack.h ../src/
	cp newstack.h ../include/

wrapper_f77.c f77_symbol.h: mpi-f77.txt makefortran.py
	python makefortran.py --xlate --f77symbol $(F77SYMBOL) mpi-f77.txt

mpi_pnmpi_fct-nopctrl: dummy.fct 
	cat dummy.fct | gawk -f get_fct | sed 's/PMPI/MPI/' | sort | uniq > mpi_pnmpi_fct-nopctrl

mpi_pnmpi_fct: mpi_pnmpi_fct-nopctrl
	gawk 'BEGIN{print "MPI_Pcontrol"}{print}'< mpi_pnmpi_fct-nopctrl > mpi_pnmpi_fct

mpi_pnmpi_proto: dummy.fct mpi_pnmpi_fct-nopctrl extrargs
	./generate_protp_file.sh mpi_pnmpi_fct-nopctrl dummy.fct | gawk -f add-pctrl> mpi_pnmpi_proto

extrargs: extrargs.o
	$(CC) -o $@ -g $<

extrargs.o: extrargs.c
	$(CC) -c $< -g

removeattr: removeattr.o
	$(CC) -o $@ -g $<

removeattr.o: removeattr.c
	$(CC) -c $< -g

clean:
	rm -f dummy.fct $(PROTOFILE) $(FCTFILE) $(TARGET) extrarg removeattr *.o

clobber: clean
	rm -f *~


