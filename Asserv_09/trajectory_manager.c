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
 *	FayÃƒÂ§al GUENNAR <f.guennar@gmail.com>
 */

#include "trajectory_manager.h"


/////////////////////////Fonctions d'initialisation///////////////////////////////
/*Init struct*/
void trajectory_init(struct trajectory * traj) {

    memset(traj, 0, sizeof(struct trajectory));
    traj->state = READY;
    }

/** Association au *CS */
void trajectory_set_cs(struct trajectory * traj, struct cs * cs_d, struct cs * cs_a) {
    traj->csm_distance = cs_d;
    traj->csm_angle = cs_a;
}

/** Association au *RS et *POS */
void trajectory_set_robot_params(struct trajectory * traj, struct robot_system * rs, struct robot_position * pos) {
     traj->robot = rs;
     traj->position = pos;
}




/** proche de la cible en distance ? */
uint8_t is_robot_in_dist_window(struct trajectory * traj, double d_win) {
    double d = ABS(traj->target_pol.distance - rs_get_distance(traj->robot));
    d = d / traj->position->phys.distance_imp_per_cm;
    return(d < d_win);

}

/** proche de la cible en  x,y ? */
uint8_t is_robot_in_xy_window(struct trajectory * traj)
{

    double x1 = traj->target_cart.x;
    double y1 = traj->target_cart.y;
    double x2 = position_get_x_double(traj->position);
    double y2 = position_get_y_double(traj->position);


   //seven_putlong(sqrt ((x2-x1) * (x2-x1) + (y2-y1) * (y2-y1)));
    return ( sqrt ((x2-x1) * (x2-x1) + (y2-y1) * (y2-y1)) < traj->d_win );
}


/** proche de la cible en angle (rad) ? */
uint8_t is_robot_in_angle_window(struct trajectory * traj) {

    double a = ABS(traj->target_pol.angle-rs_get_angle(traj->robot));

    a = (2.0 * a)/ ((traj->position->phys.distance_imp_per_cm) *
         (traj->position->phys.track_cm));
    return ( a < traj->a_win_rad );

}

//////////////////////////Fin fonctions de verif d'ÃƒÂ©tat de trajectoire //////////

/////////////////////////Fonctions modificatrices de consigne////////////////////////

#define UPDATE_D 1
#define UPDATE_A 2
#define RESET_D  4
#define RESET_A  8

/**
* maj angle et/ou distance
* fonction appelÃƒÂ©e par d'autres fonctions
*   traj  : *traj
*   d_cm  : distance en cm
*   a_rad : angle en radian
*   flags : ÃƒÂ  MAJ (UPDATE_A, UPDATE_D)
*/
void __trajectory_goto_d_a_rel(struct trajectory * traj, double d_cm, double a_rad, uint8_t flags) {
    int32_t a_consign, d_consign;

    double posx = position_get_x_double(traj->position);
    double posy = position_get_y_double(traj->position);
    double posa = position_get_a_rad_double(traj->position);

    if (flags & UPDATE_A) {
        if (flags & RESET_A) {
            a_consign = 0;
            }
        else {
            a_consign = (int32_t)(a_rad * (traj->position->phys.distance_imp_per_cm) * (traj->position->phys.track_cm));
            traj->target_pol.angle = a_consign;
            }
        cs_set_consign(traj->csm_angle, a_consign + rs_get_angle(traj->robot));
        }
    if (flags & UPDATE_D) {
        if (flags & RESET_D) {
            d_consign = 0;
            }
        else {
            d_consign = (int32_t)((d_cm) * (traj->position->phys.distance_imp_per_cm));
            traj->target_pol.distance = d_consign;
            }
        cs_set_consign(traj->csm_distance, d_consign + rs_get_distance(traj->robot));
        }
    }

/** Trace tout droit (d en cm) */
void trajectory_d_rel(struct trajectory * traj, double d_cm) {
    traj->state = RUNNING_D;
    __trajectory_goto_d_a_rel(traj, d_cm, 0, UPDATE_D | UPDATE_A | RESET_A);
    }

/** maj seulement de la consigne de distance */
void trajectory_only_d_rel(struct trajectory * traj, double d_cm) {
    traj->state = RUNNING_D;
    __trajectory_goto_d_a_rel(traj, d_cm, 0, UPDATE_D);
    }

/** Tourne de 'a' rad relatifs*/
void trajectory_a_rel(struct trajectory * traj, double a_rad_rel) {
    traj->state = RUNNING_A;
    __trajectory_goto_d_a_rel(traj, 0, a_rad_rel , UPDATE_A | UPDATE_D | RESET_D);
    }

/** Tourne de 'a' degrees absolus*/
void trajectory_a_abs(struct trajectory * traj, double a_deg_abs) {
    double posa = position_get_a_rad_double(traj->position);
    double a;

    traj->state = RUNNING_A;
    a = RAD(a_deg_abs) - posa;
    a = modulo_2pi(a);
    __trajectory_goto_d_a_rel(traj, 0, a, UPDATE_A | UPDATE_D | RESET_D);
    }

/** maj seulement consigne d'angle */
void trajectory_only_a_rel(struct trajectory * traj, double a_deg) {
    traj->state = RUNNING_A;
    __trajectory_goto_d_a_rel(traj, 0, RAD(a_deg), UPDATE_A);
    }

/** tourne de 'a' degrÃƒÂ© */
void trajectory_d_a_rel(struct trajectory * traj, double d_cm, double a_deg) {
    traj->state = RUNNING_D;
    __trajectory_goto_d_a_rel(traj, d_cm, RAD(a_deg), UPDATE_A | UPDATE_D);
    }

//////////////// Fonctions haut niveau transformant une cible XYen consigne/////////

/** Tourne le robot jusqu'a ce que X,Y soit en face */

void trajectory_turnto_xy(struct trajectory * traj, double x_abs_cm, double y_abs_cm) {
    double posx = position_get_x_double(traj->position);
    double posy = position_get_y_double(traj->position);
    double posa = position_get_a_rad_double(traj->position);

    traj->state = RUNNING_A;
    //DEBUG(E_TRAJECTORY, "Goto Turn To xy %f %f", x_abs_cm, y_abs_cm);
  traj->target_pol.angle = modulo_2pi(atan2(y_abs_cm - posy, x_abs_cm - posx) - posa);
//seven_putlong(1000*traj->target.pol.angle);
    __trajectory_goto_d_a_rel(traj, 0, traj->target_pol.angle, UPDATE_A | UPDATE_D | RESET_D);
}


/** Tourne le robot jusqu'a ce que X,Y soit derriere */
void trajectory_turnto_xy_behind(struct trajectory * traj, double x_abs_cm, double y_abs_cm){
    double posx = position_get_x_double(traj->position);
    double posy = position_get_y_double(traj->position);
    double posa = position_get_a_rad_double(traj->position);

    traj->state = RUNNING_A;
    __trajectory_goto_d_a_rel(traj, 0, modulo_2pi(atan2(y_abs_cm - posy, x_abs_cm - posx) - posa + M_PI), UPDATE_A | UPDATE_D | RESET_D);
}

/* Goto XY*/
void trajectory_goto_xy_abs(struct trajectory * traj, double x, double y) {


    traj->target_cart.y = y;
    traj->target_cart.x = x;
    traj->state = RUNNING_XY_START;
   glob_traj_state = TRAJ_STATE_RUNNING;
   	
}


////////////////////////Fin fct XY -> Consigne DA///////////////////////////////////

//////////////////////////Fonctions d'arrret ///////////////////////////////////////
/** met les consignes ÃƒÂ  0 */
void trajectory_stop(struct trajectory * traj) {
     traj->state = READY;
    __trajectory_goto_d_a_rel(traj, 0, 0, UPDATE_A | UPDATE_D | RESET_D | RESET_A);
    }


void trajectory_hardstop(struct trajectory * traj) {
    struct quadramp_filter * q_d, * q_a;

    traj->state = READY;
    q_d = traj->csm_distance->consign_filter_params;
    q_a = traj->csm_angle->consign_filter_params;
    __trajectory_goto_d_a_rel(traj, 0, 0, UPDATE_A | UPDATE_D | RESET_D | RESET_A);

    q_d->previous_var = 0;
    q_d->previous_out = rs_get_ext_distance(traj->robot);
    q_a->previous_var = 0;
    q_a->previous_out = rs_get_ext_angle(traj->robot);
    }

///////////////////////////Fin fct d'arret//////////////////////////////////////////

/** Retourne 1 si les consignes d'angle et de distances sont egales
aux consignes filtrÃƒÂ©es par la rampe */
uint8_t trajectory_finished(struct trajectory * traj) {
    return cs_get_consign(traj->csm_angle) == cs_get_filtered_consign(traj->csm_angle) && cs_get_consign(traj->csm_distance) == cs_get_filtered_consign(traj->csm_distance);
    }

//////////////////// Gestionnaire d'evenements deplacement////////////////////////////////
void trajectory_manager_event(void * param) {

    struct trajectory * traj = param;

    double a = position_get_a_rad_double(traj->position); 
    double x = position_get_x_double(traj->position); 
   	double y = position_get_y_double(traj->position);
	
	static uint32_t starttime;
	
	static uint8_t time_speed_null;
 	   	//declaration des struct qui contiendront les coord de la cible en XY et en DA   */
    struct vect2_cart v2cart_pos;
    struct vect2_pol v2pol_target;
		
	v2cart_pos.x = traj->target_cart.x - x;
	v2cart_pos.y = traj->target_cart.y - y;
	
	v2pol_target = vect2_cart2pol(&v2cart_pos);
	//v2pol_target.theta = v2pol_target.theta-a;
	v2pol_target.theta = simple_modulo_2pi(v2pol_target.theta-a);
	
	
		switch (traj->state) {
		
		case RUNNING_XY_START:
		sprintf(uart_out_buffer,"\r\n traj > Going to do dist:%d angle:%d (vect x:%d y:%d) ",(int16_t)v2pol_target.r,(int16_t)DEG(v2pol_target.theta),(int16_t)v2cart_pos.x,(int16_t)v2cart_pos.y);
	UART_PutString(&uart_out_buffer);

			trajectory_a_rel(traj, v2pol_target.theta);
			traj->state = RUNNING_A;
		break;
	
		case RUNNING_A:
		
			if(abs(cs_get_speed(traj->csm_angle))<10 && (cs_get_consign(traj->csm_angle) == cs_get_filtered_consign(traj->csm_angle))) time_speed_null++;
			else time_speed_null=0;
				
			if(time_speed_null>1){
				traj->state= RUNNING_A_OK; //RUNNING_A -> RUNNING_A_OK
			
			time_speed_null=0;
			}
			
		break;
	
		case RUNNING_A_OK:
	
			trajectory_d_rel(traj, v2pol_target.r);
			//starttime = gettime();
			traj->state = RUNNING_D;
		break;
	
		case RUNNING_D:
		
			if(abs(cs_get_speed(traj->csm_distance))<10 && (cs_get_consign(traj->csm_distance) == cs_get_filtered_consign(traj->csm_distance))) time_speed_null++;
			else time_speed_null=0;
			
			if( time_speed_null>1){
				traj->state= READY; //RUNNING_D -> RUNNING_D_OK
				 glob_traj_state = TRAJ_STATE_READY;
   	
				time_speed_null=0;
			}
		
			
		break;
		
	

		} 
	
}


enum trajectory_state trajectory_get_state(struct trajectory * traj){
	return traj->state;
}