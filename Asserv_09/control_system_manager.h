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
 
#ifndef _CONTROL_SYSTEM_MANAGER_
#define _CONTROL_SYSTEM_MANAGER_

#include <stdio.h>
#include <string.h>
#include "types.h"
#include "utiles.h"
#include "robot_system.h"
//#include "position_manager.h"
#include "glob_vars.h"
#include "pid.h"
#include "quadramp.h"
#include "interrupt.h"


/** Structure du csm */
struct cs {

        struct quadramp_filter * consign_filter_params;
        struct pid_filter * correct_filter_params;
		
        uint8_t process_out; //0 = ANGLE ou 1 = DISTANCE
		int32_t prev_process_out_value;
		
        int32_t consign_value;
        int32_t filtered_consign_value;
        int32_t error_value;
        
		int32_t out_value;
		
		int16_t speed;
};

/******* - Prototyping - *******/

//Init en mettant tout à NULL/0
void cs_init(struct cs* cs);


// Set the cs consign_filter fields in the cs structure
void cs_set_consign_filter(struct cs* cs, struct quadramp_filter * consign_filter_params);

// Set the cs correct_filter fields in the cs structure
void  cs_set_correct_filter(struct cs* cs, struct pid_filter * correct_filter_params);



// Set the cs process_out fields in the cs structure
void cs_set_process_out(struct cs* cs, uint8_t process_out);
/*void cs_set_process_out(struct cs* cs, int32_t (*process_out)(void*),
void* process_out_params);

void cs_set_process_in(struct cs* cs, void (*process_in)(void*, int32_t),
void* process_in_params);
  */

/** Ce que fait cette fonction
 *
 * - Stock la consigne dans la structure.
 * - Applique le filtre de rampe sur cette consigne.
 * - Soustrait la consigne filtrée au process out.
 * - Stocke le resultat dans error_value et applique le PID
 * - Envoie de la consigne PIDifiée au process_in.
 * - retourne la consigne PIDifée.
 * 
 */

void cs_do_process(struct cs * cs,  struct robot_system * rs, int32_t consign);
/*
 * cs doit etre un pointeur struct cs*
 */
void cs_manage(void * _cs, void * _rs);


int32_t cs_get_out( struct cs* cs);
int32_t cs_get_error( struct cs* cs);
int32_t cs_get_consign( struct cs* cs);
int16_t cs_get_speed( struct cs* cs);
/** retourne la consigne filtrée par la rampe */
int32_t cs_get_filtered_consign( struct cs* cs);
int8_t cs_get_process_out(struct cs* cs);
void cs_set_consign( struct cs* cs, int32_t v);

#endif