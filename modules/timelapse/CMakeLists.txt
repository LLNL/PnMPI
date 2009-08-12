# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Use macro to: add target for module, version it, install it, patch it
PNMPI_MAC_ADD_MODULE (timelapse "timelapse.c")

#Add header to intermediate include dir
CONFIGURE_FILE(timelapse.h "${INCLUDE_OUTPUT_PATH}/timelapse.h" COPYONLY)
