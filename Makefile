# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

TARGET = prep d_patch d_wrappergen d_wrapper d_src d_modules d_demo

all: $(TARGET)

prep:
	mkdir -p lib
	mkdir -p $(PNMPI_LIB_PATH)
	mkdir -p $(PNMPI_INC_PATH)

d_patch:
	cd patch; $(MAKE)

d_wrappergen:
	cd wrappergen; $(MAKE)

d_wrapper:
	cd wrapper; $(MAKE)

d_src:
	cd src; $(MAKE)

d_modules:
	cd modules; $(MAKE)

d_demo:
	cd demo; $(MAKE)

clean:
	cd patch; $(MAKE) clean
	cd wrappergen; $(MAKE) clean
	cd wrapper; $(MAKE) clean
	cd src; $(MAKE) clean
	cd modules; $(MAKE) clean
	cd demo; $(MAKE) clean

clobber:
	cd patch; $(MAKE) clobber
	cd wrappergen; $(MAKE) clobber
	cd wrapper; $(MAKE) clobber
	cd src; $(MAKE) clobber
	cd modules; $(MAKE) clobber
	cd demo; $(MAKE) clobber
	rm -f *~
