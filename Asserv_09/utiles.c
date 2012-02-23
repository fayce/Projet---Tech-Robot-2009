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
 
#include "utiles.h"


/** do a modulo 2.pi -> [-Pi,+Pi], knowing that 'a' is in [-3Pi,+3Pi] */
double simple_modulo_2pi(double a) {
    if (a < - M_PI) {
        a += M_2PI;
        }
    else if (a > M_PI) {
        a -= M_2PI;
        }
    return a;
    }

/** do a modulo 2.pi -> [-Pi,+Pi] */
double modulo_2pi(double a) {
    double res = a - (((int32_t)(a / M_2PI)) * M_2PI);
    return simple_modulo_2pi(res);
    }
  

