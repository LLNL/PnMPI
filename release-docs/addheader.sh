#!/bin/tcsh

setenv HEADER none

echo -n $1 ":"

if ( $1 =~ "*.c" ) then
echo -n Found C file
setenv HEADER c
endif

if ( $1 =~ "*.h" ) then
echo -n Found H file
setenv HEADER h
endif

if ( $1 =~ "*.f" ) then
echo -n Found F file
setenv HEADER f
endif

if ( $1 =~ "*.w" ) then
echo -n Found W file
setenv HEADER w
endif

if ( $1 =~ "*.py" ) then
echo -n Found PY file
setenv HEADER python
endif

if ( $1 =~ "*.sh" ) then
echo -n Found SH file
setenv HEADER sh
endif

if ( $1 =~ "*.cxx" ) then
echo -n Found CXX file
setenv HEADER c
endif

if ( $1 =~ "*.cpp" ) then
echo -n Found CPP file
setenv HEADER c
endif

if ( $1 =~ "*Makefile*" ) then
echo -n Found makefile
setenv HEADER make
endif

if ( $1 =~ "*makefile*" ) then
echo -n Found makefile
setenv HEADER make
endif

if ( $1 =~ "*CMakeLists.txt*" ) then
echo -n Found cmakefile
setenv HEADER cmake
endif

if ( $1 =~ "*README*" ) then
echo -n Found README
setenv HEADER cmake
endif

if ( $1 =~ "*.cmake" ) then
echo -n Found cmakefile
setenv HEADER cmake
endif

if ( $1 =~ "*.pnmpi-conf*" ) then
echo -n Found PnMPI file
setenv HEADER pnmpi
endif

setenv HF $PNMPIBASE/release-docs/header.$HEADER

if ( -e $HF ) then

setenv CURRENT "`grep LLNL-CODE- $1 | head -1`"

if ( "$CURRENT" =~ "*LLNL-CODE-*" ) then
echo ":" $1 already has a header
else
echo -n ":" Adding $HEADER header
cp $1 $1.headertoadd.tmp
if ( $1 =~ "*.sh" ) then
head -1 $1.headertoadd.tmp > $1
cat $HF $1.headertoadd.tmp >> $1
else
cat $HF $1.headertoadd.tmp > $1
endif
rm $1.headertoadd.tmp

if ( $1 =~ "*.sh" ) then
echo ":" Found Executable Script
chmod u+x $1
else
echo " "
endif

endif

else

echo No header for $1

endif

