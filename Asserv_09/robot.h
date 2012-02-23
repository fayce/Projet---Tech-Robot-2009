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
 
#ifndef _ROBOT_H_
#define _ROBOT_H_

#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules


#include "interrupt.h"
#include "robot_system.h"
#include "control_system_manager.h"
#include "position_manager.h"
#include "pid.h"
#include "quadramp.h"
#include "pwm.h"
#include "trajectory_manager.h"

#define EVENT_DO_CS 0x01
#define EVENT_DO_RS 0x02
#define EVENT_DO_POS 0x03
#define EVENT_DO_BD 0x04
#define EVENT_DO_STRAT 0x10
#define EVENT_GO 0x20
#define EVENT_POSITIONED 0x040

/*alias and flags*/
#define COLOR_GREEN 1
#define COLOR_RED 2

struct robot {
    
	//int8_t cs_events;
  	struct cs cs_a; //contient consigne et filtres associes (angle
    struct cs cs_d; // [...] (distance)

   	struct robot_system rs; //contient top codeurs
   
	struct robot_position posr; //contient pos de notre robot en (x,y,a)
	
    struct quadramp_filter qr_a; //filtre generateur de rampe trapezoidale (angle)
    struct quadramp_filter qr_d; // [...] (distance)

    struct pid_filter pid_a; //pid angle
    struct pid_filter pid_d; //pid distance

	int8_t events; //8ev EVENT_DO_CS | EVENT_DO_RS ...
	
	int8_t color;

    };

struct robot beudabot;
#endif
