#!/bin/sh

# This file is part of P^nMPI.
#
# Copyright (c)
#  2008-2017 Lawrence Livermore National Laboratories, United States of America
#  2011-2017 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
#  2013-2017 RWTH Aachen University, Federal Republic of Germany
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

# This helper script packs a full archive including all submodules and a valid
# version file for deployment. The git archive command can't be used here, as
# it does not pack the submodules, neither it can include the version file.


# First check if git archive-all is available. If not print an error message and
# exit this script.
if [ ! -n "$(which git-archive-all)" ]
then
  echo "Error: Can't find git-archive-all in your PATH." >&2
  exit 1
fi


# Get the current version of PnMPI and store it in the .version file. This file
# will be deleted automatically, if the script exits either gracefully or in any
# error condition.
git describe --tags > .version
finish()
{
  rm -f ".version"
}
trap finish EXIT


# Pack PnMPI repository with all submodules and the version file.
#
# Note: Extra flags passed to this file will be passed to 'git archive-all', so
#       one may activate e.g. the verbose mode.
version=$(git describe --tags --abbrev=0)
git archive-all             \
  --force-submodules        \
  --prefix="PnMPI-$version" \
  --extra .version          \
  $@                        \
  "PnMPI-$version-full.tar.gz"
