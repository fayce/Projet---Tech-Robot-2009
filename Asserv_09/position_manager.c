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
 
#include "position_manager.h"


// init structure
void position_init(struct robot_position * pos)
{
    memset(pos, 0, sizeof(struct robot_position));

}


//Definit une nouvelle position
void position_set(struct robot_position * pos, int16_t x, int16_t y, int16_t a)
{
    
    pos->pos_d.a = ((double)a * M_PI)/ 180.0;
    pos->pos_d.x = x;
    pos->pos_d.y = y;
    pos->pos_s16.x = x;
    pos->pos_s16.y = y;
    pos->pos_s16.a = a;
  
}


/**
 * stocke dans la struc pos un pointeur vers le robot_system associÃ©.
 * La structure robot_system sert a recuperer les valeurs des codeurs
 */
void position_set_related_robot_system(struct robot_position * pos, struct robot_system * rs)
{
  
    pos->rs = rs;
  
}

/**
 * Config des param physiques du robot :
 *  - nombre d'impulsions pour 1 cm (distance)
 *  - nombre d'impulsion pour 1 degrÃ© (angle)
 */
void position_set_physical_params(struct robot_position * pos, double track_cm,
                  double distance_imp_per_cm)
{
    
    pos->phys.track_cm = track_cm;
    pos->phys.distance_imp_per_cm = distance_imp_per_cm;
    
}

/**
 * Calcul la position absolue (x,y,a) en fonction du dernier delta
 * lu sur les codeurs, et biensur des param physiques
 * La position est donnÃ©e en cm.
 */
void position_manage(struct robot_position * pos)
{
    double x, y, a;
    int16_t x_s16, y_s16, a_s16;
    struct rs_polar _encoders;
    struct rs_polar delta;
    struct robot_system * rs;

  
    rs = pos->rs;
   
        _encoders.distance = rs_get_ext_distance(rs);
        _encoders.angle = rs_get_ext_angle(rs);

  
    delta.distance = _encoders.distance - pos->prev_encoders.distance;
    delta.angle = _encoders.angle - pos->prev_encoders.angle;
  
	 
    pos->prev_encoders = _encoders;

    /* maj de la pos en (double) */

    a = pos->pos_d.a;
    x = pos->pos_d.x;
    y = pos->pos_d.y;



    a += (double) delta.angle / (pos->phys.track_cm * pos->phys.distance_imp_per_cm);

    if(a < -M_PI)
        a += (M_PI*2);
    else if (a > (M_PI))
        a -= (M_PI*2);

    x += cos(a) * ((double) delta.distance / (pos->phys.distance_imp_per_cm)) ;
    y += sin(a) * ((double) delta.distance / (pos->phys.distance_imp_per_cm)) ;

    /* maj de la pos en (int) */
    x_s16 = (int16_t)x;
    y_s16 = (int16_t)y;
    a_s16 = (int16_t)(a * (180.0/(M_PI)));


   //seven_putlong(a_s16);



    pos->pos_d.a = a;
    pos->pos_d.x = x;
    pos->pos_d.y = y;
    pos->pos_s16.x = x_s16;
    pos->pos_s16.y = y_s16;
    pos->pos_s16.a = a_s16;
}

/************ int */

int16_t position_get_x_s16(struct robot_position *pos){
    return pos->pos_s16.x;
}

int16_t position_get_y_s16(struct robot_position *pos){
    return pos->pos_s16.y;
}

int16_t position_get_a_deg_s16(struct robot_position *pos){
    return pos->pos_s16.a;
}

/********* double */



double position_get_x_double(struct robot_position *pos){
    return pos->pos_d.x;
}

double position_get_y_double(struct robot_position *pos){
    return pos->pos_d.y;
}

double position_get_a_rad_double(struct robot_position *pos){
    return pos->pos_d.a;
}

