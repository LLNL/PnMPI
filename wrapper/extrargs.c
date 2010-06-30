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

#define MAXSTR 200
#define MAXARG 30

#define LOC_TYPES 0
#define LOC_NAMES 1
#define LOC_SPCLS 2

typedef char mystring[MAXSTR];

int main (int argc, char** argv)
{
  mystring types[MAXARG];
  mystring names[MAXARG];
  mystring spcls[MAXARG];

  int           num,ct,cn,cs,i,loc;
  unsigned char inp='A',last;

  /* reset variables */
  
  num=0;
  for (i=0; i<MAXARG; i++)
    {
      strcpy(types[i],"");
      strcpy(names[i],"");
      strcpy(spcls[i],"");
    }

  /* find leading ( */

  while ((!(feof(stdin)))&&(inp!='('))
    {
      inp=(unsigned char) fgetc(stdin);
    }

  if (feof(stdin))
    {
      fprintf(stderr,"WARNING: NO ( FOUND.\n");
      exit(1);
    }

  /* now the arguments start */

  loc=LOC_TYPES;
  ct=0;
  cn=0;
  cs=0;
  while ((!(feof(stdin)))&&(inp!=')'))
    {
      last=inp;
      inp=(unsigned char) fgetc(stdin);

      if ((inp==' ')||(inp=='\t')||(inp=='\n'))
	{
	  if (((inp==' ')||(inp=='\t')||(inp=='\t')) && ((last!=' ')&&(inp!='\t')&&(last!='\n')))
	    {
	      if (loc==LOC_TYPES)
		{
		  if (ct>0)
		    {
		      loc=LOC_NAMES;
		      types[num][ct]=(char) 0;
		      cn=0;
		      cs=0;
		    }
		}
	      if (loc==LOC_NAMES)
		{
		  if (cn>0)
		    {
		      loc=LOC_TYPES;
		      names[num][cn]=(char) 0;
		      ct=0;
		    }
		}
	      if (loc==LOC_SPCLS)
		{
		  if (cs>0)
		    {
		      loc=LOC_NAMES;
		      spcls[num][cs]=(char) 0;
		    }
		}
	    }
	}
      else
	{
	  if (inp==',')
	    {
	      if ((last!=' ')&&(inp!='\t')&&(last!='\n'))
		{
		  names[num][cn]=(char) 0;
		  types[num][ct]=(char) 0;
		  spcls[num][cs]=(char) 0;
		}
	      loc=LOC_TYPES;
	      cn=0;
	      ct=0;
	      cs=0;
	      num++;
	      names[num][cn]=(char) 0;
	      types[num][ct]=(char) 0;
	      spcls[num][cs]=(char) 0;
	    }
	  else
	    {
	      if (inp!=')')
		{
		  if ((loc==LOC_TYPES)||((cn==0)&&(inp=='*')))
		    {
		      if ((cn==0)&&(inp=='*'))
			{
			  types[num][ct]=' ';
			  ct++;
			}
		      types[num][ct]=inp;
		      ct++;
		    }
		  else
		    {
		      if (inp=='[')
			{
			  loc=LOC_SPCLS;
			  types[num][ct]=(char) 0;
			}
		      if (loc==LOC_SPCLS)
			{
			  spcls[num][cs]=inp;
			  cs++;
			}
		      else
			{
			  names[num][cn]=inp;
			  cn++;
			}
		      if (inp==']')
			{
			  loc=LOC_NAMES;
			  spcls[num][cs]=(char) 0;
			}
		    }
		}
	    }
	}
	  
    }

  if ((last!=' ')&&(inp!='\t')&&(last!='\n'))
    {
      types[num][ct] = (char)0;

		/* new - added due to bug on OSX - not fully verified */
		names[num][cn]=(char) 0;
		spcls[num][cs]=(char) 0;
	}

  if (feof(stdin))
    {
      fprintf(stderr,"WARNING: NO ) FOUND.\n");
      exit(1);
    }
  
  /* special case: void argument */
  
  num++;
    
  if ((num==1) && ((strcmp(types[0],"")==0)||(strcmp(types[0],"void")==0)))
    {
      num=0;
    }
    
  /* set names if necessary */

  for (i=0; i<num; i++)
    {
      if (strcmp(names[i],"")==0)
	{
	  sprintf(names[i],"_pnmpi_arg_%i",i);
	}
    }

  /* print results */

  printf("%i\n",num);

  for (i=0; i<num; i++)
    {
      printf("%s\n",types[i]);
      printf("%s\n",names[i]);
      printf("%s\n",spcls[i]);
    }

  return 0;
}
