/*
Copyright (c) 2008
Lawrence Livermore National Security, LLC.

Produced at the Lawrence Livermore National Laboratory.
Written by Martin Schulz, schulzm@llnl.gov.
LLNL-CODE-402774,
All rights reserved.

This file is part of P^nMPI.

Please also read the file "LICENSE" included in this package for
Our Notice and GNU Lesser General Public License.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
(as published by the Free Software Foundation) version 2.1
dated February 1999.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the IMPLIED WARRANTY
OF MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
terms and conditions of the GNU General Public License for more
details.

You should have received a copy of the GNU Lesser General Public
License along with this program; if not, write to the

Free Software Foundation, Inc.,
59 Temple Place, Suite 330,
Boston, MA 02111-1307 USA
*/

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "core.h"
#include "pnmpi-config.h"

pnmpi_cell_t pnmpi_activated[NUM_MPI_CELLS];
pnmpi_functions_t pnmpi_function_ptrs;

int pnmpi_level;
int pnmpi_max_level;

modules_t modules;

static void *mydlopen(char *s, char *path, int f)
{
  // reset error message
  void *ret = NULL;
  char *pathdup, *start, *end;
  module_name_t mod;

  dlerror();

  pathdup = strdup(path);
  start = pathdup;
  do
    {
      end = strchr(start, ':');
      if (end != NULL)
        {
          *end = (char)0;
          end++;
        }

      sprintf(mod, "%s/%s", start, s);

      if (access(mod, R_OK) != -1)
        {
          ret = dlopen(mod, f);
          break;
        }
      else if (access(mod, F_OK) != -1)
        {
          WARNPRINT("Can't load module %s at %s, no reading permissions", s,
                    mod);
        }

      start = end;
    }
  while ((end != NULL) && (ret == NULL));

  if (ret != NULL)
    {
      DBGPRINT2("Loading module %s\n", mod);
    }
  free(pathdup);
  return ret;
}

static void *mydlsym(void *h, char *s)
{
  void *ret;
  ret = dlsym(h, s);
  return ret;
}


static int whitespace(char c)
{
  if ((c == '\t') || (c == ' ') || (c == '\n'))
    return 1;
#ifdef AIX
  if ((int)c == 255)
    return 1;
#endif
  return 0;
}


/* Core functionality for PNMPI */

void pnmpi_PreInit()
{
  char *libdir, *confdir;
  module_name_t filename, modname;
  FILE *conffile = NULL;
  char line[MAX_LINE], c, lastc;
  int pos, err;
  char *cmdargv[MAX_CMDARGS + 1];
  int cmdargc, comment, i;

  PNMPI_RegistrationPoint_t regPoint;

  /* setup vars */

  pnmpi_level = 0;
  pnmpi_max_level = 0;


  /* set global defaults */
  /* none at this moment */

  modules.module = NULL;
  modules.num = 0;
  modules.numalloc = 0;
  modules.pcontrol = PNMPI_PCONTROL_INT;

  /* locate library */

  libdir = getenv("PNMPI_LIB_PATH");
  if (libdir == NULL)
    {
      // no user libdir; just use the install destination's module path.
      libdir = PNMPI_MODULES_DIR;
    }
  else
    {
      // concat the user libdir with the install destination's module path.
      size_t len = strlen(libdir) + strlen(PNMPI_MODULES_DIR) + 2;
      const char *old_libdir = libdir;
      libdir = (char *)malloc(len * sizeof(char));
      sprintf(libdir, "%s:%s", old_libdir, PNMPI_MODULES_DIR);
    }

  DBGPRINT2("Found library at %s", libdir);

  /* locate and open file */

  /* check the environment variable first */
  confdir = getenv("PNMPI_CONF");
  if (confdir != NULL)
    {
      /* try to open it */
      conffile = fopen(confdir, "r");
      if (conffile == NULL)
        {
          int error = errno;
          if (error == ENOENT)
            {
              WARNPRINT(
                "Configuration file not found at %s - reverting to defaults.",
                confdir);
            }
          else
            {
              WARNPRINT("Can't open configuration file %s (Error %i) - not "
                        "loading any PNMPI modules.",
                        confdir, error);
              return;
            }
        }
      else
        {
          DBGPRINT2("Open file via environment variable - %s", confdir);
        }
    }

  if (conffile == NULL)
    {
      /* now check the local directory */

      size_t confsize = 1024;
      confdir = malloc(confsize * sizeof(char));
      while (NULL == getcwd(confdir, confsize))
        {
          if (errno != ERANGE)
            {
              free(confdir);
              confdir = NULL;
              break;
            }
          confsize *= 2;
          confdir = realloc(confdir, confsize);
        }

      if (confdir == NULL)
        {
          WARNPRINT(
            "Can't find local directory - not loading any PNMPI modules.");
          return;
        }

      sprintf(filename, "%s/%s", confdir, CONFNAME);
      conffile = fopen(filename, "r");
      if (conffile == NULL)
        {
          int error = errno;
          if (error != ENOENT)
            {
              WARNPRINT("Can't open configuration file %s (Error %i) - not "
                        "loading any PNMPI modules.",
                        filename, error);
              return;
            }
        }
      else
        {
          DBGPRINT2("Open file via local directory - %s", filename);
        }

      if (confdir)
        free(confdir);
    }


  if (conffile == NULL)
    {
      /* now check the home directory */

      confdir = getenv("HOME");
      if (confdir == NULL)
        {
          WARNPRINT(
            "Can't find local directory - not loading any PNMPI modules.");
          return;
        }

      sprintf(filename, "%s/%s", confdir, CONFNAME);
      conffile = fopen(filename, "r");
      if (conffile == NULL)
        {
          int error = errno;
          if (error != ENOENT)
            {
              WARNPRINT("Can't open configuration file %s (Error %i) - not "
                        "loading any PNMPI modules.",
                        filename, error);
              return;
            }
          else
            {
              /* WARNPRINT("Can't find any configuration file - not loading any
               * PNMPI modules."); */
              return;
            }
        }
      else
        {
          DBGPRINT2("Open file via home directory - %s", filename);
        }
    }

  DBGASSERT(conffile != NULL, "Config file not open");

  /* read configuration file and load modules */

  DBGPRINT2("Starting to read config file");

  if (conffile != NULL)
    {
      while (!feof(conffile))
        {
          /* read next command */

          pos = 0;
          comment = 0;
          lastc = ' ';
          c = ' ';
          while ((!feof(conffile)) && (c != '\n'))
            {
              c = (char)getc(conffile);
              if ((c == '#') || (c == (char)255))
                comment = 1;
              if (!comment)
                {
                  if (pos == MAX_LINE - 1)
                    {
                      WARNPRINT("Line too long - ignoring the rest");
                      comment = 1;
                    }
                  else
                    {
                      if ((!(whitespace(c))) || (!(whitespace(lastc))))
                        {
                          if (whitespace(c))
                            line[pos] = ' ';
                          else
                            line[pos] = c;
                          lastc = c;
                          pos++;
                        }
                    }
                }
            }

          if (pos > 0)
            {
              if (whitespace(line[pos - 1]))
                pos--;
            }

          line[pos] = (char)0;

          DBGPRINT2("Read a line: %s### - first is %i - pos %i", line,
                    (int)line[0], pos);

          /* decode the line */

          for (i = 0; i < MAX_CMDARGS + 1; i++)
            cmdargv[i] = NULL;

          if (strcmp(line, "") == 0)
            cmdargv[0] = NULL;
          else
            cmdargv[0] = line;

          cmdargc = 0;
          while (cmdargv[cmdargc] != NULL)
            {
              cmdargv[cmdargc + 1] = strchr(cmdargv[cmdargc], ' ');
              if ((cmdargc == 2) && (strcmp(cmdargv[0], "argument") == 0))
                cmdargv[cmdargc + 1] = NULL;
              if (cmdargv[cmdargc + 1])
                {
                  *cmdargv[cmdargc + 1] = (char)0;
                  cmdargv[cmdargc + 1]++;
                }
              if (cmdargc == MAX_CMDARGS - 1)
                {
                  WARNPRINT("Too many arguments - ignoring the rest");
                  cmdargv[cmdargc] = NULL;
                }
              cmdargc++;
            }

#ifdef DBGLEVEL
          DBGPRINT2("CMDARGC = %i", cmdargc);
          for (i = 0; i < cmdargc; i++)
            {
              DBGPRINT2("  %i: %s", i, cmdargv[i]);
            }
#endif

          /* now that we all components, interprete them */

          if (cmdargc == 0)
            {
              /* do nothing */
            }
          else if ((cmdargc == 2) && (strcmp(cmdargv[0], "stack") == 0))
            {
              /* new substack */

              if (modules.num == modules.numalloc)
                {
                  DBGPRINT2("Getting new memory for names");
                  modules.module =
                    realloc(modules.module, (modules.numalloc + MODULE_SKIP) *
                                              sizeof(module_def_p));
                  if (modules.module == NULL)
                    {
                      WARNPRINT("Out memory to load configuration file - not "
                                "loading any PNMPI modules.");
                      modules.num = 0;
                      modules.numalloc = 0;
                      fclose(conffile);
                      return;
                    }
                  modules.numalloc += MODULE_SKIP;
                }

              /* now allocate the actual memory */

              modules.module[modules.num] =
                (module_def_p)malloc(sizeof(module_def_t));
              if (modules.module[modules.num] == NULL)
                {
                  WARNPRINT(
                    "Out memory to load stack defintion - ignoring delimiter.");
                }
              else
                {
                  /* now we have space and can store the information */

                  DBGPRINT2("Found stack %i: %s", modules.num + 1, cmdargv[1]);
                  if (strlen(cmdargv[1]) >= PNMPI_MODULE_FILENAMELEN)
                    {
                      WARNPRINT("Stack name too long - shortening it");
                    }
                  strncpy(modules.module[modules.num]->name, cmdargv[1],
                          PNMPI_MODULE_FILENAMELEN);
                  modules.module[modules.num]
                    ->name[PNMPI_MODULE_FILENAMELEN - 1] = (char)0;

                  /* I don't think we need this - seems copy and paste error
                     sprintf(modname,"%s/%s.so",libdir,modules.module[modules.num]->name);
                     */

                  modules.module[modules.num]->stack_delimiter = 1;
                  modules.module[modules.num]->registered = 0;
                  modules.module[modules.num]->services = NULL;
                  modules.module[modules.num]->username[0] = (char)0;
                  modules.module[modules.num]->args =
                    NULL; /*For "stack" type modules we have to set it here,
                             some debug print at the end uses the pointer*/
                  modules.num++;
                }
            }
          else if ((cmdargc == 2) && (strcmp(cmdargv[0], "module") == 0))
            {
              /* start a new module */

              if (modules.num == modules.numalloc)
                {
                  DBGPRINT2("Getting new memory for names");
                  modules.module =
                    realloc(modules.module, (modules.numalloc + MODULE_SKIP) *
                                              sizeof(module_def_p));
                  if (modules.module == NULL)
                    {
                      WARNPRINT("Out memory to load configuration file - not "
                                "loading any PNMPI modules.");
                      modules.num = 0;
                      modules.numalloc = 0;
                      fclose(conffile);
                      return;
                    }
                  modules.numalloc += MODULE_SKIP;
                }

              /* now allocate the actual memory */

              modules.module[modules.num] =
                (module_def_p)malloc(sizeof(module_def_t));
              if (modules.module[modules.num] == NULL)
                {
                  WARNPRINT(
                    "Out memory to load module defintion - ignoring module.");
                }
              else
                {
                  /* now we have space and can store the information */

                  DBGPRINT2("Found module %i: %s", modules.num + 1, cmdargv[1]);
                  if (strlen(cmdargv[1]) >= PNMPI_MODULE_FILENAMELEN)
                    {
                      WARNPRINT("Module name too long - shortening it");
                    }
                  strncpy(modules.module[modules.num]->name, cmdargv[1],
                          PNMPI_MODULE_FILENAMELEN);
                  modules.module[modules.num]
                    ->name[PNMPI_MODULE_FILENAMELEN - 1] = (char)0;
                  sprintf(modname, "%s.so", modules.module[modules.num]->name);

                  /* The first module gets the pcontrol by default */

                  if (modules.num == 0)
                    modules.module[modules.num]->pcontrol = 1;
                  else
                    modules.module[modules.num]->pcontrol = 0;

                  modules.module[modules.num]->handle =
                    mydlopen(modname, libdir, RTLD_LAZY);
                  if (modules.module[modules.num]->handle == NULL)
                    {
                      const char *dlerr = dlerror();
                      if (dlerr == NULL)
                        {
                          WARNPRINT("Can't load module %s (File not found in "
                                    "PNMPI_LIB_PATH)",
                                    modname);
                        }
                      else
                        {
                          WARNPRINT("Can't load module %s (Error %s)", modname,
                                    dlerr);
                        }
                    }
                  else
                    {
                      /* we could open the module - hence we are good to go */

                      DBGPRINT2("dlopen successful");

                      modules.module[modules.num]->stack_delimiter = 0;
                      modules.module[modules.num]->registered = 0;
                      modules.module[modules.num]->services = NULL;
                      modules.module[modules.num]->globals = NULL;
                      modules.module[modules.num]->args = NULL;
                      modules.module[modules.num]->username[0] = (char)0;

                      /* PNMPI_RegistrationPoint will be called later */

                      pnmpi_level = modules.num;
                      modules.num++;
                    }
                }
            }
          else if (((cmdargc == 1) || (cmdargc == 2)) &&
                   (strcmp(cmdargv[0], "pcontrol") == 0))
            {
              /* check if module is active */

              if (modules.num > 0)
                {
                  int turnon;

                  /* mark the module as receiving pcontrol commands */

                  if (cmdargc == 1)
                    turnon = 1;
                  else
                    {
                      if (strcmp(cmdargv[1], "on") == 0)
                        turnon = 1;
                      else if (strcmp(cmdargv[1], "off") == 0)
                        turnon = 0;
                      else
                        {
                          WARNPRINT("Can't understand pcontrol argument - "
                                    "turning pcontrol off");
                          turnon = 0;
                        }
                    }
                  modules.module[modules.num - 1]->pcontrol = turnon;
                }
              else
                {
                  WARNPRINT(
                    "No module active - ignoring local command pcontrol");
                }
            }
          else if ((cmdargc == 2) &&
                   (strcmp(cmdargv[0], "globalpcontrol") == 0))
            {
              /* find global pcontrol setting with no extra argument */

              if (strcmp(cmdargv[1], "on") == 0)
                modules.pcontrol = PNMPI_PCONTROL_ON;
              else if (strcmp(cmdargv[1], "off") == 0)
                modules.pcontrol = PNMPI_PCONTROL_OFF;
              else if (strcmp(cmdargv[1], "pmpi") == 0)
                modules.pcontrol = PNMPI_PCONTROL_PMPI;
              else if (strcmp(cmdargv[1], "pnmpi") == 0)
                modules.pcontrol = PNMPI_PCONTROL_PNMPI;
              else if (strcmp(cmdargv[1], "mixed") == 0)
                modules.pcontrol = PNMPI_PCONTROL_MIXED;
              else if (strcmp(cmdargv[1], "int") == 0)
                modules.pcontrol = PNMPI_PCONTROL_INT;
              else
                {
                  WARNPRINT(
                    "Can't understand globalpcontrol argument - ignoring it");
                }
            }
          else if ((cmdargc == 4) &&
                   (strcmp(cmdargv[0], "globalpcontrol") == 0))
            {
              /* find global pcontrol setting with two extra arguments */

              if (strcmp(cmdargv[1], "typed") == 0)
                {
                  modules.pcontrol = PNMPI_PCONTROL_TYPED;
                  modules.pcontrol_typed_level = atoi(cmdargv[2]);
                  if (strcmp(cmdargv[3], "int") == 0)
                    modules.pcontrol_typed_type = PNMPI_PCONTROL_TYPE_INT;
                  else if (strcmp(cmdargv[3], "pointer") == 0)
                    modules.pcontrol_typed_type = PNMPI_PCONTROL_TYPE_PTR;
                  else if (strcmp(cmdargv[3], "double") == 0)
                    modules.pcontrol_typed_type = PNMPI_PCONTROL_TYPE_DOUBLE;
                  else if (strcmp(cmdargv[3], "long") == 0)
                    modules.pcontrol_typed_type = PNMPI_PCONTROL_TYPE_LONG;
                  else
                    {
                      WARNPRINT("Can't understand globalpcontrol argument - "
                                "ignoring it");
                    }
                }
              else
                {
                  WARNPRINT(
                    "Can't understand globalpcontrol argument - ignoring it");
                }
            }
          else if ((cmdargc >= 3) && (strcmp(cmdargv[0], "argument") == 0))
            {
              /* check if module is active */

              if (modules.num > 0)
                {
                  module_arg_p arg, argl;

                  /* record argumemt */

                  arg = (module_arg_p)malloc(sizeof(module_arg_t));
                  if (arg == NULL)
                    {
                      WARNPRINT("No memory to allocate argument - ignoring it");
                    }
                  else
                    {
                      if (strlen(cmdargv[1]) >= MAX_ARG_NAME)
                        WARNPRINT("Argument name too long - trunacting it");
                      if (strlen(cmdargv[2]) >= MAX_ARG_VALUE)
                        WARNPRINT("Argument value too long - trunacting it");
                      strncpy(arg->name, cmdargv[1], MAX_ARG_NAME);
                      strncpy(arg->value, cmdargv[2], MAX_ARG_VALUE);
                      arg->name[MAX_ARG_NAME - 1] = (char)0;
                      arg->value[MAX_ARG_VALUE - 1] = (char)0;
                      argl = modules.module[modules.num - 1]->args;
                      arg->next = NULL;
                      if (argl)
                        {
                          while (argl->next)
                            argl = argl->next;
                          argl->next = arg;
                        }
                      else
                        modules.module[modules.num - 1]->args = arg;
                    }
                }
              else
                {
                  WARNPRINT(
                    "No module active - ignoring local command argument");
                }
            }
          else
            {
              WARNPRINT("Illegal command %s - ignoring it", cmdargv[0]);
            }
        } /* while eof */
    }     /* if file open */

  /*Free what we allocated*/
  if (libdir != PNMPI_MODULES_DIR)
    free(libdir);
  if (conffile)
    fclose(conffile);

  /*
   * Call the module registration point functions
   * (done now as to load arguments first)
   */
  {
    int i;

    for (i = 0; i < modules.num; i++)
      {
        // continue for stacks (they have no reg point)
        if (modules.module[i]->stack_delimiter)
          continue;

        regPoint = (PNMPI_RegistrationPoint_t)dlsym(modules.module[i]->handle,
                                                    PNMPI_REGISTRATION_POINT);
        if (regPoint != 0)
          {
            /* check if this module has a RegistrationPoint and if yes, all it
             */

            pnmpi_level = i;
            err = regPoint();
            if (err != PNMPI_SUCCESS)
              {
                WARNPRINT("Error registering module %s (Error %i)", modname,
                          err);
              }
          }
        else
          {
            DBGPRINT2("Module %s has no registration point", modname);
          }
      } /*for modules*/
  }     /*call PNMPI_REGISTRATION_POINT*/

/* if we are debugging, print the parsed information */

#ifdef DBGLEVEL
  {
    int _i, _j;
    module_arg_p arg;
    DBGPRINT4("Parsed information from configuration file");
    for (_i = 0; _i < modules.num; _i++)
      {
        DBGPRINT4("- Module %s (pcontrol %i)", modules.module[_i]->name,
                  modules.module[_i]->pcontrol);
        _j = 1;
        arg = modules.module[_i]->args;
        while (arg != NULL)
          {
            DBGPRINT4("\t Argument %2i: %s = %s", _j, arg->name, arg->value);
            arg = arg->next;
            _j++;
          }
      }
    DBGPRINT4("\n");
  }
#endif /* DBGLEVEL */


  /* Initialize and load the indirection arrays */

  if (modules.num > 0)
    {
      INITIALIZE_ALL_FUNCTION_STACKS(modules);
    }

#ifdef DBGLEVEL
  {
    int _i;
    for (_i = 0; _i < NUM_MPI_CELLS; _i++)
      {
        DBGPRINT3("Cell %i = %lx", _i, pnmpi_activated[_i]);
      }
  }
#endif

  /* fix variables */
  pnmpi_max_level = modules.num;
  pnmpi_level = 0;
}
