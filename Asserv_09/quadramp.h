/*  
 *  Copyright Droids Corporation, Microb Technology, Eirbot (2005)
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
 */
#ifndef _QUADRAMP_H_
#define _QUADRAMP_H_

#include <stdio.h>
#include "types.h"
#include "utiles.h"
#include "interrupt.h"

#define NEXT(n, i)  (((n) + (i)/(n)) >> 1)

struct quadramp_filter
{
    uint32_t var_2nd_ord_pos;
    uint32_t var_2nd_ord_neg;
    uint32_t var_1st_ord_pos;
    uint32_t var_1st_ord_neg;

    int32_t previous_var;
    int32_t previous_out;
};

/** Initialisation du filtre */
void quadramp_init(struct quadramp_filter * r);

void quadramp_set_2nd_order_vars(struct quadramp_filter * r, uint32_t var_2nd_ord_pos, uint32_t var_2nd_ord_neg);
void quadramp_set_1st_order_vars(struct quadramp_filter * r, uint32_t var_1st_ord_pos, uint32_t var_1st_ord_neg);

uint32_t quadramp_get_2nd_order_vars(struct quadramp_filter * r);
uint32_t quadramp_get_1st_order_vars(struct quadramp_filter * r);

int32_t quadramp_get_previous_out(struct quadramp_filter * r);

/**
 * Calcul la rampe 
 * data doit etre une  (struct quadramp_filter *) pointer
 */
int32_t quadramp_do_filter(void * data, int32_t in);

#endif
