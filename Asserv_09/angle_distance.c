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

#include "angle_distance.h"

struct rs_polar rs_get_polar_from_wheels(struct rs_wheels w)
{
    struct rs_polar p;

    p.distance = (w.right + w.left)/2;
    p.angle    = w.right - w.left;

    return p;
}

/**
 * convert (distance, angle) into (left, right)
 */
struct rs_wheels rs_get_wheels_from_polar(struct rs_polar p)
{
    struct rs_wheels w;

    w.left  = (p.distance - p.angle) / 2;
    w.right = (p.distance + p.angle) / 2;

    return w;
}



