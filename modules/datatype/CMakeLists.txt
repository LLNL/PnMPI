# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Set names for targets
SET (MODS "datatype" "dtprint")

#Loop over all targets (submods)
FOREACH(mod ${MODS})
    #Use macro to: add target for module, version it, install it, patch it
    PNMPI_MAC_ADD_MODULE (${mod} "${mod}")
ENDFOREACH(mod)

#Add header to intermediate include dir
CONFIGURE_FILE(datatype.h "${INCLUDE_OUTPUT_PATH}/datatype.h" COPYONLY)
