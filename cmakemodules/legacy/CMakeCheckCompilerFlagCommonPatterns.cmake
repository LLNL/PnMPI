#=============================================================================
# CMake - Cross Platform Makefile Generator
#
# Copyright 2006-2011 Kitware, Inc.
#
# Copyright 2006 Alexander Neundorf <neundorf@kde.org>
# Copyright 2011 Matthias Kretz <kretz@kde.org>
# Copyright 2013 Rolf Eike Beer <eike@sf-mail.de>
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the names of Kitware, Inc., the Insight Software Consortium, nor the
#   names of their contributors may be used to endorse or promote products
#   derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# ------------------------------------------------------------------------------
#
# The above copyright and license notice applies to distributions of CMake in
# source and binary form. Some source files contain additional notices of
# original copyright by their contributors; see each source for details.
# Third-party software packages supplied with CMake under compatible licenses
# provide their own copyright notices documented in corresponding
# subdirectories.
#
# ------------------------------------------------------------------------------
#
# CMake was initially developed by Kitware with the following sponsorship:
#
# * National Library of Medicine at the National Institutes of Health as part of
#   the Insight Segmentation and Registration Toolkit (ITK).
#
# * US National Labs (Los Alamos, Livermore, Sandia) ASC Parallel Visualization
#   Initiative.
#
# * National Alliance for Medical Image Computing (NAMIC) is funded by the
#   National Institutes of Health through the NIH Roadmap for Medical Research,
#   Grant U54 EB005149.
#
# * Kitware, Inc.

# Do NOT include this module directly into any of your code. It is meant as
# a library for Check*CompilerFlag.cmake modules. It's content may change in
# any way between releases.

# Try to include a recent version provided by the CMake installation. If no
# Module is available, use this file as fallback.
include("${CMAKE_ROOT}/Modules/CMakeCheckCompilerFlagCommonPatterns.cmake"
  OPTIONAL RESULT_VARIABLE INCLUDED)
if (INCLUDED)
  return()
else ()
  message(STATUS "Using legacy CMakeCheckCompilerFlagCommonPatterns.cmake")
endif ()


macro (CHECK_COMPILER_FLAG_COMMON_PATTERNS _VAR)
   set(${_VAR}
     FAIL_REGEX "[Uu]nrecogni[sz]ed .*option"               # GNU, NAG
     FAIL_REGEX "unknown .*option"                          # Clang
     FAIL_REGEX "optimization flag .* not supported"        # Clang
     FAIL_REGEX "ignoring unknown option"                   # MSVC, Intel
     FAIL_REGEX "warning D9002"                             # MSVC, any lang
     FAIL_REGEX "option.*not supported"                     # Intel
     FAIL_REGEX "invalid argument .*option"                 # Intel
     FAIL_REGEX "ignoring option .*argument required"       # Intel
     FAIL_REGEX "ignoring option .*argument is of wrong type" # Intel
     FAIL_REGEX "[Uu]nknown option"                         # HP
     FAIL_REGEX "[Ww]arning: [Oo]ption"                     # SunPro
     FAIL_REGEX "command option .* is not recognized"       # XL
     FAIL_REGEX "command option .* contains an incorrect subargument" # XL
     FAIL_REGEX "not supported in this configuration. ignored"       # AIX
     FAIL_REGEX "File with unknown suffix passed to linker" # PGI
     FAIL_REGEX "[Uu]nknown switch"                         # PGI
     FAIL_REGEX "WARNING: unknown flag:"                    # Open64
     FAIL_REGEX "Incorrect command line option:"            # Borland
     FAIL_REGEX "Warning: illegal option"                   # SunStudio 12
     FAIL_REGEX "[Ww]arning: Invalid suboption"             # Fujitsu
   )
endmacro ()
