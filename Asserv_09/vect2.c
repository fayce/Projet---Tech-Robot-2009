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
#include "vect2.h"

#include <stdlib.h>
#include <math.h>

/* Convert a polar vector to a cartesian one */
void vect2_pol2cart(vect2_pol* vp, vect2_cart* vc)
{
   if(vp == NULL) return;
   if(vc == NULL) return;
   
   vc->x = (Real)( (vp->r)*cos(vp->theta) );
   vc->y = (Real)( (vp->r)*sin(vp->theta) );
   
   return;
}

/* Convert a cartesian vector to a polar one 
void vect2_cart2pol(vect2_cart* vc, vect2_pol* vp)
{
   if(vc == NULL) return;
   if(vp == NULL) return;
   
   vp->r = (Real)( sqrt((vc->x)*(vc->x)+(vc->y)*(vc->y)) );
   vp->theta = (Real)atan2(vc->x,vc->y);
   
   return;
}
*/
vect2_pol vect2_cart2pol(vect2_cart* vc){
vect2_pol vp;
 
 
   vp.r = (Real)( sqrt((vc->x)*(vc->x)+(vc->y)*(vc->y)) );
   vp.theta = (Real)atan2(vc->y,vc->x);
   
   return vp;
}


