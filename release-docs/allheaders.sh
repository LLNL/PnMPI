# Copyright (c) 2008, Lawrence Livermore National Security, LLC.
# Written by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774,
# All rights reserved - please read information in "LICENCSE"

#!/bin/tcsh

cd $PNMPIBASE

foreach f ( `find . | grep -v -e ".git"` )

release-docs/addheader.sh $f

end

