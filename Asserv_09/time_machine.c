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
 
 #include "time_machine.h"
 
 void init_tm(void){
 	time_slice_cpt=0;
 	TIME_MACHINE_DO_CS=0;
	TIME_MACHINE_UART_CHECK=0;
	TIME_MACHINE_DO_STRAT=0;
	TIME_MACHINE_DO_POS=0;
	TIME_MACHINE_DO_TRAJ=0;
	TIME_MACHINE_DO_IR=0;
 }
 
 void reset_time(void){
 time_slice_cpt=0;
 }
 uint24_t gettime(void){
 return (uint24_t)(time_slice_cpt*TIMER_PERIOD);
 }
 
 int8_t wait8ms(uint16_t delay){
 	uint24_t start = time_slice_cpt;
	
 while(time_slice_cpt < start + delay);
 
 return 0;
 }