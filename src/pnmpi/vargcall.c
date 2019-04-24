/* This file is part of P^nMPI.
 *
 * Copyright (c)
 *  2008-2019 Lawrence Livermore National Laboratories, United States of America
 *  2011-2016 ZIH, Technische Universitaet Dresden, Federal Republic of Germany
 *  2013-2019 RWTH Aachen University, Federal Republic of Germany
 *
 *
 * P^nMPI is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation version 2.1 dated February 1999.
 *
 * P^nMPI is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with P^nMPI; if not, write to the
 *
 *   Free Software Foundation, Inc.
 *   51 Franklin St, Fifth Floor
 *   Boston, MA 02110, USA
 *
 *
 * Written by Martin Schulz, schulzm@llnl.gov.
 *
 * LLNL-CODE-402774
 */

case 0: return fct(level);
case 1: return fct(level, args[0]);
case 2: return fct(level, args[0], args[1]);
case 3: return fct(level, args[0], args[1], args[2]);
case 4: return fct(level, args[0], args[1], args[2], args[3]);
case 5: return fct(level, args[0], args[1], args[2], args[3], args[4]);
case 6: return fct(level, args[0], args[1], args[2], args[3], args[4], args[5]);
case 7:
return fct(level, args[0], args[1], args[2], args[3], args[4], args[5],
           args[6]);
case 8:
return fct(level, args[0], args[1], args[2], args[3], args[4], args[5], args[6],
           args[7]);
case 9: return fct(level, *((dummy10_t *)&(args[0])));
case 10: return fct(level, *((dummy10_t *)&(args[0])));
case 11: return fct(level, *((dummy10_t *)&(args[0])), args[10]);
case 12: return fct(level, *((dummy10_t *)&(args[0])), args[10], args[11]);
case 13:
return fct(level, *((dummy10_t *)&(args[0])), args[10], args[11], args[12]);
case 14:
return fct(level, *((dummy10_t *)&(args[0])), args[10], args[11], args[12],
           args[13]);
case 15:
return fct(level, *((dummy10_t *)&(args[0])), args[10], args[11], args[12],
           args[13], args[14]);
case 16:
return fct(level, *((dummy10_t *)&(args[0])), args[10], args[11], args[12],
           args[13], args[14], args[15]);
case 17:
return fct(level, *((dummy10_t *)&(args[0])), args[10], args[11], args[12],
           args[13], args[14], args[15], args[16]);
case 18:
return fct(level, *((dummy10_t *)&(args[0])), args[10], args[11], args[12],
           args[13], args[14], args[15], args[16], args[17]);
case 19:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])));
case 20:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])));
case 21:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           args[20]);
case 22:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           args[20], args[21]);
case 23:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           args[20], args[21], args[22]);
case 24:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           args[20], args[21], args[22], args[23]);
case 25:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           args[20], args[21], args[22], args[23], args[24]);
case 26:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           args[20], args[21], args[22], args[23], args[24], args[25]);
case 27:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           args[20], args[21], args[22], args[23], args[24], args[25],
           args[26]);
case 28:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           args[20], args[21], args[22], args[23], args[24], args[25], args[26],
           args[27]);
case 29:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])));
case 30:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])));
case 31:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), args[30]);
case 32:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), args[30], args[31]);
case 33:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), args[30], args[31], args[32]);
case 34:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), args[30], args[31], args[32], args[33]);
case 35:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), args[30], args[31], args[32], args[33],
           args[34]);
case 36:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), args[30], args[31], args[32], args[33],
           args[34], args[35]);
case 37:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), args[30], args[31], args[32], args[33],
           args[34], args[35], args[36]);
case 38:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), args[30], args[31], args[32], args[33],
           args[34], args[35], args[36], args[37]);
case 39:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])));
case 40:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])));
case 41:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])), args[40]);
case 42:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])), args[40],
           args[41]);
case 43:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])), args[40],
           args[41], args[42]);
case 44:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])), args[40],
           args[41], args[42], args[43]);
case 45:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])), args[40],
           args[41], args[42], args[43], args[44]);
case 46:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])), args[40],
           args[41], args[42], args[43], args[44], args[45]);
case 47:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])), args[40],
           args[41], args[42], args[43], args[44], args[45], args[46]);
case 48:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])), args[40],
           args[41], args[42], args[43], args[44], args[45], args[46],
           args[47]);
case 49:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])));
case 50:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])));
case 51:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), args[50]);
case 52:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), args[50], args[51]);
case 53:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), args[50], args[51], args[52]);
case 54:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), args[50], args[51], args[52], args[53]);
case 55:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), args[50], args[51], args[52], args[53],
           args[54]);
case 56:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), args[50], args[51], args[52], args[53],
           args[54], args[55]);
case 57:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), args[50], args[51], args[52], args[53],
           args[54], args[55], args[56]);
case 58:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), args[50], args[51], args[52], args[53],
           args[54], args[55], args[56], args[57]);
case 59:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])));
case 60:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])));
case 61:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])), args[60]);
case 62:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])), args[60],
           args[61]);
case 63:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])), args[60],
           args[61], args[62]);
case 64:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])), args[60],
           args[61], args[62], args[63]);
case 65:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])), args[60],
           args[61], args[62], args[63], args[64]);
case 66:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])), args[60],
           args[61], args[62], args[63], args[64], args[65]);
case 67:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])), args[60],
           args[61], args[62], args[63], args[64], args[65], args[66]);
case 68:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])), args[60],
           args[61], args[62], args[63], args[64], args[65], args[66],
           args[67]);
case 69:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])));
case 70:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])));
case 71:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), args[70]);
case 72:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), args[70], args[71]);
case 73:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), args[70], args[71], args[72]);
case 74:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), args[70], args[71], args[72], args[73]);
case 75:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), args[70], args[71], args[72], args[73],
           args[74]);
case 76:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), args[70], args[71], args[72], args[73],
           args[74], args[75]);
case 77:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), args[70], args[71], args[72], args[73],
           args[74], args[75], args[76]);
case 78:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), args[70], args[71], args[72], args[73],
           args[74], args[75], args[76], args[77]);
case 79:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])));
case 80:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])));
case 81:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])), args[80]);
case 82:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])), args[80],
           args[81]);
case 83:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])), args[80],
           args[81], args[82]);
case 84:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])), args[80],
           args[81], args[82], args[83]);
case 85:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])), args[80],
           args[81], args[82], args[83], args[84]);
case 86:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])), args[80],
           args[81], args[82], args[83], args[84], args[85]);
case 87:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])), args[80],
           args[81], args[82], args[83], args[84], args[85], args[86]);
case 88:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])), args[80],
           args[81], args[82], args[83], args[84], args[85], args[86],
           args[87]);
case 89:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])),
           *((dummy10_t *)&(args[80])));
case 90:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])),
           *((dummy10_t *)&(args[80])));
case 91:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])),
           *((dummy10_t *)&(args[80])), args[90]);
case 92:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])),
           *((dummy10_t *)&(args[80])), args[90], args[91]);
case 93:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])),
           *((dummy10_t *)&(args[80])), args[90], args[91], args[92]);
case 94:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])),
           *((dummy10_t *)&(args[80])), args[90], args[91], args[92], args[93]);
case 95:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])),
           *((dummy10_t *)&(args[80])), args[90], args[91], args[92], args[93],
           args[94]);
case 96:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])),
           *((dummy10_t *)&(args[80])), args[90], args[91], args[92], args[93],
           args[94], args[95]);
case 97:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])),
           *((dummy10_t *)&(args[80])), args[90], args[91], args[92], args[93],
           args[94], args[95], args[96]);
case 98:
return fct(level, *((dummy10_t *)&(args[0])), *((dummy10_t *)&(args[10])),
           *((dummy10_t *)&(args[20])), *((dummy10_t *)&(args[30])),
           *((dummy10_t *)&(args[40])), *((dummy10_t *)&(args[50])),
           *((dummy10_t *)&(args[60])), *((dummy10_t *)&(args[70])),
           *((dummy10_t *)&(args[80])), args[90], args[91], args[92], args[93],
           args[94], args[95], args[96], args[97]);
case 99: return fct(level, *((dummy100_t *)&(args[0])));
case 100: return fct(level, *((dummy100_t *)&(args[0])));
case 101: return fct(level, *((dummy100_t *)&(args[0])), args[100]);
case 102: return fct(level, *((dummy100_t *)&(args[0])), args[100], args[101]);
case 103:
return fct(level, *((dummy100_t *)&(args[0])), args[100], args[101], args[102]);
case 104:
return fct(level, *((dummy100_t *)&(args[0])), args[100], args[101], args[102],
           args[103]);
case 105:
return fct(level, *((dummy100_t *)&(args[0])), args[100], args[101], args[102],
           args[103], args[104]);
case 106:
return fct(level, *((dummy100_t *)&(args[0])), args[100], args[101], args[102],
           args[103], args[104], args[105]);
case 107:
return fct(level, *((dummy100_t *)&(args[0])), args[100], args[101], args[102],
           args[103], args[104], args[105], args[106]);
case 108:
return fct(level, *((dummy100_t *)&(args[0])), args[100], args[101], args[102],
           args[103], args[104], args[105], args[106], args[107]);
case 109:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])));
case 110:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])));
case 111:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           args[110]);
case 112:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           args[110], args[111]);
case 113:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           args[110], args[111], args[112]);
case 114:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           args[110], args[111], args[112], args[113]);
case 115:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           args[110], args[111], args[112], args[113], args[114]);
case 116:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           args[110], args[111], args[112], args[113], args[114], args[115]);
case 117:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           args[110], args[111], args[112], args[113], args[114], args[115],
           args[116]);
case 118:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           args[110], args[111], args[112], args[113], args[114], args[115],
           args[116], args[117]);
case 119:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])));
case 120:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])));
case 121:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), args[120]);
case 122:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), args[120], args[121]);
case 123:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), args[120], args[121], args[122]);
case 124:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), args[120], args[121], args[122],
           args[123]);
case 125:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), args[120], args[121], args[122],
           args[123], args[124]);
case 126:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), args[120], args[121], args[122],
           args[123], args[124], args[125]);
case 127:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), args[120], args[121], args[122],
           args[123], args[124], args[125], args[126]);
case 128:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), args[120], args[121], args[122],
           args[123], args[124], args[125], args[126], args[127]);
case 129:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])));
case 130:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])));
case 131:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           args[130]);
case 132:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           args[130], args[131]);
case 133:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           args[130], args[131], args[132]);
case 134:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           args[130], args[131], args[132], args[133]);
case 135:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           args[130], args[131], args[132], args[133], args[134]);
case 136:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           args[130], args[131], args[132], args[133], args[134], args[135]);
case 137:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           args[130], args[131], args[132], args[133], args[134], args[135],
           args[136]);
case 138:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           args[130], args[131], args[132], args[133], args[134], args[135],
           args[136], args[137]);
case 139:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])));
case 140:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])));
case 141:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), args[140]);
case 142:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), args[140], args[141]);
case 143:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), args[140], args[141], args[142]);
case 144:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), args[140], args[141], args[142],
           args[143]);
case 145:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), args[140], args[141], args[142],
           args[143], args[144]);
case 146:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), args[140], args[141], args[142],
           args[143], args[144], args[145]);
case 147:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), args[140], args[141], args[142],
           args[143], args[144], args[145], args[146]);
case 148:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), args[140], args[141], args[142],
           args[143], args[144], args[145], args[146], args[147]);
case 149:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])));
case 150:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])));
case 151:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           args[150]);
case 152:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           args[150], args[151]);
case 153:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           args[150], args[151], args[152]);
case 154:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           args[150], args[151], args[152], args[153]);
case 155:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           args[150], args[151], args[152], args[153], args[154]);
case 156:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           args[150], args[151], args[152], args[153], args[154], args[155]);
case 157:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           args[150], args[151], args[152], args[153], args[154], args[155],
           args[156]);
case 158:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           args[150], args[151], args[152], args[153], args[154], args[155],
           args[156], args[157]);
case 159:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])));
case 160:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])));
case 161:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), args[160]);
case 162:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), args[160], args[161]);
case 163:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), args[160], args[161], args[162]);
case 164:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), args[160], args[161], args[162],
           args[163]);
case 165:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), args[160], args[161], args[162],
           args[163], args[164]);
case 166:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), args[160], args[161], args[162],
           args[163], args[164], args[165]);
case 167:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), args[160], args[161], args[162],
           args[163], args[164], args[165], args[166]);
case 168:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), args[160], args[161], args[162],
           args[163], args[164], args[165], args[166], args[167]);
case 169:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])));
case 170:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])));
case 171:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           args[170]);
case 172:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           args[170], args[171]);
case 173:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           args[170], args[171], args[172]);
case 174:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           args[170], args[171], args[172], args[173]);
case 175:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           args[170], args[171], args[172], args[173], args[174]);
case 176:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           args[170], args[171], args[172], args[173], args[174], args[175]);
case 177:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           args[170], args[171], args[172], args[173], args[174], args[175],
           args[176]);
case 178:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           args[170], args[171], args[172], args[173], args[174], args[175],
           args[176], args[177]);
case 179:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])));
case 180:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])));
case 181:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), args[180]);
case 182:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), args[180], args[181]);
case 183:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), args[180], args[181], args[182]);
case 184:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), args[180], args[181], args[182],
           args[183]);
case 185:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), args[180], args[181], args[182],
           args[183], args[184]);
case 186:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), args[180], args[181], args[182],
           args[183], args[184], args[185]);
case 187:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), args[180], args[181], args[182],
           args[183], args[184], args[185], args[186]);
case 188:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), args[180], args[181], args[182],
           args[183], args[184], args[185], args[186], args[187]);
case 189:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), *((dummy10_t *)&(args[180])));
case 190:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), *((dummy10_t *)&(args[180])));
case 191:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), *((dummy10_t *)&(args[180])),
           args[190]);
case 192:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), *((dummy10_t *)&(args[180])),
           args[190], args[191]);
case 193:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), *((dummy10_t *)&(args[180])),
           args[190], args[191], args[192]);
case 194:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), *((dummy10_t *)&(args[180])),
           args[190], args[191], args[192], args[193]);
case 195:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), *((dummy10_t *)&(args[180])),
           args[190], args[191], args[192], args[193], args[194]);
case 196:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), *((dummy10_t *)&(args[180])),
           args[190], args[191], args[192], args[193], args[194], args[195]);
case 197:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), *((dummy10_t *)&(args[180])),
           args[190], args[191], args[192], args[193], args[194], args[195],
           args[196]);
case 198:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy10_t *)&(args[100])),
           *((dummy10_t *)&(args[110])), *((dummy10_t *)&(args[120])),
           *((dummy10_t *)&(args[130])), *((dummy10_t *)&(args[140])),
           *((dummy10_t *)&(args[150])), *((dummy10_t *)&(args[160])),
           *((dummy10_t *)&(args[170])), *((dummy10_t *)&(args[180])),
           args[190], args[191], args[192], args[193], args[194], args[195],
           args[196], args[197]);
case 199:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])));
case 200:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])));
case 201:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           args[200]);
case 202:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           args[200], args[201]);
case 203:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           args[200], args[201], args[202]);
case 204:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           args[200], args[201], args[202], args[203]);
case 205:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           args[200], args[201], args[202], args[203], args[204]);
case 206:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           args[200], args[201], args[202], args[203], args[204], args[205]);
case 207:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           args[200], args[201], args[202], args[203], args[204], args[205],
           args[206]);
case 208:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           args[200], args[201], args[202], args[203], args[204], args[205],
           args[206], args[207]);
case 209:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])));
case 210:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])));
case 211:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), args[210]);
case 212:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), args[210], args[211]);
case 213:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), args[210], args[211], args[212]);
case 214:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), args[210], args[211], args[212],
           args[213]);
case 215:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), args[210], args[211], args[212],
           args[213], args[214]);
case 216:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), args[210], args[211], args[212],
           args[213], args[214], args[215]);
case 217:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), args[210], args[211], args[212],
           args[213], args[214], args[215], args[216]);
case 218:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), args[210], args[211], args[212],
           args[213], args[214], args[215], args[216], args[217]);
case 219:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])));
case 220:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])));
case 221:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           args[220]);
case 222:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           args[220], args[221]);
case 223:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           args[220], args[221], args[222]);
case 224:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           args[220], args[221], args[222], args[223]);
case 225:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           args[220], args[221], args[222], args[223], args[224]);
case 226:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           args[220], args[221], args[222], args[223], args[224], args[225]);
case 227:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           args[220], args[221], args[222], args[223], args[224], args[225],
           args[226]);
case 228:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           args[220], args[221], args[222], args[223], args[224], args[225],
           args[226], args[227]);
case 229:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])));
case 230:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])));
case 231:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), args[230]);
case 232:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), args[230], args[231]);
case 233:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), args[230], args[231], args[232]);
case 234:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), args[230], args[231], args[232],
           args[233]);
case 235:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), args[230], args[231], args[232],
           args[233], args[234]);
case 236:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), args[230], args[231], args[232],
           args[233], args[234], args[235]);
case 237:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), args[230], args[231], args[232],
           args[233], args[234], args[235], args[236]);
case 238:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), args[230], args[231], args[232],
           args[233], args[234], args[235], args[236], args[237]);
case 239:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])));
case 240:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])));
case 241:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           args[240]);
case 242:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           args[240], args[241]);
case 243:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           args[240], args[241], args[242]);
case 244:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           args[240], args[241], args[242], args[243]);
case 245:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           args[240], args[241], args[242], args[243], args[244]);
case 246:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           args[240], args[241], args[242], args[243], args[244], args[245]);
case 247:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           args[240], args[241], args[242], args[243], args[244], args[245],
           args[246]);
case 248:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           args[240], args[241], args[242], args[243], args[244], args[245],
           args[246], args[247]);
case 249:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])));
case 250:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])));
case 251:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), args[250]);
case 252:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), args[250], args[251]);
case 253:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), args[250], args[251], args[252]);
case 254:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), args[250], args[251], args[252],
           args[253]);
case 255:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), args[250], args[251], args[252],
           args[253], args[254]);
case 256:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), args[250], args[251], args[252],
           args[253], args[254], args[255]);
case 257:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), args[250], args[251], args[252],
           args[253], args[254], args[255], args[256]);
case 258:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), args[250], args[251], args[252],
           args[253], args[254], args[255], args[256], args[257]);
case 259:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])));
case 260:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])));
case 261:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           args[260]);
case 262:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           args[260], args[261]);
case 263:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           args[260], args[261], args[262]);
case 264:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           args[260], args[261], args[262], args[263]);
case 265:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           args[260], args[261], args[262], args[263], args[264]);
case 266:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           args[260], args[261], args[262], args[263], args[264], args[265]);
case 267:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           args[260], args[261], args[262], args[263], args[264], args[265],
           args[266]);
case 268:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           args[260], args[261], args[262], args[263], args[264], args[265],
           args[266], args[267]);
case 269:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])));
case 270:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])));
case 271:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), args[270]);
case 272:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), args[270], args[271]);
case 273:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), args[270], args[271], args[272]);
case 274:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), args[270], args[271], args[272],
           args[273]);
case 275:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), args[270], args[271], args[272],
           args[273], args[274]);
case 276:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), args[270], args[271], args[272],
           args[273], args[274], args[275]);
case 277:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), args[270], args[271], args[272],
           args[273], args[274], args[275], args[276]);
case 278:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), args[270], args[271], args[272],
           args[273], args[274], args[275], args[276], args[277]);
case 279:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])));
case 280:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])));
case 281:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           args[280]);
case 282:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           args[280], args[281]);
case 283:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           args[280], args[281], args[282]);
case 284:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           args[280], args[281], args[282], args[283]);
case 285:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           args[280], args[281], args[282], args[283], args[284]);
case 286:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           args[280], args[281], args[282], args[283], args[284], args[285]);
case 287:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           args[280], args[281], args[282], args[283], args[284], args[285],
           args[286]);
case 288:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           args[280], args[281], args[282], args[283], args[284], args[285],
           args[286], args[287]);
case 289:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           *((dummy10_t *)&(args[280])));
case 290:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           *((dummy10_t *)&(args[280])));
case 291:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           *((dummy10_t *)&(args[280])), args[290]);
case 292:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           *((dummy10_t *)&(args[280])), args[290], args[291]);
case 293:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           *((dummy10_t *)&(args[280])), args[290], args[291], args[292]);
case 294:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           *((dummy10_t *)&(args[280])), args[290], args[291], args[292],
           args[293]);
case 295:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           *((dummy10_t *)&(args[280])), args[290], args[291], args[292],
           args[293], args[294]);
case 296:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           *((dummy10_t *)&(args[280])), args[290], args[291], args[292],
           args[293], args[294], args[295]);
case 297:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           *((dummy10_t *)&(args[280])), args[290], args[291], args[292],
           args[293], args[294], args[295], args[296]);
case 298:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy10_t *)&(args[200])), *((dummy10_t *)&(args[210])),
           *((dummy10_t *)&(args[220])), *((dummy10_t *)&(args[230])),
           *((dummy10_t *)&(args[240])), *((dummy10_t *)&(args[250])),
           *((dummy10_t *)&(args[260])), *((dummy10_t *)&(args[270])),
           *((dummy10_t *)&(args[280])), args[290], args[291], args[292],
           args[293], args[294], args[295], args[296], args[297]);
case 299:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])));
case 300:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])));
case 301:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), args[300]);
case 302:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), args[300], args[301]);
case 303:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), args[300], args[301], args[302]);
case 304:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), args[300], args[301], args[302],
           args[303]);
case 305:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), args[300], args[301], args[302],
           args[303], args[304]);
case 306:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), args[300], args[301], args[302],
           args[303], args[304], args[305]);
case 307:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), args[300], args[301], args[302],
           args[303], args[304], args[305], args[306]);
case 308:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), args[300], args[301], args[302],
           args[303], args[304], args[305], args[306], args[307]);
case 309:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])));
case 310:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])));
case 311:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           args[310]);
case 312:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           args[310], args[311]);
case 313:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           args[310], args[311], args[312]);
case 314:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           args[310], args[311], args[312], args[313]);
case 315:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           args[310], args[311], args[312], args[313], args[314]);
case 316:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           args[310], args[311], args[312], args[313], args[314], args[315]);
case 317:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           args[310], args[311], args[312], args[313], args[314], args[315],
           args[316]);
case 318:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           args[310], args[311], args[312], args[313], args[314], args[315],
           args[316], args[317]);
case 319:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])));
case 320:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])));
case 321:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), args[320]);
case 322:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), args[320], args[321]);
case 323:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), args[320], args[321], args[322]);
case 324:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), args[320], args[321], args[322],
           args[323]);
case 325:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), args[320], args[321], args[322],
           args[323], args[324]);
case 326:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), args[320], args[321], args[322],
           args[323], args[324], args[325]);
case 327:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), args[320], args[321], args[322],
           args[323], args[324], args[325], args[326]);
case 328:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), args[320], args[321], args[322],
           args[323], args[324], args[325], args[326], args[327]);
case 329:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])));
case 330:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])));
case 331:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           args[330]);
case 332:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           args[330], args[331]);
case 333:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           args[330], args[331], args[332]);
case 334:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           args[330], args[331], args[332], args[333]);
case 335:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           args[330], args[331], args[332], args[333], args[334]);
case 336:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           args[330], args[331], args[332], args[333], args[334], args[335]);
case 337:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           args[330], args[331], args[332], args[333], args[334], args[335],
           args[336]);
case 338:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           args[330], args[331], args[332], args[333], args[334], args[335],
           args[336], args[337]);
case 339:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])));
case 340:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])));
case 341:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), args[340]);
case 342:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), args[340], args[341]);
case 343:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), args[340], args[341], args[342]);
case 344:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), args[340], args[341], args[342],
           args[343]);
case 345:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), args[340], args[341], args[342],
           args[343], args[344]);
case 346:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), args[340], args[341], args[342],
           args[343], args[344], args[345]);
case 347:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), args[340], args[341], args[342],
           args[343], args[344], args[345], args[346]);
case 348:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), args[340], args[341], args[342],
           args[343], args[344], args[345], args[346], args[347]);
case 349:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])));
case 350:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])));
case 351:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           args[350]);
case 352:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           args[350], args[351]);
case 353:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           args[350], args[351], args[352]);
case 354:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           args[350], args[351], args[352], args[353]);
case 355:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           args[350], args[351], args[352], args[353], args[354]);
case 356:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           args[350], args[351], args[352], args[353], args[354], args[355]);
case 357:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           args[350], args[351], args[352], args[353], args[354], args[355],
           args[356]);
case 358:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           args[350], args[351], args[352], args[353], args[354], args[355],
           args[356], args[357]);
case 359:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])));
case 360:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])));
case 361:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), args[360]);
case 362:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), args[360], args[361]);
case 363:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), args[360], args[361], args[362]);
case 364:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), args[360], args[361], args[362],
           args[363]);
case 365:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), args[360], args[361], args[362],
           args[363], args[364]);
case 366:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), args[360], args[361], args[362],
           args[363], args[364], args[365]);
case 367:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), args[360], args[361], args[362],
           args[363], args[364], args[365], args[366]);
case 368:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), args[360], args[361], args[362],
           args[363], args[364], args[365], args[366], args[367]);
case 369:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])));
case 370:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])));
case 371:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           args[370]);
case 372:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           args[370], args[371]);
case 373:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           args[370], args[371], args[372]);
case 374:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           args[370], args[371], args[372], args[373]);
case 375:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           args[370], args[371], args[372], args[373], args[374]);
case 376:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           args[370], args[371], args[372], args[373], args[374], args[375]);
case 377:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           args[370], args[371], args[372], args[373], args[374], args[375],
           args[376]);
case 378:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           args[370], args[371], args[372], args[373], args[374], args[375],
           args[376], args[377]);
case 379:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])));
case 380:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])));
case 381:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), args[380]);
case 382:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), args[380], args[381]);
case 383:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), args[380], args[381], args[382]);
case 384:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), args[380], args[381], args[382],
           args[383]);
case 385:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), args[380], args[381], args[382],
           args[383], args[384]);
case 386:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), args[380], args[381], args[382],
           args[383], args[384], args[385]);
case 387:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), args[380], args[381], args[382],
           args[383], args[384], args[385], args[386]);
case 388:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), args[380], args[381], args[382],
           args[383], args[384], args[385], args[386], args[387]);
case 389:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), *((dummy10_t *)&(args[380])));
case 390:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), *((dummy10_t *)&(args[380])));
case 391:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), *((dummy10_t *)&(args[380])),
           args[390]);
case 392:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), *((dummy10_t *)&(args[380])),
           args[390], args[391]);
case 393:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), *((dummy10_t *)&(args[380])),
           args[390], args[391], args[392]);
case 394:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), *((dummy10_t *)&(args[380])),
           args[390], args[391], args[392], args[393]);
case 395:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), *((dummy10_t *)&(args[380])),
           args[390], args[391], args[392], args[393], args[394]);
case 396:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), *((dummy10_t *)&(args[380])),
           args[390], args[391], args[392], args[393], args[394], args[395]);
case 397:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), *((dummy10_t *)&(args[380])),
           args[390], args[391], args[392], args[393], args[394], args[395],
           args[396]);
case 398:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy10_t *)&(args[300])),
           *((dummy10_t *)&(args[310])), *((dummy10_t *)&(args[320])),
           *((dummy10_t *)&(args[330])), *((dummy10_t *)&(args[340])),
           *((dummy10_t *)&(args[350])), *((dummy10_t *)&(args[360])),
           *((dummy10_t *)&(args[370])), *((dummy10_t *)&(args[380])),
           args[390], args[391], args[392], args[393], args[394], args[395],
           args[396], args[397]);
case 399:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])));
case 400:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])));
case 401:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           args[400]);
case 402:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           args[400], args[401]);
case 403:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           args[400], args[401], args[402]);
case 404:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           args[400], args[401], args[402], args[403]);
case 405:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           args[400], args[401], args[402], args[403], args[404]);
case 406:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           args[400], args[401], args[402], args[403], args[404], args[405]);
case 407:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           args[400], args[401], args[402], args[403], args[404], args[405],
           args[406]);
case 408:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           args[400], args[401], args[402], args[403], args[404], args[405],
           args[406], args[407]);
case 409:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])));
case 410:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])));
case 411:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), args[410]);
case 412:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), args[410], args[411]);
case 413:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), args[410], args[411], args[412]);
case 414:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), args[410], args[411], args[412],
           args[413]);
case 415:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), args[410], args[411], args[412],
           args[413], args[414]);
case 416:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), args[410], args[411], args[412],
           args[413], args[414], args[415]);
case 417:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), args[410], args[411], args[412],
           args[413], args[414], args[415], args[416]);
case 418:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), args[410], args[411], args[412],
           args[413], args[414], args[415], args[416], args[417]);
case 419:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])));
case 420:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])));
case 421:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           args[420]);
case 422:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           args[420], args[421]);
case 423:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           args[420], args[421], args[422]);
case 424:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           args[420], args[421], args[422], args[423]);
case 425:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           args[420], args[421], args[422], args[423], args[424]);
case 426:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           args[420], args[421], args[422], args[423], args[424], args[425]);
case 427:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           args[420], args[421], args[422], args[423], args[424], args[425],
           args[426]);
case 428:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           args[420], args[421], args[422], args[423], args[424], args[425],
           args[426], args[427]);
case 429:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])));
case 430:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])));
case 431:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), args[430]);
case 432:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), args[430], args[431]);
case 433:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), args[430], args[431], args[432]);
case 434:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), args[430], args[431], args[432],
           args[433]);
case 435:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), args[430], args[431], args[432],
           args[433], args[434]);
case 436:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), args[430], args[431], args[432],
           args[433], args[434], args[435]);
case 437:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), args[430], args[431], args[432],
           args[433], args[434], args[435], args[436]);
case 438:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), args[430], args[431], args[432],
           args[433], args[434], args[435], args[436], args[437]);
case 439:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])));
case 440:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])));
case 441:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           args[440]);
case 442:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           args[440], args[441]);
case 443:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           args[440], args[441], args[442]);
case 444:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           args[440], args[441], args[442], args[443]);
case 445:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           args[440], args[441], args[442], args[443], args[444]);
case 446:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           args[440], args[441], args[442], args[443], args[444], args[445]);
case 447:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           args[440], args[441], args[442], args[443], args[444], args[445],
           args[446]);
case 448:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           args[440], args[441], args[442], args[443], args[444], args[445],
           args[446], args[447]);
case 449:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])));
case 450:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])));
case 451:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), args[450]);
case 452:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), args[450], args[451]);
case 453:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), args[450], args[451], args[452]);
case 454:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), args[450], args[451], args[452],
           args[453]);
case 455:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), args[450], args[451], args[452],
           args[453], args[454]);
case 456:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), args[450], args[451], args[452],
           args[453], args[454], args[455]);
case 457:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), args[450], args[451], args[452],
           args[453], args[454], args[455], args[456]);
case 458:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), args[450], args[451], args[452],
           args[453], args[454], args[455], args[456], args[457]);
case 459:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])));
case 460:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])));
case 461:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           args[460]);
case 462:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           args[460], args[461]);
case 463:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           args[460], args[461], args[462]);
case 464:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           args[460], args[461], args[462], args[463]);
case 465:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           args[460], args[461], args[462], args[463], args[464]);
case 466:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           args[460], args[461], args[462], args[463], args[464], args[465]);
case 467:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           args[460], args[461], args[462], args[463], args[464], args[465],
           args[466]);
case 468:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           args[460], args[461], args[462], args[463], args[464], args[465],
           args[466], args[467]);
case 469:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])));
case 470:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])));
case 471:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), args[470]);
case 472:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), args[470], args[471]);
case 473:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), args[470], args[471], args[472]);
case 474:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), args[470], args[471], args[472],
           args[473]);
case 475:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), args[470], args[471], args[472],
           args[473], args[474]);
case 476:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), args[470], args[471], args[472],
           args[473], args[474], args[475]);
case 477:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), args[470], args[471], args[472],
           args[473], args[474], args[475], args[476]);
case 478:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), args[470], args[471], args[472],
           args[473], args[474], args[475], args[476], args[477]);
case 479:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])));
case 480:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])));
case 481:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           args[480]);
case 482:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           args[480], args[481]);
case 483:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           args[480], args[481], args[482]);
case 484:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           args[480], args[481], args[482], args[483]);
case 485:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           args[480], args[481], args[482], args[483], args[484]);
case 486:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           args[480], args[481], args[482], args[483], args[484], args[485]);
case 487:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           args[480], args[481], args[482], args[483], args[484], args[485],
           args[486]);
case 488:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           args[480], args[481], args[482], args[483], args[484], args[485],
           args[486], args[487]);
case 489:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           *((dummy10_t *)&(args[480])));
case 490:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           *((dummy10_t *)&(args[480])));
case 491:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           *((dummy10_t *)&(args[480])), args[490]);
case 492:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           *((dummy10_t *)&(args[480])), args[490], args[491]);
case 493:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           *((dummy10_t *)&(args[480])), args[490], args[491], args[492]);
case 494:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           *((dummy10_t *)&(args[480])), args[490], args[491], args[492],
           args[493]);
case 495:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           *((dummy10_t *)&(args[480])), args[490], args[491], args[492],
           args[493], args[494]);
case 496:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           *((dummy10_t *)&(args[480])), args[490], args[491], args[492],
           args[493], args[494], args[495]);
case 497:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           *((dummy10_t *)&(args[480])), args[490], args[491], args[492],
           args[493], args[494], args[495], args[496]);
case 498:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy10_t *)&(args[400])), *((dummy10_t *)&(args[410])),
           *((dummy10_t *)&(args[420])), *((dummy10_t *)&(args[430])),
           *((dummy10_t *)&(args[440])), *((dummy10_t *)&(args[450])),
           *((dummy10_t *)&(args[460])), *((dummy10_t *)&(args[470])),
           *((dummy10_t *)&(args[480])), args[490], args[491], args[492],
           args[493], args[494], args[495], args[496], args[497]);
case 499:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy100_t *)&(args[400])));
case 500:
return fct(level, *((dummy100_t *)&(args[0])), *((dummy100_t *)&(args[100])),
           *((dummy100_t *)&(args[200])), *((dummy100_t *)&(args[300])),
           *((dummy100_t *)&(args[400])));
