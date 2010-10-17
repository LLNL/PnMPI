#!/bin/tcsh
# Copyright (c) 2008, Lawrence Livermore National Security, LLC. 
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

#!/bin/tcsh

setenv PROTO $2
setenv FCTS  $1

setenv NUM `wc -l $2 | awk '{print $1;}'`

echo $NUM

foreach f ( `cat $FCTS` )
setenv pf P$f
cat $PROTO | awk -v fct=$pf -f grep_fct | awk -f get_fct_ret | sed 's/PMPI/MPI/' 
cat $PROTO | awk -v fct=$pf -f grep_fct | ./extrargs
end



