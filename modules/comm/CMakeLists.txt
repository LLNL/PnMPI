# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

#Set names for targets
SET (PREFIX "commsub")
SET (SUBMOD "print" "empty") #"checksum" ##TODO checksum removed due to incompatibility with OpenMPI

#Loop over all targets (submods)
FOREACH(mod ${SUBMOD})
    #Use macro to: add target for module, version it, install it, patch it
    PNMPI_MAC_ADD_MODULE (${PREFIX}-${mod} "${PREFIX}-${mod}.cxx;comm.cxx")
    
    #Add the submodname define as a flag
    ##TODO Martin, please have a look at the "SUBMODNAME" define, is that what you want ???
    SET_SOURCE_FILES_PROPERTIES(${PREFIX}-${mod}.cxx COMPILE_FLAGS "-DSUBMODNAME='\"${mod}\"'")
ENDFOREACH(mod)
