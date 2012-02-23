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
 
#ifndef TRAJECTORY_MANAGER
#define TRAJECTORY_MANAGER

#include "PSoCAPI.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "vect2.h"
#include "utiles.h"
#include "types.h"
#include "interrupt.h"
#include "robot.h"
#include "glob_vars.h"
#include "time_machine.h"

#define M_2PI (2*M_PI)

#define DEG(x) ((x) * (180.0 / M_PI))
#define RAD(x) ((x) * (M_PI / 180.0))


enum trajectory_state {

    RUNNING_XY_START,
    RUNNING_A,
    RUNNING_A_OK,
    RUNNING_D,
    READY,
	NEW_TRAJ,
	INTERRUPTED,

};


struct trajectory {
    enum trajectory_state state; /*<< decrit le type de cible, et si elle a été atteinte */

    struct vect2_cart target_cart;     /**<< cible, si c'est un vecteur x,y */
    struct rs_polar target_pol; /**<< cibble, si c'est un vecteur d,a */

    double d_win;      /**<< distance window (for END_NEAR) */
    double a_win_rad;  /**<< angle window (for END_NEAR) */
    double a_start_rad;/**<< in xy consigns, commence à bouger lorsque a_target < a_start */

    uint16_t d_speed;  /**<< consigne vitess distance */
    uint16_t a_speed;  /**<< consigne vitesse angle*/

    struct robot_position * position; /**<< *POS associé */
    struct robot_system * robot;      /**<< *RS associé */
    struct cs * csm_angle;    /**<< *CS_a associé */
    struct cs * csm_distance; /**<< *CS_d associé */
	
	uint8_t Curr_Action;

};

struct trajectory beudatraj;


 //extra func
 void __trajectory_goto_d_a_rel(struct trajectory * traj, double d_cm, double a_rad, uint8_t flags);
//Fct init struc Trajectory
void trajectory_init(struct trajectory * traj);
void trajectory_set_cs(struct trajectory * traj, struct cs * cs_d,
               struct cs * cs_a);

void trajectory_set_robot_params(struct trajectory * traj,
                 struct robot_system * rs,
                 struct robot_position * pos) ;

void trajectory_set_speed(struct trajectory * traj, int16_t d_speed, int16_t a_speed);


void trajectory_set_windows(struct trajectory * traj, double d_win,
                double a_win_deg, double a_start_deg);

//Fct verif etat traj
uint8_t trajectory_finished(struct trajectory* traj);
uint8_t trajectory_in_window(struct trajectory* traj, double d_win, double a_win_rad);

//Fct stop
void trajectory_stop(struct trajectory * traj);
void trajectory_hardstop(struct trajectory * traj);

//Fct app consign
void trajectory_d_rel(struct trajectory * traj, double d_cm);
void trajectory_only_d_rel(struct trajectory * traj, double d_cm);
void trajectory_a_rel(struct trajectory * traj, double a_deg);
void trajectory_a_abs(struct trajectory * traj, double a_deg);
void trajectory_only_a_rel(struct trajectory * traj, double a_deg);
void trajectory_d_a_rel(struct trajectory * traj, double d_cm, double a_deg);
void trajectory_goto_d_a_rel(struct trajectory* traj, double d, double a);

//Fct X,Y -> D,A  consign
//void trajectory_turnto_xy(struct trajectory* traj, double x_abs_cm, double y_abs_cm);
void trajectory_turnto_xy_behind(struct trajectory* traj, double x_abs_cm, double y_abs_cm);
void trajectory_turnto_xy(struct trajectory * traj, double x_abs_cm, double y_abs_cm) ;
void trajectory_goto_xy_abs(struct trajectory * traj, double x_abs_cm, double y_abs_cm);
void trajectory_goto_forward_xy_abs(struct trajectory * traj, double x_abs_cm, double y_abs_cm);
void trajectory_goto_xy_rel(struct trajectory* traj, double x_rel_cm, double y_rel_cm);

void trajectory_manager_event(void * param);

uint8_t is_robot_in_dist_window(struct trajectory * traj, double d_win);

/** proche de la cible en  x,y ? */
uint8_t is_robot_in_xy_window(struct trajectory * traj) ;

/** proche de la cible en angle (rad) ? */
uint8_t is_robot_in_angle_window(struct trajectory * traj);
enum trajectory_state trajectory_get_state(struct trajectory * traj);
#endif
