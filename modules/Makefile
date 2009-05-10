MODS = d_sample d_empty d_datatype d_status d_requests d_comm d_switch-matrix d_virtual d_timelapse

all: $(MODS)

d_sample:
	cd sample ; $(MAKE)  ; cd -

d_empty:
	cd empty ; $(MAKE)  ; cd -

d_switch-matrix:
	cd switch-matrix; $(MAKE)  ; cd -

d_virtual:
	cd virtual; $(MAKE) ; cd -

d_comm:
	cd comm ; $(MAKE)  ; cd -

d_requests:
	cd requests ; $(MAKE)  ; cd -

d_datatype:
	cd datatype ; $(MAKE)  ; cd -

d_status:
	cd status ; $(MAKE)  ; cd -

d_timelapse:
	cd timelapse ; $(MAKE)  ; cd -


clean:
	cd sample ; $(MAKE) clean ; cd -
	cd empty ; $(MAKE) clean ; cd -
	cd datatype ; $(MAKE) clean ; cd -
	cd status ; $(MAKE) clean ; cd -
	cd requests ; $(MAKE) clean ; cd -
	cd comm ; $(MAKE) clean ; cd -
	cd switch-matrix ; $(MAKE) clean ; cd -
	cd virtual ; $(MAKE) clean ; cd -

clobber:
	cd sample ; $(MAKE) clobber ; cd -
	cd empty ; $(MAKE) clobber ; cd -
	cd comm ; $(MAKE) clobber ; cd -
	cd datatype ; $(MAKE) clobber ; cd -
	cd requests ; $(MAKE) clobber ; cd -
	cd status ; $(MAKE) clobber ; cd -
	cd switch-matrix ; $(MAKE) clobber ; cd -
	cd virtual ; $(MAKE) clobber ; cd -


