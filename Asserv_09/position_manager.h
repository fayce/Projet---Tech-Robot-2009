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
 
#ifndef _POSITION_MANAGER_H_
#define _POSITION_MANAGER_H_

#include <string.h>
#include <math.h>
#include "robot_system.h"
#include "types.h"
#include "glob_vars.h"
#include "interrupt.h"
/**
 * Structure qui contient les param physiques du robot.
 */
struct robot_physical_params 
{
    double track_cm;
    double distance_imp_per_cm;
};


/**
 * Pos. en (double)
 * ATTENTION : a en radians
 */
struct xya_position
{
    double x;
    double y;
    double a;
};

/**
 * Pos. en (int)
 * ATTENTION : a en degrÃ©s
 */
struct xya_position_s16
{
    int16_t x;
    int16_t y;
    int16_t a;
};

/**
 * Structure qui contient (x,y,a) en int et double.
 */
struct robot_position
{
    uint8_t use_ext;
    struct robot_physical_params phys;
    struct xya_position pos_d;
    struct xya_position_s16 pos_s16;
    struct rs_polar prev_encoders;
    struct robot_system * rs;
};


// init structure
void position_init(struct robot_position * pos);

//Definit une nouvelle position
void position_set(struct robot_position * pos, int16_t x, int16_t y, int16_t a);

void position_use_ext(struct robot_position * pos);


 /**
 * stocke dans la struc pos un pointeur vers le robot_system associÃ©.
 * La structure robot_system sert a recuperer les valeurs des codeurs
 */
void position_set_related_robot_system(struct robot_position * pos, struct robot_system * rs);


/**
 * Config des param physiques du robot :
 *  - nombre d'impulsions pour 1 cm (distance)
 *  - nombre d'impulsion pour 1 degrÃ© (angle)
 */
void position_set_physical_params(struct robot_position * pos, double track_cm,
                  double distance_imp_per_cm);


/**
 * Calcul la position absolue (x,y,a) en fonction du dernier delta
 * lu sur les codeurs, et biensur des param physiques
 * La position est donnÃ©e en cm.
 */
void position_manage(struct robot_position * pos);


int16_t position_get_x_s16(struct robot_position *pos);
int16_t position_get_y_s16(struct robot_position *pos);
int16_t position_get_a_deg_s16(struct robot_position *pos);

double position_get_x_double(struct robot_position *pos);
double position_get_y_double(struct robot_position *pos);
double position_get_a_rad_double(struct robot_position *pos);


#endif
