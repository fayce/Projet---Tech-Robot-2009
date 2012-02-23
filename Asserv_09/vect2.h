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
 
#ifndef _VECT2_H_
#define _VECT2_H_

/** \brief Definition of reals used in vector 2d */
typedef double Real;

#define TO_RAD(x) (((double)x)*(M_PI/180.0))
#define TO_DEG(x) (((double)x)*(180.0/M_PI))

/** \brief Cartesian vector structure
**/
typedef struct vect2_cart
{
   Real x;
   Real y;
   
}vect2_cart;


/** \brief Polar vector structure
**/
typedef struct vect2_pol
{
   Real r;
   Real theta;
   
}vect2_pol;

/************************ Begin prototyping ************************/

/** \addtogroup Convertions
 * This functions allow to convert between the 2 vector types.
 * @{
 */

/** \brief Convert a polar vector to a cartesian one
* \param vp reference to target polar vector to convert from.
* \param vc reference to target cartesian vector where the function write.
* \warning This function doesn't do any malloc ! You have to allocate structures before calling this function.
*
**/
void vect2_pol2cart(vect2_pol* vp, vect2_cart* vc);


/** \brief Convert a cartesian vector to a polar one
* \param vc reference to target cartesian vector to convert from.
* \param vp reference to target polar vector where the function write the result.
* \warning This function doesn't do any malloc ! You have to allocate structures before calling this function.
*
**/
//void vect2_cart2pol(vect2_cart* vc, vect2_pol* vp);

vect2_pol vect2_cart2pol(vect2_cart* vc);




#endif /*_VECT2_H_*/
