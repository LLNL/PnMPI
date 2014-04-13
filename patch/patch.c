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

/* objcopy.c -- copy object file from input to output, optionally massaging it.
   Copyright 1991, 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000,
   2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This file is part of GNU Binutils.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA
   02110-1301, USA.  */

#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <pnmpi-config.h>

#if PNMPI_HAVE_BFD == TRUE
#include <bfd.h>
#include <libiberty.h>
#endif

#ifdef AIX
#define DYNSTAB ".loader"
#else
#define DYNSTAB ".dynstr"
#endif

char prefix0, prefix1, prefix2;

int verbose = 0;
int copyonly = 0;
int status = 0;

size_t num_generic_symbols = 0;
size_t num_dynamic_symbols = 0;

/*=======================================================================*/
/*=======================================================================*/
/* BFD BASED COPY BASED ON objcopy from the binutils */
/*=======================================================================*/

#if PNMPI_HAVE_BFD == TRUE

static asymbol **isympp = NULL; /* Input symbols.  */
static asymbol **osympp = NULL; /* Output symbols that survive stripping.  */


/* Structure used to hold lists of sections and actions to take.  */
struct section_list
{
  struct section_list *next; /* Next section to change.  */
  const char *name;          /* Section name.  */
  bfd_boolean used;          /* Whether this entry was used.  */
  bfd_boolean remove;        /* Whether to remove this section.  */
  bfd_boolean copy;          /* Whether to copy this section.  */
  bfd_vma vma_val;           /* Amount to change by or set to.  */
  bfd_vma lma_val;           /* Amount to change by or set to.  */
  bfd_boolean set_flags;     /* Whether to set the section flags.	 */
  flagword flags;            /* What to set the section flags to.	 */
};

static struct section_list *change_sections;


#define FATAL_ERROR(a) \
  {                    \
    printf("%s", a);   \
    exit(1);           \
  }
#define RETURN_NONFATAL(a) FATAL_ERROR("Found problem during copy")


#define BUFSIZE 8192



void hook_dynstab(char *mem, bfd_size_type size)
{
  int i, c0, c1, c2;
  int c3;

  c0 = ' ';
  c1 = ' ';
  c2 = ' ';

  if (verbose)
    {
      printf("BEFORE:\n");
      for (i = 0; i < size; i++)
        {
          if (((int)mem[i]) >= 32)
            {
              printf("%c", mem[i]);
            }
          else
            {
              printf("-");
            }
        }
      printf("\n\n");
    }

  for (i = 0; i < size; i++)
    {
      c3 = mem[i];
      if (!copyonly)
        {
          if ((c3 == 'I') && (c2 == 'P') && (c1 == 'M') && (c0 == 'P'))
            {
              mem[i - 3] = 'X';
              printf("Found dynamic PNMPI symbol\n");
            }
        }
      c0 = c1;
      c1 = c2;
      c2 = c3;
    }

  if (verbose)
    {
      printf("BEFORE:\n");
      for (i = 0; i < size; i++)
        {
          if (((int)mem[i]) >= 32)
            {
              printf("%c", mem[i]);
            }
          else
            {
              printf("-");
            }
        }
      printf("\n\n");
    }
}



extern void *bfd_malloc(bfd_size_type size);

#if 0
{
  void *ptr;

  if (size != (size_t) size)
    {
      bfd_set_error (bfd_error_no_memory);
      return NULL;
    }

  ptr = malloc ((size_t) size);
  if (ptr == NULL && (size_t) size != 0)
    bfd_set_error (bfd_error_no_memory);

  return ptr;
}
#endif

void bfd_nonfatal(const char *string)
{
  const char *errmsg = bfd_errmsg(bfd_get_error());

  if (string)
    fprintf(stderr, "REPORT: %s: %s\n", string, errmsg);
  else
    fprintf(stderr, "REPORT: %s\n", errmsg);
}

void non_fatal VPARAMS((const char *format, ...))
{
  VA_OPEN(args, format);
  VA_FIXEDARG(args, const char *, format);
  vfprintf(stderr, format, args);
  putc('\n', stderr);
  VA_CLOSE(args);
}

void fatal VPARAMS((const char *format, ...))
{
  VA_OPEN(args, format);
  VA_FIXEDARG(args, const char *, format);
  vfprintf(stderr, format, args);
  VA_CLOSE(args);
  xexit(1);
}


void list_matching_formats(char **p)
{
  fprintf(stderr, "patching lib: Matching formats:");
  while (*p)
    fprintf(stderr, " %s", *p++);
  fputc('\n', stderr);
}



const char *bfd_get_archive_filename(bfd *abfd)
{
  static size_t curr = 0;
  static char *buf;
  size_t needed;

  assert(abfd != NULL);

  if (!abfd->my_archive)
    return bfd_get_filename(abfd);

  needed = (strlen(bfd_get_filename(abfd->my_archive)) +
            strlen(bfd_get_filename(abfd)) + 3);
  if (needed > curr)
    {
      if (curr)
        free(buf);
      curr = needed + (needed >> 1);
      buf = bfd_malloc(curr);
      /* If we can't malloc, fail safe by returning just the file name.
         This function is only used when building error messages.  */
      if (!buf)
        {
          curr = 0;
          return bfd_get_filename(abfd);
        }
    }
  sprintf(buf, "%s(%s)", bfd_get_filename(abfd->my_archive),
          bfd_get_filename(abfd));
  return buf;
}


char *make_tempname(char *filename)
{
  static char template[] = "stXXXXXX";
  char *tmpname;
  char *slash = strrchr(filename, '/');

#ifdef HAVE_DOS_BASED_FILE_SYSTEM
  {
    /* We could have foo/bar\\baz, or foo\\bar, or d:bar.  */
    char *bslash = strrchr(filename, '\\');
    if (slash == NULL || (bslash != NULL && bslash > slash))
      slash = bslash;
    if (slash == NULL && filename[0] != '\0' && filename[1] == ':')
      slash = filename + 1;
  }
#endif

  if (slash != (char *)NULL)
    {
      char c;

      c = *slash;
      *slash = 0;
      tmpname = xmalloc(strlen(filename) + sizeof(template) + 2);
      strcpy(tmpname, filename);
#ifdef HAVE_DOS_BASED_FILE_SYSTEM
      /* If tmpname is "X:", appending a slash will make it a root
         directory on drive X, which is NOT the same as the current
         directory on drive X.  */
      if (tmpname[1] == ':' && tmpname[2] == '\0')
        strcat(tmpname, ".");
#endif
      strcat(tmpname, "/");
      strcat(tmpname, template);
      mktemp(tmpname);
      *slash = c;
    }
  else
    {
      tmpname = xmalloc(sizeof(template));
      strcpy(tmpname, template);
      mktemp(tmpname);
    }
  return tmpname;
}



off_t get_file_size(const char *file_name)
{
  struct stat statbuf;

  if (stat(file_name, &statbuf) < 0)
    {
      if (errno == ENOENT)
        non_fatal("'%s': No such file", file_name);
      else
        non_fatal("Warning: could not locate '%s'.  reason: %s", file_name,
                  strerror(errno));
    }
  else if (!S_ISREG(statbuf.st_mode))
    non_fatal("Warning: '%s' is not an ordinary file", file_name);
  else
    return statbuf.st_size;

  return 0;
}



void setup_bfd_headers(bfd *ibfd, bfd *obfd)
{
  const char *err;

  /* Allow the BFD backend to copy any private data it understands
     from the input section to the output section.  */
  if (!bfd_copy_private_header_data(ibfd, obfd))
    {
      err = "private header data";
      goto loser;
    }

  /* All went well.  */
  return;

loser:
  non_fatal("%s: error in %s: %s", bfd_get_filename(ibfd), err,
            bfd_errmsg(bfd_get_error()));
  status = 1;
}


struct section_list *find_section_list(const char *name, bfd_boolean add)
{
  struct section_list *p;

  for (p = change_sections; p != NULL; p = p->next)
    if (strcmp(p->name, name) == 0)
      return p;

  if (!add)
    return NULL;

  p = xmalloc(sizeof(struct section_list));
  p->name = name;
  p->used = FALSE;
  p->remove = FALSE;
  p->copy = FALSE;
  p->vma_val = 0;
  p->lma_val = 0;
  p->set_flags = FALSE;
  p->flags = 0;

  p->next = change_sections;
  change_sections = p;

  return p;
}



void setup_section(bfd *ibfd, sec_ptr isection, void *obfdarg)
{
  bfd *obfd = obfdarg;
  struct section_list *p;
  sec_ptr osection;
  bfd_size_type size;
  bfd_vma vma;
  bfd_vma lma;
  flagword flags;
  const char *err;
  const char *name;

  p = find_section_list(bfd_section_name(ibfd, isection), FALSE);
  if (p != NULL)
    p->used = TRUE;

  flags = bfd_get_section_flags(ibfd, isection);
  name = bfd_section_name(ibfd, isection);

  if (p != NULL && p->set_flags)
    flags = p->flags | (flags & (SEC_HAS_CONTENTS | SEC_RELOC));

  osection = bfd_make_section_anyway_with_flags(obfd, name, flags);

  if (osection == NULL)
    {
      err = "making";
      goto loser;
    }

  size = bfd_section_size(ibfd, isection);

  if (!bfd_set_section_size(obfd, osection, size))
    {
      err = "size";
      goto loser;
    }

  vma = bfd_section_vma(ibfd, isection);

  if (!bfd_set_section_vma(obfd, osection, vma))
    {
      err = "vma";
      goto loser;
    }

  lma = isection->lma;

  osection->lma = lma;

  /* FIXME: This is probably not enough.  If we change the LMA we
     may have to recompute the header for the file as well.  */
  if (!bfd_set_section_alignment(obfd, osection,
                                 bfd_section_alignment(ibfd, isection)))
    {
      err = "alignment";
      goto loser;
    }

  /* Copy merge entity size.  */
  osection->entsize = isection->entsize;

  /* This used to be mangle_section; we do here to avoid using
     bfd_get_section_by_name since some formats allow multiple
     sections with the same name.  */
  isection->output_section = osection;
  isection->output_offset = 0;

  /* Allow the BFD backend to copy any private data it understands
     from the input section to the output section.  */

  if (!bfd_copy_private_section_data(ibfd, isection, obfd, osection))
    {
      err = "private data";
      goto loser;
    }

  /* All went well.  */
  return;

loser:
  non_fatal("%s: section `%s': error in %s: %s", bfd_get_filename(ibfd),
            bfd_section_name(ibfd, isection), err, bfd_errmsg(bfd_get_error()));
  status = 1;
}



/* Copy the data of input section ISECTION of IBFD
   to an output section with the same name in OBFD.
   If stripping then don't copy any relocation info.  */

static void copy_section(bfd *ibfd, sec_ptr isection, void *obfdarg)
{
  bfd *obfd = obfdarg;
  struct section_list *p;
  arelent **relpp;
  long relcount;
  sec_ptr osection;
  bfd_size_type size;
  long relsize;
  flagword flags;

  /* If we have already failed earlier on,
     do not keep on generating complaints now.  */
  if (status != 0)
    return;

  flags = bfd_get_section_flags(ibfd, isection);
  if ((flags & SEC_GROUP) != 0)
    return;

  osection = isection->output_section;
  size = bfd_get_section_size(isection);

  if (size == 0 || osection == 0)
    return;

  p = find_section_list(bfd_get_section_name(ibfd, isection), FALSE);

  /* Core files do not need to be relocated.  */
  if (bfd_get_format(obfd) == bfd_core)
    relsize = 0;
  else
    {
      relsize = bfd_get_reloc_upper_bound(ibfd, isection);

      if (relsize < 0)
        {
          /* Do not complain if the target does not support relocations.  */
          if (relsize == -1 && bfd_get_error() == bfd_error_invalid_operation)
            relsize = 0;
          else
            RETURN_NONFATAL(bfd_get_filename(ibfd));
        }
    }

  if (relsize == 0)
    bfd_set_reloc(obfd, osection, NULL, 0);
  else
    {
      relpp = xmalloc(relsize);
      relcount = bfd_canonicalize_reloc(ibfd, isection, relpp, isympp);
      if (relcount < 0)
        RETURN_NONFATAL(bfd_get_filename(ibfd));

      bfd_set_reloc(obfd, osection, relcount == 0 ? NULL : relpp, relcount);
      if (relcount == 0)
        free(relpp);
    }

  if (bfd_get_section_flags(ibfd, isection) & SEC_HAS_CONTENTS &&
      bfd_get_section_flags(obfd, osection) & SEC_HAS_CONTENTS)
    {
      void *memhunk = xmalloc(size);

      if (!bfd_get_section_contents(ibfd, isection, memhunk, 0, size))
        RETURN_NONFATAL(bfd_get_filename(ibfd));


      /*------------------------------------------------*/
      /* PNMPI PATCHING FOR DYNAMIC SYMBOLS STARTS HERE */

      if (verbose)
        printf("Copying Section: %s\n", osection->name);

      if (!strcmp(osection->name, DYNSTAB))
        hook_dynstab(memhunk, size);

      /* PNMPI PATCHING FOR DYNAMIC SYMBOLS ENDS HERE */
      /*----------------------------------------------*/


      if (!bfd_set_section_contents(obfd, osection, memhunk, 0, size))
        RETURN_NONFATAL(bfd_get_filename(obfd));

      free(memhunk);
    }
  else if (p != NULL && p->set_flags && (p->flags & SEC_HAS_CONTENTS) != 0)
    {
      void *memhunk = xmalloc(size);

      /* We don't permit the user to turn off the SEC_HAS_CONTENTS
         flag--they can just remove the section entirely and add it
         back again.  However, we do permit them to turn on the
         SEC_HAS_CONTENTS flag, and take it to mean that the section
         contents should be zeroed out.  */

      memset(memhunk, 0, size);
      if (!bfd_set_section_contents(obfd, osection, memhunk, 0, size))
        RETURN_NONFATAL(bfd_get_filename(obfd));
      free(memhunk);
    }
}



/* Copy unkown object file IBFD onto OBFD.
   Returns TRUE upon success, FALSE otherwise.  */

bfd_boolean copy_unknown_object(bfd *ibfd, bfd *obfd)
{
  char *cbuf;
  int tocopy;
  long ncopied;
  long size;
  struct stat buf;

  if (bfd_stat_arch_elt(ibfd, &buf) != 0)
    {
      bfd_nonfatal(bfd_get_archive_filename(ibfd));
      return FALSE;
    }

  size = buf.st_size;
  if (size < 0)
    {
      non_fatal("stat returns negative size for `%s'",
                bfd_get_archive_filename(ibfd));
      return FALSE;
    }

  if (bfd_seek(ibfd, (file_ptr)0, SEEK_SET) != 0)
    {
      bfd_nonfatal(bfd_get_archive_filename(ibfd));
      return FALSE;
    }

  if (verbose)
    printf("copy from `%s' [unknown] to `%s' [unknown]\n",
           bfd_get_archive_filename(ibfd), bfd_get_filename(obfd));

  cbuf = xmalloc(BUFSIZE);
  ncopied = 0;
  while (ncopied < size)
    {
      tocopy = size - ncopied;
      if (tocopy > BUFSIZE)
        tocopy = BUFSIZE;

      if (bfd_bread(cbuf, (bfd_size_type)tocopy, ibfd) != (bfd_size_type)tocopy)
        {
          bfd_nonfatal(bfd_get_archive_filename(ibfd));
          free(cbuf);
          return FALSE;
        }

      if (bfd_bwrite(cbuf, (bfd_size_type)tocopy, obfd) !=
          (bfd_size_type)tocopy)
        {
          bfd_nonfatal(bfd_get_filename(obfd));
          free(cbuf);
          return FALSE;
        }

      ncopied += tocopy;
    }

  chmod(bfd_get_filename(obfd), buf.st_mode);
  free(cbuf);
  return TRUE;
}



/* Copy object file IBFD onto OBFD.
   Returns TRUE upon success, FALSE otherwise.  */

bfd_boolean copy_object(bfd *ibfd, bfd *obfd)
{
  bfd_vma start;
  long symcount;
  long symsize;
  void *dhandle;
  enum bfd_architecture iarch;
  unsigned int imach;
  int i;
  char *p;

  if (ibfd->xvec->byteorder != obfd->xvec->byteorder &&
      ibfd->xvec->byteorder != BFD_ENDIAN_UNKNOWN &&
      obfd->xvec->byteorder != BFD_ENDIAN_UNKNOWN)
    FATAL_ERROR("Unable to change endianness of input file(s)");

  if (!bfd_set_format(obfd, bfd_get_format(ibfd)))
    {
      bfd_nonfatal(bfd_get_filename(obfd));
      return FALSE;
    }

  if (verbose)
    printf("copy from `%s' [%s] to `%s' [%s]\n", bfd_get_archive_filename(ibfd),
           bfd_get_target(ibfd), bfd_get_filename(obfd), bfd_get_target(obfd));

  start = bfd_get_start_address(ibfd);

  /* Neither the start address nor the flags
     need to be set for a core file.  */
  if (bfd_get_format(obfd) != bfd_core)
    {
      flagword flags;

      flags = bfd_get_file_flags(ibfd);
      flags &= bfd_applicable_file_flags(obfd);

      if (!bfd_set_start_address(obfd, start) ||
          !bfd_set_file_flags(obfd, flags))
        {
          bfd_nonfatal(bfd_get_archive_filename(ibfd));
          return FALSE;
        }
    }

  /* Copy architecture of input file to output file.  */
  iarch = bfd_get_arch(ibfd);
  imach = bfd_get_mach(ibfd);
  if (!bfd_set_arch_mach(obfd, iarch, imach) &&
      (ibfd->target_defaulted || bfd_get_arch(ibfd) != bfd_get_arch(obfd)))
    {
      if (bfd_get_arch(ibfd) == bfd_arch_unknown)
        non_fatal("Unable to recognise the format of the input file `%s'",
                  bfd_get_archive_filename(ibfd));
      else
        non_fatal(
          "Warning: Output file cannot represent architecture `%s'",
          bfd_printable_arch_mach(bfd_get_arch(ibfd), bfd_get_mach(ibfd)));
      return FALSE;
    }

  if (!bfd_set_format(obfd, bfd_get_format(ibfd)))
    {
      bfd_nonfatal(bfd_get_archive_filename(ibfd));
      return FALSE;
    }

  if (isympp)
    free(isympp);

  if (osympp != isympp)
    free(osympp);

  isympp = NULL;
  osympp = NULL;

  /* BFD mandates that all output sections be created and sizes set before
     any output is done.  Thus, we traverse all sections multiple times.  */
  bfd_map_over_sections(ibfd, setup_section, obfd);

  setup_bfd_headers(ibfd, obfd);

  if (bfd_count_sections(obfd) == 0)
    {
      non_fatal("there are no sections to be copied!");
      return FALSE;
    }


  /* Symbol filtering must happen after the output sections
     have been created, but before their contents are set.  */
  dhandle = NULL;
  symsize = bfd_get_symtab_upper_bound(ibfd);
  if (symsize < 0)
    {
      bfd_nonfatal(bfd_get_archive_filename(ibfd));
      return FALSE;
    }

  osympp = isympp = xmalloc(symsize);
  symcount = bfd_canonicalize_symtab(ibfd, isympp);
  if (symcount < 0)
    {
      bfd_nonfatal(bfd_get_filename(ibfd));
      return FALSE;
    }


  /*-----------------------------------------------*/
  /* PNMPI PATCHING FOR STATIC SYMBOLS STARTS HERE */

  if (verbose)
    printf("Patching static symbol table");

  for (i = 0; i < symcount; i++)
    {
      if (strncmp(osympp[i]->name, "MPI_", 4) == 0)
        {
          printf("Found Static PMPI Symbol: %s\n", osympp[i]->name);

          p = (char *)&(osympp[i]->name[0]);
          *p = prefix0;
          p = (char *)&(osympp[i]->name[1]);
          *p = prefix1;
          p = (char *)&(osympp[i]->name[2]);
          *p = prefix2;
        }

      if (strncmp(osympp[i]->name, "PMPI_", 5) == 0)
        {
          printf("Found Static PMPI Symbol: %s\n", osympp[i]->name);

          p = (char *)&(osympp[i]->name[0]);
          *p = 'X';
          p = (char *)&(osympp[i]->name[1]);
          *p = prefix0;
          p = (char *)&(osympp[i]->name[2]);
          *p = prefix1;
          p = (char *)&(osympp[i]->name[3]);
          *p = prefix2;
        }
    }

  /* PNMPI PATCHING FOR STATIC SYMBOLS ENDS HERE */
  /*---------------------------------------------*/


  bfd_set_symtab(obfd, osympp, symcount);

  /* This has to happen after the symbol table has been set.  */
  bfd_map_over_sections(ibfd, copy_section, obfd);


  /* Allow the BFD backend to copy any private data it understands
     from the input BFD to the output BFD.  This is done last to
     permit the routine to look at the filtered symbol table, which is
     important for the ECOFF code at least.  */

  if (!bfd_copy_private_bfd_data(ibfd, obfd))
    {
      non_fatal("%s: error copying private BFD data: %s",
                bfd_get_filename(obfd), bfd_errmsg(bfd_get_error()));
      return FALSE;
    }

  return TRUE;
}



/* Read each archive element in turn from IBFD, copy the
   contents to temp file, and keep the temp file handle.  */

#undef MKDIR
#if defined(_WIN32) && !defined(__CYGWIN32__)
#define MKDIR(DIR, MODE) mkdir(DIR)
#else
#define MKDIR(DIR, MODE) mkdir(DIR, MODE)
#endif

void copy_archive(bfd *ibfd, bfd *obfd, const char *output_target)
{
  struct name_list
  {
    struct name_list *next;
    const char *name;
    bfd *obfd;
  } * list, *l;
  bfd **ptr = &obfd->archive_head;
  bfd *this_element;
  char *dir = make_tempname(bfd_get_filename(obfd));

  /* Make a temp directory to hold the contents.  */
  if (MKDIR(dir, 0700) != 0)
    fatal("cannot mkdir %s for archive copying (error: %s)", dir,
          strerror(errno));

  obfd->has_armap = ibfd->has_armap;

  list = NULL;

  this_element = bfd_openr_next_archived_file(ibfd, NULL);

  if (!bfd_set_format(obfd, bfd_get_format(ibfd)))
    RETURN_NONFATAL(bfd_get_filename(obfd));

  while (!status && this_element != NULL)
    {
      char *output_name;
      bfd *output_bfd;
      bfd *last_element;
      struct stat buf;
      bfd_boolean delete = TRUE;

      /* Create an output file for this member.  */
      output_name = concat(dir, "/", bfd_get_filename(this_element), (char *)0);

      /* If the file already exists, make another temp dir.  */
      if (stat(output_name, &buf) >= 0)
        {
          output_name = make_tempname(output_name);
          if (MKDIR(output_name, 0700) != 0)
            fatal("cannot mkdir %s for archive copying (error: %s)",
                  output_name, strerror(errno));

          l = xmalloc(sizeof(struct name_list));
          l->name = output_name;
          l->next = list;
          l->obfd = NULL;
          list = l;
          output_name =
            concat(output_name, "/", bfd_get_filename(this_element), (char *)0);
        }

      output_bfd = bfd_openw(output_name, output_target);

      l = xmalloc(sizeof(struct name_list));
      l->name = output_name;
      l->next = list;
      l->obfd = NULL;
      list = l;

      if (output_bfd == NULL)
        FATAL_ERROR("Can't open temp file");

      if (bfd_check_format(this_element, bfd_object))
        {
          delete = !copy_object(this_element, output_bfd);

          if (!delete || bfd_get_arch(this_element) != bfd_arch_unknown)
            {
              if (!bfd_close(output_bfd))
                {
                  bfd_nonfatal(bfd_get_filename(output_bfd));
                  /* Error in new object file. Don't change archive.  */
                  status = 1;
                }
            }
          else
            goto copy_unknown_element;
        }
      else
        {
          non_fatal("Unable to recognise the format of the input file `%s'",
                    bfd_get_archive_filename(this_element));

        copy_unknown_element:
          delete = !copy_unknown_object(this_element, output_bfd);
          if (!bfd_close_all_done(output_bfd))
            {
              bfd_nonfatal(bfd_get_filename(output_bfd));
              /* Error in new object file. Don't change archive.  */
              status = 1;
            }
        }

      if (delete)
        {
          unlink(output_name);
          status = 1;
        }
      else
        {
          /* Open the newly output file and attach to our list.  */
          output_bfd = bfd_openr(output_name, output_target);

          l->obfd = output_bfd;

          *ptr = output_bfd;
          // ptr = &output_bfd->next;
          ptr = &output_bfd->link_next; // new bfd?

          last_element = this_element;

          this_element = bfd_openr_next_archived_file(ibfd, last_element);

          bfd_close(last_element);
        }
    }
  *ptr = NULL;

  if (!bfd_close(obfd))
    FATAL_ERROR("Can't close output file");

  if (!bfd_close(ibfd))
    FATAL_ERROR("Can't close input file");

  /* Delete all the files that we opened.  */
  for (l = list; l != NULL; l = l->next)
    {
      if (l->obfd == NULL)
        rmdir(l->name);
      else
        {
          bfd_close(l->obfd);
          unlink(l->name);
        }
    }
  rmdir(dir);
}



void copy_file(const char *input_filename, const char *output_filename,
               const char *input_target, const char *output_target)
{
  bfd *ibfd;
  char **obj_matching;
  char **core_matching;

  if (get_file_size(input_filename) < 1)
    {
      non_fatal("error: the input file '%s' is empty", input_filename);
      status = 1;
      return;
    }

  ibfd = bfd_openr(input_filename, input_target);
  if (ibfd == NULL)
    FATAL_ERROR("Can't open input file");

  if (bfd_check_format(ibfd, bfd_archive))
    {
      bfd *obfd;

      /* bfd_get_target does not return the correct value until
         bfd_check_format succeeds.  */
      if (output_target == NULL)
        output_target = bfd_get_target(ibfd);

      obfd = bfd_openw(output_filename, output_target);
      if (obfd == NULL)
        FATAL_ERROR("Can't open output file");

      copy_archive(ibfd, obfd, output_target);
    }
  else if (bfd_check_format_matches(ibfd, bfd_object, &obj_matching))
    {
      bfd *obfd;
    do_copy:

      /* bfd_get_target does not return the correct value until
         bfd_check_format succeeds.  */
      if (output_target == NULL)
        output_target = bfd_get_target(ibfd);

      obfd = bfd_openw(output_filename, output_target);
      if (obfd == NULL)
        FATAL_ERROR("Can't open output file");

      if (!copy_object(ibfd, obfd))
        status = 1;

      if (!bfd_close(obfd))
        FATAL_ERROR("Can't close output file");

      if (!bfd_close(ibfd))
        FATAL_ERROR("Can't close input file");
    }
  else
    {
      bfd_error_type obj_error = bfd_get_error();
      bfd_error_type core_error;

      if (bfd_check_format_matches(ibfd, bfd_core, &core_matching))
        {
          /* This probably can't happen..  */
          if (obj_error == bfd_error_file_ambiguously_recognized)
            free(obj_matching);
          goto do_copy;
        }

      core_error = bfd_get_error();
      /* Report the object error in preference to the core error.  */
      if (obj_error != core_error)
        bfd_set_error(obj_error);

      bfd_nonfatal(input_filename);

      if (obj_error == bfd_error_file_ambiguously_recognized)
        {
          list_matching_formats(obj_matching);
          free(obj_matching);
        }
      if (core_error == bfd_error_file_ambiguously_recognized)
        {
          list_matching_formats(core_matching);
          free(core_matching);
        }

      status = 1;

      FATAL_ERROR("Can only work on libraries or objects");
    }
}

/*=======================================================================*/
/*=======================================================================*/
/* DIRECT COPY */
/*=======================================================================*/

#else

void copy_file(const char *input_filename, const char *output_filename,
               const char *input_target, const char *output_target)
{
  int res, fd_in, fd_out;
  char c1, c2, c3, c0, c4;
  char cOld = 'a';

  fd_in = open(input_filename, O_RDONLY);
  fd_out = open(output_filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);

  if (verbose)
    printf("In %i Out %i\n", fd_in, fd_out);

  read(fd_in, &c0, 1);
  read(fd_in, &c1, 1);
  read(fd_in, &c2, 1);
  read(fd_in, &c3, 1);

  do
    {
      res = read(fd_in, &c4, 1);
      if (!copyonly)
        {
          if (res == 1)
            {
              if ((c4 == '_') && (c3 == 'I') && (c2 == 'P') && (c1 == 'M') &&
                  (c0 == 'P'))
                {
                  c0 = 'X';
                  if (verbose)
                    printf("Found a dynamic symbol\n");
                  num_dynamic_symbols++;
                }
              if ((c3 == '_') && (c2 == 'I') && (c1 == 'P') && (c0 == 'M'))
                {
                  c0 = prefix0;
                  c1 = prefix1;
                  c2 = prefix2;

                  if (verbose)
                    printf("Found generic symbol\n");
                  num_generic_symbols++;
                }
#if 0
          if ((c4=='r') && (c3=='_') && (c2=='i') && (c1=='p') && (c0=='m'))
          {
            c0='p';
            c1='n';
            c2='m';
            c3='p';
            c4='i';
            if (verbose) printf("Found library name\n");
          }
#endif
            }
          write(fd_out, &c0, 1);

          c0 = c1;
          c1 = c2;
          c2 = c3;
          c3 = c4;
        }
    }
  while (res == 1);

  write(fd_out, &c1, 1);
  write(fd_out, &c2, 1);
  write(fd_out, &c3, 1);
  write(fd_out, &c4, 1);

  close(fd_in);
  close(fd_out);
}



/*=======================================================================*/
/*=======================================================================*/
/* COMMON MAIN */
/*=======================================================================*/

#endif

int main(int argc, char *argv[])
{
  int argvindex;
  char *input_filename = NULL;
  char *output_filename = NULL;
  int efargc;

  efargc = argc;
  verbose = 0;
  copyonly = 0;
  if (efargc > 3)
    {
      if (strcmp(argv[1], "-v") == 0)
        {
          verbose = 1;
        }
      if (strcmp(argv[1], "-c") == 0)
        {
          copyonly = 1;
        }
      efargc--;
    }

  if (efargc > 3)
    {
      if (strcmp(argv[1], "-v") == 0)
        {
          verbose = 1;
        }
      if (strcmp(argv[1], "-c") == 0)
        {
          copyonly = 1;
        }
      efargc--;
    }

  argvindex = argc - efargc;

  if ((efargc != 3) && (efargc != 4))
    {
      printf("Usage: patch [-v] [-c] <in-tool> <out-tool> [<prefix>]\n");
      exit(1);
    }

  input_filename = argv[argvindex + 1];
  output_filename = argv[argvindex + 2];
  if (efargc == 4)
    {
      if (strlen(argv[argvindex + 3]) != 3)
        {
          printf("Usage: patch [-v] <in-tool> <out-tool> [<prefix>]\n");
          printf("prefix has to be exactly three characters.\n");
          exit(1);
        }
      prefix0 = argv[argvindex + 3][0];
      prefix1 = argv[argvindex + 3][1];
      prefix2 = argv[argvindex + 3][2];
    }
  else
    {
      prefix0 = 'M';
      prefix1 = 'P';
      prefix2 = 'I';
    }

  copy_file(input_filename, output_filename, NULL, NULL);

  printf("Patched %d dynamic symbols.\n", num_dynamic_symbols);
  printf("Patched %d generic symbols.\n", num_generic_symbols);

  return status;
}
