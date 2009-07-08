# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# Based upon a Makefile with the following copyright statment:
## Generated automatically from Makefile.in by configure.
## (C) 1993 by Argonne National Laboratory and Mississipi State University.
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

ADD_DEFINITIONS("-DPROTO_FILE=\"../wrapper/mpi_proto_def\"")
ADD_EXECUTABLE(${TARGET_WRAPPERGEN} readproto.c wrappergen.c args.c)

 