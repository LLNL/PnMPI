# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#modules to build:
SET(MODS "sample1" "sample2" "sample3" "sample4" "sample1f")

#loop over modules and build them
FOREACH (mod ${MODS})
    #Use macro to: add target for module, version it, install it, patch it
    PNMPI_MAC_ADD_MODULE (${mod} "${mod}.c")
ENDFOREACH (mod ${MODS})

#Create special non-patched version of sample1 (as SHARED and not as MODULE)
#Used as part of the demo
ADD_LIBRARY(sample1-non-patched SHARED sample1.c)
