#.rst:
# CheckFortranCompilerFlag
# ------------------------
#
# Check whether the Fortran compiler supports a given flag.
#
# CHECK_Fortran_COMPILER_FLAG(<flag> <var>)
#
# ::
#
#   <flag> - the compiler flag
#   <var>  - variable to store the result
#            Will be created as an internal cache variable.
#
# This internally calls the check_fortran_source_compiles macro and
# sets CMAKE_REQUIRED_DEFINITIONS to <flag>.  See help for
# CheckFortranSourceCompiles for a listing of variables that can
# otherwise modify the build.  The result only tells that the compiler
# does not give an error message when it encounters the flag.  If the
# flag has any effect or even a specific one is beyond the scope of
# this module.

#=============================================================================
# CMake - Cross Platform Makefile Generator
#
# Copyright 2000-2015 Kitware, Inc.
# Copyright 2000-2011 Insight Software Consortium
#
# Copyright 2015 Nicolas Bock <nicolasbock@gmail.com>
# Copyright 2006-2011 Kitware, Inc.
# Copyright 2006 Alexander Neundorf <neundorf@kde.org>
# Copyright 2011 Matthias Kretz <kretz@kde.org>
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

# Try to include a recent version provided by the CMake installation. If no
# Module is available, use this file as fallback.
include("${CMAKE_ROOT}/Modules/CheckFortranCompilerFlag.cmake" OPTIONAL
  RESULT_VARIABLE INCLUDED)
if (INCLUDED)
  return()
else ()
  message(STATUS "Using legacy CheckFortranCompilerFlag.cmake")
endif ()


include(CheckFortranSourceCompiles)
include(CMakeCheckCompilerFlagCommonPatterns)

macro (CHECK_Fortran_COMPILER_FLAG _FLAG _RESULT)
  set(SAFE_CMAKE_REQUIRED_DEFINITIONS "${CMAKE_REQUIRED_DEFINITIONS}")
  set(CMAKE_REQUIRED_DEFINITIONS "${_FLAG}")

  # Normalize locale during test compilation.
  set(_CheckFortranCompilerFlag_LOCALE_VARS LC_ALL LC_MESSAGES LANG)
  foreach(v ${_CheckFortranCompilerFlag_LOCALE_VARS})
    set(_CheckFortranCompilerFlag_SAVED_${v} "$ENV{${v}}")
    set(ENV{${v}} C)
  endforeach()
  CHECK_COMPILER_FLAG_COMMON_PATTERNS(_CheckFortranCompilerFlag_COMMON_PATTERNS)
  CHECK_Fortran_SOURCE_COMPILES("       program test\n       stop\n       end program" ${_RESULT}
    # Some compilers do not fail with a bad flag
    FAIL_REGEX "command line option .* is valid for .* but not for Fortran" # GNU
    ${_CheckFortranCompilerFlag_COMMON_PATTERNS}
    )
  foreach(v ${_CheckFortranCompilerFlag_LOCALE_VARS})
    set(ENV{${v}} ${_CheckFortranCompilerFlag_SAVED_${v}})
    unset(_CheckFortranCompilerFlag_SAVED_${v})
  endforeach()
  unset(_CheckFortranCompilerFlag_LOCALE_VARS)
  unset(_CheckFortranCompilerFlag_COMMON_PATTERNS)

  set (CMAKE_REQUIRED_DEFINITIONS "${SAFE_CMAKE_REQUIRED_DEFINITIONS}")
endmacro ()
