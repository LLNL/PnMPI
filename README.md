PnMPI Tool Infrastructure
========================================

[![Travis](https://img.shields.io/travis/LLNL/PnMPI/master.svg?style=flat-square)](https://travis-ci.org/LLNL/PnMPI)
[![Codecov](https://img.shields.io/codecov/c/github/LLNL/PnMPI.svg?style=flat-square)](https://codecov.io/github/LLNL/PnMPI?branch=master)
[![](https://img.shields.io/github/issues-raw/LLNL/PnMPI.svg?style=flat-square)](https://github.com/LLNL/PnMPI/issues)
[![](https://img.shields.io/badge/license-LGPL-blue.svg?style=flat-square)](LICENSE)

by Martin Schulz, schulzm@llnl.gov, LLNL-CODE-402774
---------

PnMPI is a dynamic MPI tool infrastructure that builds on top of the
standardized PMPI interface. It allows the user to

  * run multiple PMPI tools concurrently
  * activate PMPI tools without relinking by just changing a configuration file
  * multiplex toolsets during a single run
  * write cooperative PMPI tools

The package contains two main components:

  * The PnMPI core infrastructure
  * Tool modules that can explicitly exploit PnMPI's capabilities

So far, this software has mainly been tested on Linux clusters with RHEL-based
OS distributions as well as IBM's BG/P systems. Continuous integration tests run
at Ubuntu 12.04 and 14.04, OSX El Capitan and macOS Sierra. Some preliminary
experiments have also included SGI Altix systems. Ports to other platforms
should be straightforward, but this is not extensively tested. Please file an
[issue](https://github.com/LLNL/PnMPI/issues) if you run into problems porting
PnMPI or if you successfully deployed PnMPI on a new platform.

Many thanks to our
[contributors](https://github.com/LLNL/PnMPI/graphs/contributors).


A) Building PnMPI
===============================================================
PnMPI uses CMake for its build system.


A1) Dependencies
----------------

  * [CMake](http://www.cmake.org) (at least version 2.8.11.2 is required).
  * [argp](https://www.gnu.org/software/libc/manual/html_node/Argp.html) is
    usually included in glibc. However at macOS you have to install it from
    homebrew as `argp-standalone` package.
  * [binutils](https://www.gnu.org/software/binutils/) and libiberty for the
    patcher utility *(optional)*
  * [Doxygen](www.doxygen.org) for generating the docs and man-pages.
    *(optional)*
  * [help2man](https://www.gnu.org/software/help2man/) for generating man-pages.
    *(optional)*
  * An MPI implementation with header files. So far, PnMPI has been tested with
    MPICH, OpenMPI and IntelMPI.

In addition, PnMPI uses git submodules for several CMake modules,
[wrap](https://github.com/LLNL/wrap) and
[adept-utils](https://github.com/LLNL/adept-utils). While the deploy source
tar-ball includes all required submodules, git users need to checkout them with
the following command in the root of the cloned repository:

    git submodule update --init --recursive


A2) Configure the project
-------------------------
In the simplest case, you can run this in the top-level directory of the PnMPI
tree:

    cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/destination
    make
    make install

This will configure, build, and install PnMPI to the destination specified.
PnMPI supports parallel make with the `-j` parameter. E.g., for using eight
build tasks, use:

    cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/destination
    make -j8
    make install

On more complex machines, such as those with filesystems shared among multiple
platforms, you will want to separate out your build directories for each
platform. CMake makes this easy.

Create a new build directory named according to the platform you are using, `cd`
into it, an run `cmake` there.  For example:

    cd <pnmpi>
    mkdir x86_64
    cd x86_64
    cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/destination ..

Here, `<pnmpi>` is the top-level directory in the PnMPI tree. Note that when you
run CMake this way, you need to supply the path to the PnMPI *source* directory
as the last parameter. Here, that's just `..` as we are building in a
subdirectory of the source directory. Once you run CMake, simply run make and
make install as before:

    make -j8
    make install

The PnMPI build should auto-detect your MPI installation and determine library
and header locations. If you want to build with a particular MPI that is NOT the
one auto-detected by the build, you can supply your particular MPI compiler as a
parameter:

    cmake \
      -DCMAKE_INSTALL_PREFIX=/path/to/install/destination \
      -DMPI_C_COMPILER=/path/to/my/mpicc \
      ..

See the documentation in [FindMPI.cmake](cmakemodules/legacy/FindMPI.cmake) for
more details on MPI build configuration options.

If you have problems, you may want to build PnMPI in debug mode. You can do this
by supplying an additional parameter to `cmake`, e.g.:

    cmake \
      -DCMAKE_INSTALL_PREFIX=/path/to/install/destination \
      -DCMAKE_BUILD_TYPE=Debug \
      ..

The `extra/build` directory contains a few sample invocations of CMake that have
been successfully used on LLNL systems.


A3) Configuring with/without Fortran
------------------------------------
By default PnMPI is configured to work with C/C++ and Fortran codes. However, on
systems where Fortran is not available, the system should auto-detect this and
not build the Fortran libraries and test cases. It can also be manually turned
off by adding

    -DENABLE_FORTRAN=OFF

to the `cmake` configuration command.

The PnMPI distribution contains test cases for C and Fortran that allow you to
test the correct linkage.


A3a) Optional configuration options
-----------------------------------
If you want to change the default build configuration, you can enable / disable
features by adding the following flags to the `cmake` configuration command.

  * `-DBUILD_DOC=ON`: Generate the public code documentation and man-pages.
    PnMPI developers may enable `BUILD_DOC_INTERNAL` instead for additional
    PnMPI internal documentation. *(requires Doxygen and help2man)*
  * `-DENABLE_DEBUG=OFF`: Disable PnMPI on-demand debug logging. *Disable this
    option only for performance optimization.*
  * `-DENABLE_MODULES=OFF`: Don't build the built-in modules.
  * `-DENABLE_TESTING=ON`: Build the test cases and enable the `test` target.
  * `-DENABLE_THREAD_SAFETY=OFF`: Build PnMPI without thread safety. PnMPI will
    limit the MPI threading level if this option is enabled, so it still will be
    thread safe. *Disable this option only for performance optimization.*
  * `-DENABLE_PNMPIZE=OFF`: Don't build the PnMPI invocation tool and don't run
    any tests for it. This option may be required when testing PnMPI at a
    platform that doesn't support the `execvp()` syscall.
  * `-DENABLE_ADEPT_UTILS=ON`: Enable support for adept-utils, to check the
    module's symbols for their origin. *(Not supported with all compilers.)*
  * [CMake-codecov](https://github.com/RWTH-HPC/CMake-codecov) provides the
    additional `-DENABLE_COVERAGE=ON` option to enable code coverage. You can
    generate coverage reports with the `gcov` and `lcov` targets. Read their
    [README](https://github.com/RWTH-HPC/CMake-codecov/blob/master/README.md)
    for more information.
  * [cmake-sanitizers](https://github.com/arsenm/sanitizers-cmake) provides
    additional options to enable sanitizers for all PnMPI targets. E.g. for
    AddressSanitizer add `-DSANITIZE_ADDRESS=ON`. Read their
    [README](https://github.com/arsenm/sanitizers-cmake/blob/master/README.md)
    for more information.


A4) Configuring for cross-compiled environments
-----------------------------------------------
When configuring PnMPI in cross-compiled environments (such as Blue Gene/Q
systems), it is necessary to provide a matching tool chain file. Many toolchain
files are included in CMake, additional example files that allow the compilation
on certain LC machines can be found in `cmakemodules/Platform` and
`cmakemodules/Toolchain`.

For example, to configure PnMPI for a BG/Q machine using the GNU compiler
suite, add the following to the `cmake` configuration command:

    -DCMAKE_TOOLCHAIN_FILE=../cmakemodules/Toolchain/BlueGeneQ-gnu.cmake

*You may need to modify the toolchain file for your system.*


A5) Installed structure
-----------------------
Once you've installed, all your PnMPI files and executables should be in
`<CMAKE_INSTALL_PREFIX>`, the path specified during configuration. Roughly, the
install tree looks like this:

    bin/
      pnmpi                PnMPI invocation tool
      pnmpi-patch          Library patching utility
    lib/
      libpnmpi[f].[so,a]   PnMPI runtime libraries
      pnmpi-modules/       System-installed tool modules
      cmake/               Build files for external modules
    include/
      pnmpi/               PnMPI header directory
        debug_io.h         PnMPI module debug print functions.
        hooks.h            PnMPI module hook definitions.
        service.h          PnMPI module service functions.
        ...
      pnmpi.h              PnMPI main header
      pnmpimod.h           PnMPI module support (legacy)
      pnmpi-config.h       CMake generated configuration file
    share/
      cmake/               CMake files to support tool module builds

Test programs are not installed, but in the [tests/src](tests/src) folder of the
build directory, there should also be test programs built with PnMPI. See below
for details on running these to test your PnMPI installation.


A6) Environment setup
-----------------------
You will need to set one environment variable to run PnMPI:

  * `PNMPI_LIB_PATH` should be set to the full path to the PnMPI modules
    directory. If not set, the default path of your installation directory will
    be used.
  * `PNMPI_CONF` should be set to the full to the path of the PnMPI
    configuration file to be used.
  * `PNMPI_BE_SILENT` will silence the PnMPI banner. For benchmark purposes you
    should also disable `ENABLE_DEBUG` in your CMake configuration. **Note:**
    Warnings on errors still will be printed.


A6a) Using the PnMPI invocation tool
------------------------------------
To run PnMPI in front of any application (that is dynamically linked to MPI),
you may use the `bin/pnmpi` tool. It will setup the environment and preloads
PnMPI for you. For a list of supported arguments, invoke it with the `--help`
flag.

    :~$ pnmpi --help
    Usage: pnmpi [OPTION...] utility [utility options]
    P^nMPI -- Virtualization Layer for the MPI Profiling Interface

      -c, --config=FILE          Configuration file
      -q, -s, --quiet, --silent  Don't produce any output
      -?, --help                 Give this help list
          --usage                Give a short usage message
      -V, --version              Print program version
    ...


    :~$ mpiexec -np 2 pnmpi -c pnmpi.conf a.out
      _____   _ __   __  __  _____   _____
     |  __ \ | '_ \ |  \/  ||  __ \ |_   _|
     | |__) || | | || \  / || |__) |  | |
     |  ___/ |_| |_|| |\/| ||  ___/   | |
     | |            | |  | || |      _| |_
     |_|            |_|  |_||_|     |_____|


     Application:
      MPI interface: C

     Global settings:
      Pcontrol: 5

     Loaded modules:
      Stack default:
        sample1 (Pcontrol: 1)
      Stack foo:
    ...

**Note:** The PnMPI invocation tool is not compatible with all platforms (e.g.
BlueGene/Q), as it requires the `execvp()` function, which might not be
supported.


A7) RPATH settings
-----------------------
By default, the build adds the paths of all dependency libraries to the rpath of
the installed PnMPI library. This is the preferred behavior on LLNL systems,
where many packages are installed and `LD_LIBRARY_PATH` usage can become
confusing.

If you are installing on a system where you do NOT want dependent libraries
added to your RPATH, e.g. if you expect all of PnMPI's dependencies to be found
in system paths, then you can build without rpath additions using this option to
`cmake`:

    -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=FALSE

This will add only the PnMPI installation lib directory to the rpath of the
PnMPI library.


B) Usage
========
PnMPI supports multiple ways to use it with an MPI application:

B1) Dynamic linking
-------------------
If your application is dynamically linked against the MPI library, you may use
PnMPI by simply preloading it via `LD_PRELOAD` (or `DYLD_INSERT_LIBRARIES` at
macOS).

Instead of manually preloading, you may use the PnMPI invocation tool in
`bin/pnmpi`.

B2) Static linking
------------------
Instead of linking your application against the MPI library, you may link
against the static MPI library.

**Note:** By default the linker will only link functions used by your code, so
most of the API functions would not get linked into your binary. PnMPI
implements a helper function to force the linker to link all required functions
into the binary. However there might be complications, if not all functions
wrapped by the modules are used by the application. You should tell the linker
to link the whole PnMPI archive explicitly:

    mpicc main.o -Wl,--whole-archive pnmpi.a -Wl,--no-whole-archive -o test

*Note: The linker option `--whole-archive` is not available at mac OS.*


C) Modules
==========
PnMPI supports two different kind of tool modules:

  * Transparent modules
  * PnMPI-specific modules

Among the former are modules that have been created independently of PnMPI and
are just based on the PMPI interface. To use a transparent module in PnMPI the
user has to perform two steps:

  1. Build the tool as a shared module (a dlopen-able shared library).
  2. Patch the tool using the `pnmpi-patch` utility, which is included
     with the PnMPI distribution.

Usage:

    pnmpi-patch <original tool (in)> <patched tool (out)>

e.g.:

    pnmpi-patch my-module.so my-pnmpi-module.so

After that, copy the tool in one of the directories listed in `$PNMPI_LIB_PATH`
so that PnMPI can pick it up. *Note that all of this is handled automatically
by the CMake build files included with PnMPI (see below for more information).*

The second option is the use of PnMPI specific modules: these modules also rely
on the PMPI interface, but explicitly use some of the PnMPI features (i.e., they
won't run outside of PnMPI). These modules include the PnMPI header files, which
describe the interface that modules can use. In short, the interface offers an
ability to register a module and after that use a publish/subscribe interface to
offer/use services in other modules.

**Note:** also PnMPI specific modules have to be patched using the utility
described above, if they don't use the XMPI interface instead of the PMPI.


C1) Built-in modules
--------------------
This package includes a set of modules that can be used both to create other
tools using their services and as templates for new modules.

The source for all modules is stored in separate directories inside the
`src/modules/` directory. There are:

* **sample:**
  A set of example modules that show how to wrap send and receive operations.

* **empty:**
  A transparent module that simply wraps all calls without executing any code.
  This can be used to test overhead or as a sample for new modules.

* **status:**
  This PnMPI specific module offers a service to add extra data to each status
  object to store additional information.

* **requests:**
  This PnMPI specific module offers a service to track asynchronous
  communication requests. It relies on the status module.

* **datatype:**
  This PnMPI specific module tracks all datatype creations and provides an
  iterator to walk any datatype.

* **comm:**
  This PnMPI specific module abstracts all communication in a few simple
  callback routines. It can be used to write quick prototype tools that
  intercept all communication operations independent of the originating MPI
  call. This infrastructure can be used by creating submodules: two such
  submodules are included: an empty one that can be used as a template and one
  that prints every message.
  Note: this module relies on the status, requests, and datatype modules. A more
  detailed description on how to implemented submodules is included in the comm
  directory as a separate README.

* **limit-threading**
  This PnMPI specific module limits the MPI threading level to the value set in
  the `PNMPI_THREADING_LEVEL` environment variable. It may be used to check the
  behavior of an application, if the MPI environment doesn't support a specific
  threading level.

* **metrics-counter**
  This PnMPI specific module counts the MPI call invocations. Add the module at
  the top of your config file to count how often each rank invoked which MPI
  call or in front of a specific module to count how often invocations reached
  this module.

* **metrics-timing**
  This PnMPI specific module measures the time MPI call invocations take. It has
  two different operation modes:

  * simple: Add it in front of a module and it will measure the time of all
    following modules.

        module metrics-timing
        module sample1

  * advanced: Add the timing module before and after the modules you want to
    measure and it will only measure the time of these, but not the following
    modules.

        module metrics-timing
        module sample1
        module sample2
        module metrics-timing

        module empty

    Measuring `MPI_Pcontrol` is available in advanced mode, only. To measure
    `MPI_Pcontrol` calls both `metrics-timing` invocations need to be pcontrol
    enabled:

        module metrics-timing
        pcontrol on # May be ignored if metrics-timing is the first module.
        module sample1
        module metrics-timing
        pcontrol on

* **wait-for-debugger**
  This module prints the PID of each rank before executing the application. If
  the `WAIT_AT_STARTUP` environment variable is set to a numeric value, the
  execution will be delayed up to `value` seconds, so you may attach with a
  debugger in that time.

**Note:** All modules must be compiled with the same MPI as PnMPI is build
with. Modules should only be linked to their required libraries, except PnMPI
and MPI, as these routines will be provided by PnMPI when the module will be
loaded.


D) Building your own modules with CMake
=======================================
PnMPI installs CMake build files with its distribution to allow external
projects to quickly build MPI tool modules. The build files allow external
tools to use PnMPI, the `pnmpi-patch` utility, PnMPI's wrapper generator, and
PnMPI's dependency libraries.

To create a new PnMPI module, simply create a new project that looks something
like this:

    my-project/
      CMakeLists.txt
      foo.c
      wrapper.w

Assume that `wrapper.w` is a wrapper generator input file that will generate
another file called `wrapper.c`, which contains MPI interceptor functions for
the tool library. `foo.c` is additional code needed for the tool library.
`CMakeLists.txt` is the CMake build file.

Your `CMakeLists.txt` file should start with something like this:

```CMake
project(my-module C)
cmake_minimum_required(VERSION 2.8.11.2)

find_package(PnMPI REQUIRED)
find_package(MPI REQUIRED)

add_wrapped_file(wrapper.c wrapper.w)
pnmpi_add_pmpi_module(foo foo.c wrapper.c)

install(TARGETS foo DESTINATION ${PnMPI_MODULES_DIR})

include_directories(
  ${MPI_INCLUDE_PATH}
  ${PnMPI_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})
```

* `project()` and `cmake_minimum_required()` are standard.  These tell the build
  the name of the project and what version of CMake it will need.

* `find_package(PnMPI)` will try to find PnMPI in system locations, and if
  it is not found, it will fail because the `REQUIRED` option has been
  specified.

* `find_package(MPI)` locates the system MPI installation so that you can use
  its variables in the `CMakeLists.txt` file.

* `add_wrapped_file()` tells the build that `wrapper.c` is generated from
  `wrapper.w`, and it adds rules and dependencies to the build to automatically
  run the wrapper generator when `wrapper.c` is needed.

* Finally, `pnmpi_add_pmpi_module()` function's much like CMake's builtin
  `add_library()` command, but it ensures that the PnMPI library is built as a
  loadable module and that the library is properly patched.

  If your module uses the XMPI interface (i.e. doesn't need to be patched), use
  `pnmpi_add_xmpi_module()` instead, so it will build the module but don't patch
  it.

* `install()` functions just as it does in a standard CMake build, but here we
  install to the `PnMPI_MODULES_DIR` instead of an install-relative location.
  This variable is set if PnMPI is found by `find_package(PnMPI)`, and it allows
  users to install directly into the system PnMPI installation.

* Finally, `include_directories()` functions as it would in a normal CMake
  build.

Once you've make your `CMakeLists.txt` file like this, you can build your PnMPI
module like so:

    cd my-module
    mkdir $SYS_TYPE && cd $SYS_TYPE
    cmake ..
    make -j8
    make -j8 install

This should find PnMPI on your system and build your module, assuming that you
have your environment set up correctly.


D2) Limiting the threading level
--------------------------------
If your module is not thread safe or is only able to process a limited amount of
threading, it should limit the required threading level in the `MPI_Init_thread`
wrapper:

```C
int MPI_Init_thread(int *argc, char ***argv, int required, int *provided)
{
  if (required > MPI_THREAD_SINGLE)
    required = MPI_THREAD_SINGLE;

  return XMPI_Init_thread(argc, argv, required, provided);
}
```


D3) Module hooks
----------------
At different points hooks will be called in all loaded modules. These can be
used to trigger some functionality at a given time. All hooks have the return
type `void` and are defined in `pnmpi/hooks.h`, which should be included for
type safety. These following hooks will be called in all modules:

* `PNMPI_RegistrationPoint`: This hook will be called just after the module has
  been loaded. It may be used to register the name of the module, services
  provided by the module, etc.
* `PNMPI_UnregistrationPoint`: This hook will be called just before the module
  will be unloaded. It may be used e.g. to free allocated memory.
* `PNMPI_Init`: This hook will be called just after all modules have been
  registered to initialize the module. It may be used to initialize the module's
  to code and is communicate with other modules.
* `PNMPI_Fini`: This hook will be called just before all modules will be
  unregistered. The modules may communicate with each other and should execute
  some final steps in here.

*Note: You can use `PNMPI_Service_CallHook()` to call custom hooks in your
modules. Just pass a custom hook name as first parameter.*

For a detailed description, see the Doxygen docs or man-pages for these
functions.


D4) Module service functions
----------------------------
Modules may interact with the PnMPI core and other modules with the module
service functions defined in `pnmpi/service.h`. For a detailed description about
these functions, see the Doxygen docs for the service header or the man-pages.


D5) Debug message functions
---------------------------
Modules may print debug messages, warnings and errors with the PnMPI API
functions `PNMPI_Debug`, `PNMPI_Warning` and `PNMPI_Warning` defined in
`pnmpi/debug_io.h`. *PnMPI will add additional informations like rank or line
numbers to the printed messages.*

For a detailed description, see the Doxygen docs or man-pages for these
functions.


E) Debug Options
================
If PnMPI is build with `ENABLE_DEBUG`, PnMPI includes debug print functions,
that can be dynamically enabled. To control it, the environment variable
`PNMPI_DBGLEVEL` can be set to any combination of the following debug levels:

* `0x01` - Messages about PnMPI initialization (before MPI is initialized).
* `0x02` - Messages about module loading.
* `0x04` - Messages about MPI call entry and exit.

**NOTE:** The first two levels should be enabled for single-rank executions
only, as their output can't be limited to a single rank and thus will be printed
on **all** ranks.

Additionally, the printouts can be restricted to a single node by setting the
variable `PNMPI_DBGNODE` to an MPI rank.


E1) Using the PnMPI invocation tool
-----------------------------------
You may set the above options in the PnMPI invocation tool. Use the `--debug`
option to enable a specific debug level and `--debug-node` to limit the debug
output to a single rank.


F) Configuration and Demo codes
===============================
The PnMPI distribution includes test cases (in C and Fortran). They can be used
to experiment with the basic PnMPI functionalities and to test the system setup.
The following describes the C version (the F77 version works similarly):

1. Change into the [tests/src](tests/src) directory.
2. The program [test-mpi.c](tests/src/test-mpi.c), which only initializes and
   then finalizes MPI, was compiled into three binaries:

    * `testbin-binary_mpi_c-preload` (plain MPI code)
    * `testbin-binary_mpi_c-dynamic` (linked dynamically against PnMPI)
    * `testbin-binary_mpi_c-static` (linked statically against PnMPI)

3. Executing the `*-preload` binary will not print any output, but the binaries
   linked against PnMPI will print the PnMPI header, indicating PnMPI is loaded
   before MPI.

4. PnMPI is configured through a configuration file that lists all modules to be
   load by PnMPI as well as optional arguments. The name for this file can be
   specified by the environment variable `PNMPI_CONF`. If this variable is not
   set or the file specified can not be found, PnMPI looks for a file called
   `.pnmpi_conf` in the current working directory, and if not found, in the
   user's home directory.

   A simple configuration file may look as follows:

       module sample1
       module sample2
       module sample3
       module sample4

  (plus some additional lines starting with `#`, which indicates comments)

  This configuration causes these four modules to be loaded in the specified
  order. PnMPI will look for the corresponding modules (.so shared library
  files) in `PNMPI_LIB_PATH`.

5. Running the `testbin-binary_mpi_sendrecv` (a simple test sending messages
   between the ranks) will load all four modules in the specified order and
   intercept all MPI calls included in these modules:

    * sample1: send and receive
    * sample2: send
    * sample3: receive
    * sample4: send and receive

  The program output (for 2 nodes) will be:

  ```
    _____   _ __   __  __  _____   _____
   |  __ \ | '_ \ |  \/  ||  __ \ |_   _|
   | |__) || | | || \  / || |__) |  | |
   |  ___/ |_| |_|| |\/| ||  ___/   | |
   | |            | |  | || |      _| |_
   |_|            |_|  |_||_|     |_____|


   Application:
    MPI interface: C

   Global settings:
    Pcontrol: 5

   Loaded modules:
    Stack default:
      sample1 (Pcontrol: 1)
      sample2
      sample3
      sample4

  WRAPPER 1: Before recv
  WRAPPER 1: Before send
  WRAPPER 2: Before send
  WRAPPER 4: Before send
  WRAPPER 4: After send
  WRAPPER 2: After send
  WRAPPER 1: After send
  WRAPPER 1: Before recv
  WRAPPER 3: Before recv
  WRAPPER 4: Before recv
  WRAPPER 3: Before recv
  WRAPPER 4: Before recv
  WRAPPER 4: After recv
  WRAPPER 3: After recv
  WRAPPER 1: After recv
  Got 1 from rank 1.
  WRAPPER 1: Before send
  WRAPPER 2: Before send
  WRAPPER 4: Before send
  WRAPPER 4: After send
  WRAPPER 4: After recv
  WRAPPER 3: After recv
  WRAPPER 1: After recv
  Got 1 from rank 0.
  WRAPPER 2: After send
  WRAPPER 1: After send
  ```

  When running on a BG/P systems, it is necessary to explicitly export some
  environment variables. Here is an example:

        mpirun -np 4 -exp_env LD_LIBRARY_PATH -exp_env PNMPI_LIB_PATH \
          -cwd $PWD testbin-binary_mpi_sendrecv


G) Using `MPI_Pcontrol`
========================
The MPI standard defines the `MPI_Pcontrol`, which does not have any direct
effect (it is implemented as a dummy call inside of MPI), but that can be
replaced by PMPI to accepts additional information from MPI applications (e.g.,
turn on/off data collection or markers for main iterations). The information is
used by a PMPI tool linked to the application. When it is used with PnMPI the
user must therefore decide which tool an `MPI_Pcontrol` call is directed to.

By default PnMPI will direct `MPI_Pcontrol` calls to first module in the tool
stack only. If this is not the desired effect, users can turn on and off which
module Pcontrols reach by adding `pcontrol on` and `pcontrol off` into the
configuration file in a separate line following the corresponding module
specification. Note that PnMPI allows that `MPI_Pcontrol` calls are sent to
multiple modules.

In addition, the general behavior of Pcontrols can be specified with a global
option at the beginning of the configuration file. This option is called
`globalpcontrol` and can take one of the following arguments:

* `int`: Only deliver the first argument, but ignore the variable list arguments
  (default)
* `pmpi`: Forward the variable list arguments
* `pnmpi`: Requires the application to specify a specific format in the variable
  argument list known to PnMPI (level must be `PNMPI_PCONTROL_LEVEL`)
* `mixed`
  * Same as pnmpi, if `level == PNMPI_PCONTROL_LEVEL`.
  * Same as pmpi, if  `level != PNMPI_PCONTROL_LEVEL`.
* `typed <tlevel> <type>`: Forward the first argument in any case, the second
  only if the level matches `<tlevel>`. In this case, assume that the second
  argument is of type `<type>`.

The PnMPI internal format for Pcontrol arguments is as follows:

```
int level (same semantics as for MPI_Pcontrol itself)
int type = PNMPI_PCONTROL_SINGLE or PNMPI_PCONTROL_MULTIPLE
               (target one or more modules) |
               PNMPI_PCONTROL_VARG or PNMPI_PCONTROL_PTR
               (arguments as vargs or one pointer)
int mod = target module (if SINGLE)
int modnum = number of modules (if MULTIPLE)
int *mods = pointer to array of modules
int size = length of all variable arguments (if VARG)
void *buf = pointer to argument block (if PTR)
```


#### Known issues:

Forwarding the variable argument list as done in `pmpi` and `mixed` is only
implemented in a highly experimental version and disabled by default. To enable,
compile PnMPI with the flag `EXPERIMENTAL_UNWIND` and link PnMPI with the
libunwind library. Note that this is not extensively tested and not portable
across platforms.


H) References
=============
More documentation on PnMPI can be found in the following two published articles:

* Martin Schulz and Bronis R. de Supinski.
  **PnMPI Tools: A Whole Lot Greater Than the Sum of Their Parts**.
  *Supercomputing 2007*.
  November 2007, Reno, NV, USA.

  Available [here](http://sc07.supercomputing.org/schedule/pdf/pap224.pdf).

* Martin Schulz and Bronis R. de Supinski.
  **A Flexible and Dynamic Infrastructure for MPI Tool Interoperability**.
  *International Conference on Parallel Processing (ICPP)*.
  August 2005, Columbus, OH, USA.
  Published by IEEE Press

  Available [here](http://ieeexplore.ieee.org/iel5/11126/35641/01690620.pdf?isnumber=35641&prod=CNF&arnumber=1690620&arSt=193&ared=202&arAuthor=Martin+Schulz%3B+Bronis+R.+de+Supinski).


I) Contact
==========
For more information or in case of questions, please contact
[Martin Schulz](schulzm@llnl.gov) or
[file an issue](https://github.com/LLNL/PnMPI/issues/new).


Copyright
===========
Copyright &copy; 2008-2019 Lawrence Livermore National Security, LLC.<br/>
Copyright &copy; 2011-2016 ZIH, Technische Universitaet Dresden, Federal
Republic of Germany<br/>
Copyright &copy; 2013-2019 RWTH Aachen University, Federal Republic of Germany

All rights reserved - please read the information in the [LICENSE](LICENSE)
file.
