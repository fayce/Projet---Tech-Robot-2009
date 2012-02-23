/*  
 *  Copyright Projet & Tech' (2008-2009)
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
 *	Fayçal GUENNAR <f.guennar@gmail.com>
 */
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include <htc.h>
//Disable/Enable Coders Interrupt
void dci(void);
void eci(void);

//Disable/Enable global interrupt
void dgi(void);
void egi(void);

void di2ci(void);
void ei2ci(void);