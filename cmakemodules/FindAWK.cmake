# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"
#
# - Find AWK
# This module looks for AWK.
# It will set the following variables:
#  AWK = the awk binary
#
# @author Tobias Hilbrich
#

#TODO: should be a bit more advanced maybe
FIND_PROGRAM (AWK NAMES awk gawk PATH /usr/bin /bin /local/bin /usr/local/bin)
MARK_AS_ADVANCED(FORCE AWK)
SET (AWK_FOUND TRUE)
