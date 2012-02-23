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
 
#ifndef _PWM_H_
#define _PWM_H_


#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "types.h"
#include "utiles.h"

#define AVANCE 1
#define RECULE 0

#define PWM_MAX 1023

void pwm_left(int16_t value);
void pwm_right(int16_t value);

//int8_t pwmg_prev_sens;
//int8_t pwmd_prev_sens;

void pwm_stop(void);
#endif