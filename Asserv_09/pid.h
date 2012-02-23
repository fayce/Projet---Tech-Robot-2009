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
 
#ifndef _PID_H_
#define _PID_H_

#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "interrupt.h"
#include "utiles.h"

/** structure pid_filter */
struct pid_filter
{
//changement des types from int32_t à uint16_t 
    int32_t gain_P; /**< Gain du module Proportionnel */
    int32_t gain_I; /**< Gain du module integral */
    int32_t gain_D; /**< Gain du module de derivee */

    uint8_t out_shift;

    int32_t max_in; /**<  niveau de saturation en entree*/
    int32_t max_I; /**<   niveau de saturation en integrale */
    int32_t max_out; /**< niveau de saturation en sortie */

    int32_t integral; /**< integrale precedente */
    int32_t prev_in;  /**< prec entree */
    int32_t prev_D;   /**< prec derivee */
    int32_t prev_out; /**< prec command de sortie*/
};

/** Prototyping */

void pid_init (struct pid_filter * p);

/* Fonctions de configuration du filtre pid */
void pid_set_gains(struct pid_filter *p, int32_t gp, int32_t gi, int32_t gd) ;
void pid_set_maximums(struct pid_filter *p, int32_t max_in, int32_t max_I, int32_t max_out);
void pid_set_out_shift(struct pid_filter *p, uint8_t out_shift);

//accesseurs
int32_t pid_get_gain_P(struct pid_filter *p);
int32_t pid_get_gain_I(struct pid_filter *p);
int32_t pid_get_gain_D(struct pid_filter *p);

int32_t pid_get_max_in(struct pid_filter *p);
int32_t pid_get_max_I(struct pid_filter *p);
int32_t pid_get_max_out(struct pid_filter *p);

uint8_t pid_get_out_shift(struct pid_filter *p);

int32_t pid_get_value_I(struct pid_filter *p);
int32_t pid_get_value_in(struct pid_filter *p);
int32_t pid_get_value_D(struct pid_filter *p);
int32_t pid_get_value_out(struct pid_filter *p);


/** process of ze PID  */
int32_t pid_do_filter(void *p, int32_t in);
        
        
#endif
