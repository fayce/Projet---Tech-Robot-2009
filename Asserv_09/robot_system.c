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
 
#include "robot_system.h"

void rs_init( struct robot_system * rs){
   memset(rs, 0, sizeof(struct robot_system));

}


int32_t rs_get_ext_angle(void * data){
    struct robot_system * rs = data;
    int32_t angle;
dgi();
    angle = rs->pext_prev.angle ;
egi();
    return angle;
}
int32_t rs_get_ext_distance(void * data){
    struct robot_system * rs = data;
    int32_t distance;
dgi();
    distance = rs->pext_prev.distance ;
egi();
    return distance;
}
int32_t rs_get_ext_left(void * data){
    struct robot_system * rs = data;
    int32_t left;
	dgi();
    left = rs->wext_prev.left ;
	egi();
    return left;
}
int32_t rs_get_ext_right(void * data){
    struct robot_system * rs = data;
    int32_t right;
	dgi();
    right = rs->wext_prev.right ;
	egi();
    return right;
}

void rs_update(void * data){
    struct robot_system * rs = data;
    struct rs_wheels wext;
    struct rs_polar pext;
	
	//int32_t delta_angle, delta_distance;

	
	dci();
	wext.right = TopD;
	wext.left = TopG;
	eci();

//dgi();
   	pext = rs_get_polar_from_wheels(wext);
    wext = rs_get_wheels_from_polar(pext);
//	egi();
	
	
   //	rs->speed_a = ABS(pext.angle -  rs->pext_prev.angle);
   // rs->speed_d = ABS(pext.distance - rs->pext_prev.distance);


 /*   rs->virtual_encoders.angle += delta_angle;
    rs->virtual_encoders.distance += delta_distance;
*/
    rs->pext_prev = pext;
    rs->wext_prev = wext;
	

}


int32_t rs_get_distance( struct robot_system * rs){
    return rs->pext_prev.distance;;
}

int32_t rs_get_angle( struct robot_system * rs){
    return rs->pext_prev.angle;
}


void rs_set_angle(void * data, int32_t angle){
    struct rs_polar p;
    struct rs_wheels w;
    struct robot_system * rs = data;

    p.distance = rs->virtual_pwm.distance ;
    rs->virtual_pwm.angle = angle;
   
//dgi();
    p.angle = angle;
    w = rs_get_wheels_from_polar(p);
// egi();

	pwm_left(w.left);
  	pwm_right(w.right);

}
void rs_set_distance(void * data, int32_t distance){
    struct robot_system * rs = data;
    struct rs_polar p2;
    struct rs_wheels w2;


    dgi();
    p2.angle = rs->virtual_pwm.angle ;
    rs->virtual_pwm.distance = distance;
    egi();

//dgi();
    p2.distance = distance;
    w2 = rs_get_wheels_from_polar(p2);
  // egi();

    pwm_left(w2.left);
  	pwm_right(w2.right);
}
