#!/bin/tcsh

setenv PROTO $2
setenv FCTS  $1

setenv NUM `wc -l $2 | gawk '{print $1;}'`

echo $NUM

foreach f ( `cat $FCTS` )
setenv pf P$f
cat $PROTO | gawk -v fct=$pf -f grep_fct | gawk -f get_fct_ret | sed 's/PMPI/MPI/' 
cat $PROTO | gawk -v fct=$pf -f grep_fct | ./extrargs
end



