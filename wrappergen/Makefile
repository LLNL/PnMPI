# Generated automatically from Makefile.in by configure.
#  (C) 1993 by Argonne National Laboratory and Mississipi State University.
#

include $(PNMPIBASE)/common/Makefile.common

EXECS = wrappergen write_proto
MPI_PROTO = '-DPROTO_FILE="../wrapper/mpi_proto_def"'

default: wrappergen

all: $(EXECS)

readproto.o: readproto.c wrappergen.h
	$(CC) $(CFLAGS) $(MPI_PROTO) -c $*.c

wrappergen.o: wrappergen.c wrappergen.h
	$(CC) $(CFLAGS) -c $*.c

wrappergen: readproto.o wrappergen.o args.o
	$(LD) -o wrappergen $(CFLAGS) readproto.o \
	   wrappergen.o args.o

clean:
	-rm -rf $(EXECS) *.o

clobber: clean
	-rm *~
