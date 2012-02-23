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
 *	FayÃ§al GUENNAR <f.guennar@gmail.com>
 */
 
#ifndef _ROBOT_SYSTEM_H_
#define _ROBOT_SYSTEM_H_

#include <m8c.h>        // part specific constants and macros
#include <string.h>
#include <stdio.h>

#include "angle_distance.h"
#include "types.h"
#include "glob_vars.h"
#include "interrupt.h"
#include "utiles.h"
#include "pwm.h"


struct robot_system
{
    struct rs_polar pext_prev;
    struct rs_wheels wext_prev;

    struct rs_polar virtual_pwm;
    struct rs_polar virtual_encoders;
	
	//uint16_t speed_a;
 	//uint16_t speed_d;

};


void rs_init( struct robot_system * );

int32_t rs_get_ext_angle(void * rs);
int32_t rs_get_ext_distance(void * rs);
int32_t rs_get_ext_left(void * rs);
int32_t rs_get_ext_right(void * rs);

void rs_update(void * rs);

int32_t rs_get_angle( struct robot_system * rs);
int32_t rs_get_distance(struct robot_system * rs);

void rs_set_angle(void * data, int32_t angle);
void rs_set_distance(void * data, int32_t distance);


#endif
