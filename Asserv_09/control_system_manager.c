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
 
#include "control_system_manager.h"


void cs_init(struct cs * cs) {
    //memset(cs, 0, sizeof(struct cs));

         cs->process_out = 0; //0 = ANGLE ou 1 = DISTANCE
		 cs->prev_process_out_value = 0;
		
         cs->consign_value = 0;
         cs->filtered_consign_value = 0;
         cs->error_value = 0;
        
		 cs->out_value = 0;
		
		 cs->speed = 0;
  //  cs->consign_filter_params = NULL;
  //  cs->correct_filter_params = NULL;
   
}

void cs_set_consign_filter(struct cs* cs, struct quadramp_filter* consign_filter_params){
    cs->consign_filter_params = consign_filter_params;

}



void cs_set_correct_filter(struct cs* cs, struct pid_filter* correct_filter_params){
    cs->correct_filter_params = correct_filter_params;
}

void  cs_do_process(struct cs * cs,  struct robot_system * rs, int32_t consign) {
 
    cs->consign_value = consign;
  
  //reglages sans rampe pour l'instant
   cs->filtered_consign_value = quadramp_do_filter(cs->consign_filter_params, consign);

    if (cs->process_out == PROCESS_DISTANCE){
		cs->speed = cs->prev_process_out_value - rs_get_distance(rs);
    	cs->prev_process_out_value = rs_get_distance(rs);
	}
    else{ //(PROCESS_ANGLE)
		cs->speed = cs->prev_process_out_value - rs_get_angle(rs);
      	cs->prev_process_out_value = rs_get_angle(rs);
		
    }
   
    cs->error_value = cs->filtered_consign_value - cs->prev_process_out_value;
    cs->out_value = pid_do_filter(cs->correct_filter_params, cs->error_value);
	
	
	if(cs->process_out == PROCESS_DISTANCE)
 		rs_set_distance(rs, cs->out_value);
	
 	else
 		rs_set_angle(rs, cs->out_value);
}

void cs_manage(void * _cs, void * _rs){

    struct cs * cs = _cs;
    struct robot_system    * rs = _rs;
    cs_do_process(cs,rs,cs->consign_value);
}

int32_t cs_get_out(struct cs * cs) {
    return cs->out_value;
 }

int32_t cs_get_error(struct cs * cs) {
    return cs->error_value;
}

int32_t cs_get_consign(struct cs * cs) {
    return cs->consign_value;
}

int32_t cs_get_filtered_consign(struct cs * cs) {
    return cs->filtered_consign_value;
}

void cs_set_consign(struct cs * cs, int32_t v) {
    cs->consign_value = v;
}


void cs_set_process_out(struct cs* cs, uint8_t _process_out){

   cs->process_out = _process_out;

}

int8_t cs_get_process_out(struct cs* cs){
return cs->process_out;
}

int16_t cs_get_speed( struct cs* cs){
return cs->speed;
}