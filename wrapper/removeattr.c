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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char compare[100]="__attribute__";

int main (int argc, char** argv)
{
  int par=0;
  int attrfound=0;
  int printme=0;
  char buffer[100],inp;
  int ptr=0;
  int i;

  /* loop over input */

  while (!(feof(stdin)))
    {
      inp=(char) fgetc(stdin);

      if (attrfound)
      {
        if (inp=='(')
        {
          par++;
          printme=0;
        }
        else
        {
          if (inp==')')
          {
            par--;
            if (par==0)
            {
              attrfound=0;
            }
            printme=0;
          }
          else
          {
            if (par==0)
              {
                printme=1;
              }
            else
            {
              printme=0;
            }
          }
        }
      }
      else
      {
        if (inp==compare[ptr])
        {
          ptr++;
          if (ptr==strlen(compare))
          {
            attrfound=1;
            ptr=0;
          }
          printme=0;
        }
        else
        {
          printme=1;
          for (i=0; i<ptr; i++)
          {
            printf("%c",compare[i]);
          }
          ptr=0;
        }
      }
      if (printme) printf("%c",inp);
    }
}
