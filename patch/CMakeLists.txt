# TODO Martin please add the copyright statment of your choice, as well as 
#      a reference to the license or license file!
#
# @authors Tobias Hilbrich, Martin Schulz
# @date 07-07-2009

# TODO:
#       Should be detected, BFD presence can be tested for. 
IF ( LINUX )
   ADD_DEFINITIONS( "-DBFD_COPY" )
ELSE()
   ADD_DEFINITIONS( "-DDIRECT_COPY" )
ENDIF()

#Add the target for the patcher
ADD_EXECUTABLE(${TARGET_PATCHER} patch.c)
