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
 
#include "glob_vars.h"
#include "types.h"
#include "trajectory_manager.h"

//Glob vars
int32_t TopG,TopD; //top codeurs

uint8_t bError; //flash r/w operation error state

uint8_t uart_out_buffer[32];
int8_t GlobalFlags;
uint8_t glob_traj_state;

uint8_t Strat_Action;
uint8_t Strat_State;

void init_globals(void){
	TopG=0;
	TopD=0;
	GlobalFlags = 0;
	glob_traj_state=0;
	Strat_State=0;
	Strat_Action=0;
}

