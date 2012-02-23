/*  
 *  Copyright Projet & Tech' (2008-2009)
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
 *	Fayçal GUENNAR <f.guennar@gmail.com>
 */
 
#include "interrupt.h"

//Disable Coders Interrupt
void dci(void){
DigBuf_1_DisableInt();  
DigBuf_2_DisableInt();  
}

//Enable Coders Interrupt
void eci(void){
DigBuf_1_EnableInt();  
DigBuf_2_EnableInt();  
}


void dgi(void){
	//M8C_DisableGInt;
//	di();
	}
void egi(void){
//ei();
	//M8C_EnableGInt;
    //M8C_EnableIntMask(INT_MSK0,INT_MSK0_GPIO);  
	
}


void di2ci(void){
I2CHW_1_DisableInt();				// Enable I2C Interrupt
}

void ei2ci(void){
I2CHW_1_EnableInt();				// Disable I2C Interrupt
}