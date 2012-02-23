/*  
 *  Copyright Droids Corporation, Microb Technology, Eirbot (2005), Projet & Tech' (2008-2009)
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Olivier MATZ <zer0@droids-corp.org> 
 *	Fayçal GUENNAR <f.guennar@gmail.com>
 */
 
#ifndef _UTILES_H_
#define _UTILES_H_

#include "glob_vars.h"
#include "types.h"

#define M_2PI (2*M_PI)

/**
 *  signed maxmimum : both signs are tested
 */
#define S_MAX(to_saturate, value_max)    \
do {                                     \
   if (to_saturate > value_max)          \
     to_saturate = value_max;            \
   else if (to_saturate < -value_max)    \
     to_saturate = -value_max;           \
 } while(0)

/**
 *  unsigned maximum : result >0 is forced
 */
#define U_MAX(to_saturate, value_max)    \
do {                                     \
   if (to_saturate > value_max)          \
     to_saturate = value_max;            \
   else if (to_saturate < 0)             \
     to_saturate = 0;                    \
 } while(0)


// ecrete par dessus (simple)
#define MAX(to_saturate, value_max)      \
do {                                     \
   if (to_saturate > value_max)          \
     to_saturate = value_max;            \
} while(0)


// ecrete par dessous (simple)
#define MIN(to_saturate, value_min)      \
do {                                     \
   if (to_saturate < value_min)          \
     to_saturate = value_min;            \
} while(0)


#define ABS( val) ( ((val) <0) ? -(val) : (val) )
#define DEG(x) ((x) * (180.0 / M_PI))
#define RAD(x) ((x) * (M_PI / 180.0))



double simple_modulo_2pi(double a);
double modulo_2pi(double a);
double modulo_2pi(double a);



#endif
