#!/bin/bash -e

# This file is part of P^nMPI.
#
# Copyright (c)
#  2008-2019 Lawrence Livermore National Laboratories, United States of America
#  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
#  2013-2019 RWTH Aachen University, Federal Republic of Germany
#
#
# P^nMPI is free software; you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation version 2.1 dated February 1999.
#
# P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with P^nMPI; if not, write to the
#
#   Free Software Foundation, Inc.
#   51 Franklin St, Fifth Floor
#   Boston, MA 02110, USA
#
#
# Written by Martin Schulz, schulzm@llnl.gov.
#
# LLNL-CODE-402774

# The following functions will help for folding the output. If this script will
# be called outside of Travis CI, these functions will generate no output.
#
# Below functions are adapted versions of the original ones of Travis CI,
# released under the MIT license.
#
#   Copyright (c) 2013 Konstantin Haase
#   Copyright (c) 2014-2015 Travis CI GmbH
#
#   Permission is hereby granted, free of charge, to any person obtaining a copy
#   of this software and associated documentation files (the "Software"), to
#   deal in the Software without restriction, including without limitation the
#   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
#   sell copies of the Software, and to permit persons to whom the Software is
#   furnished to do so, subject to the following conditions:
#
#   The above copyright notice and this permission notice shall be included in
#   all copies or substantial portions of the Software.
#
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
#   IN THE SOFTWARE.
#
fold_start() {
  if [ -n "$TRAVIS" ]
  then
    echo -ne "travis_fold:start:$1\r"

    travis_timer_id=$(printf %08x $(( RANDOM * RANDOM )))
    travis_start_time=$(travis_nanoseconds)
    echo -en "travis_time:start:$travis_timer_id\r${ANSI_CLEAR}"
  fi

  echo -e "\033[34;1m$2\033[0m"
}

fold_end() {
  if [ -n "$TRAVIS" ]
  then
    travis_end_time=$(travis_nanoseconds)
    local duration=$(($travis_end_time-$travis_start_time))
    echo -en "\ntravis_time:end:$travis_timer_id:start=$travis_start_time,"
    echo -en "finish=$travis_end_time,duration=$duration\r${ANSI_CLEAR}"

    echo -e "\ntravis_fold:end:$1\r"
  fi
}

travis_fold() {
  marker=$1
  fold_start $marker "$2"
  shift 2
  echo "\$ $@"
  eval $@
  fold_end $marker
}

function travis_nanoseconds() {
  local cmd="date"
  local format="+%s%N"
  local os=$(uname)

  if hash gdate > /dev/null 2>&1
  then
    # use gdate if available
    cmd="gdate"
  elif [[ "$os" = Darwin ]]
  then
    # fallback to second precision on darwin (does not support %N)
    format="+%s000000000"
  fi

  $cmd -u $format
}

echo_info() {
  echo -e "\033[33;1mINFO: $@\033[0m"
}

minify_args() {
    echo $1 | tr "\n" " " | sed 's/[[:space:]]+/ /g'
}


#
# Build preparation.
#
if [ -n "$CI" ]
then
  travis_fold script.0 'Preparation' 'mkdir travis; cd travis'

# If the 'CI' environment variable is not available, this script is run
# interactively. Gather some information about the system, which would be
# provided by Travis if run in CI.
else
  export TRAVIS_OS_NAME=$(uname | sed 's/Darwin/osx/g' \
                                | tr '[:upper:]' '[:lower:]')

  if [ -z "$CC" ]
  then
    export CC=$(which cc || which gcc || which clang)
    export CXX=$(which c++ || which g++ || which clang++)
  fi
fi


#
# Configuration.
#
fold_start script.1 'Configuration'

COMMON_FLAGS="-pipe
              -fstack-protector
              -Werror -Wall
             "

if [ "$CC" == gcc* ] || [ "$CC" == clang* ] || [ "$CC" == icc* ]
then
  export CFLAGS=$(minify_args "$COMMON_FLAGS $CFLAGS")
  export CPPFLAGS=$(minify_args "$COMMON_FLAGS $CPPFLAGS")
  export FFLAGS=$(minify_args "$COMMON_FLAGS $FFLAGS")
fi

if [ "$TRAVIS_OS_NAME" == "linux" ] && [ "$CC" == gcc* ]
then
  SANITIZE_FLAGS="-DSANITIZE_ADDRESS=On -DSANITIZE_UNDEFINED=On"
else
  echo_info "Address Sanitizer is supported for Linux GCC builds only."
fi

if [ "$CC" == gcc* ]
then
  COVERAGE_FLAGS="-DENABLE_COVERAGE=On"
else
  echo_info "Code coverage is disabled for $CC."
fi

echo "\$ cmake .. -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=Debug" \
  "-DENABLE_TESTS=On -DBUILD_DOC=On -DBUILD_DOC_INTERNAL=On"                  \
  "$SANITIZE_FLAGS $COVERAGE_FLAGS $CMAKE_EXTRA_FLAGS"
cmake .. -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_TESTS=On -DBUILD_DOC=On -DBUILD_DOC_INTERNAL=On          \
  $SANITIZE_FLAGS $COVERAGE_FLAGS $CMAKE_EXTRA_FLAGS

fold_end script.1


#
# Run the build and the test cases.
#
travis_fold script.2 'make all' make all
travis_fold script.3 'ctest --output-on-failure' ctest --output-on-failure
travis_fold script.4 'make install' make install
