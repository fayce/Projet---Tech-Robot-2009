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
 
#include "flash.h"

/*
void flash_load_pid_cfg(struct pid_filter *_pida,struct pid_filter *_pidd){
struct pid_filter * pida = _pida;
struct pid_filter * pidd = _pidd;


int16_t loaded_pidcfg[8];  // int16_t = 2 bytes
uint8_t ret[64];	
 
 
 pid_set_maximums(pida, 0, 10000, 0);
 pid_set_maximums(pidd, 0, 10000, 0);
 
      E2PROM_1_E2Read( 0, loaded_pidcfg, 16 );  
			
	if(loaded_pidcfg[0]!=12336){  //default value when flash is Like a Virgiin !
		//on charge la config pid de l'eeprom
		#ifdef UART_VERBOSE
UART_CPutString("\r\n e2prom > Flash PID not empty! loading from flash");
#endif
		pid_set_gains(pida, loaded_pidcfg[0], loaded_pidcfg[1], loaded_pidcfg[2]);
		pid_set_out_shift(pida, (uint8_t)loaded_pidcfg[3]);

		pid_set_gains(pidd, loaded_pidcfg[4], loaded_pidcfg[5], loaded_pidcfg[6]);
		pid_set_out_shift(pidd, (uint8_t)loaded_pidcfg[7]);
		
		
		sprintf(ret,"\r\n e2prom > loaded values %d %d %d %d %d %d %d %d ",loaded_pidcfg[0],loaded_pidcfg[1],loaded_pidcfg[2],loaded_pidcfg[3],loaded_pidcfg[4],loaded_pidcfg[5],loaded_pidcfg[6],loaded_pidcfg[7]);
	
		
	}
	  
  	else{
	#ifdef UART_VERBOSE
	UART_CPutString("\r\n e2prom > Ow! flash is Like a Virgin! loading default values...");
	#endif
		pid_set_gains(pida, PIDA_P, PIDA_I, PIDA_D);
		pid_set_out_shift(pida, PIDA_SH);

		pid_set_gains(pidd, PIDD_P, PIDD_I, PIDD_D);
		pid_set_out_shift(pidd, PIDD_SH);
	
		sprintf(ret,"\r\n e2prom > loaded values values A: %d %d %d %d D: %d %d %d %d ",PIDA_P,PIDA_I,PIDA_D,PIDA_SH,PIDD_P,PIDD_I,PIDD_D,PIDD_SH);
		
		flash_save_pid_cfg(pida,pidd); //we save to the flash
	}
	#ifdef UART_VERBOSE
	UART_PutString(&ret);
	#endif
}

void flash_save_pid_cfg(struct pid_filter *_pida,struct pid_filter *_pidd){
	struct pid_filter * pida = _pida;
	struct pid_filter * pidd = _pidd;
	
	int16_t tosave_pidcfg[8];  // int16_t = 2 bytes
	
	tosave_pidcfg[0]=pid_get_gain_P(pida); 
	tosave_pidcfg[1]=pid_get_gain_I(pida);
	tosave_pidcfg[2]=pid_get_gain_D(pida);
	tosave_pidcfg[3]=pid_get_out_shift(pida);

	tosave_pidcfg[4]=pid_get_gain_P(pidd); 
	tosave_pidcfg[5]=pid_get_gain_I(pidd);
	tosave_pidcfg[6]=pid_get_gain_D(pidd);
	tosave_pidcfg[7]=pid_get_out_shift(pidd);
	

      /* On suppose ici que la température est de 25C //  
      bError = E2PROM_1_bE2Write(0,tosave_pidcfg, E2PROM_1_MAX_BYTES, 25); 
	  
	  	#ifdef UART_VERBOSE	
		uint8_t ret[32];
		sprintf(ret,"\r\n e2prom > trying to save %d %d %d %d %d %d %d %d ",tosave_pidcfg[0],tosave_pidcfg[1],tosave_pidcfg[2],tosave_pidcfg[3],tosave_pidcfg[4],tosave_pidcfg[5],tosave_pidcfg[6],tosave_pidcfg[7]);
		
		UART_PutString(&ret);
		#endif
		
	  if ( bError == E2PROM_1_NOERROR ){  
	  #ifdef UART_VERBOSE
      	UART_CPutString("\r\n e2prom > saving done...");
		#endif

		}
	  else{
	  #ifdef UART_VERBOSE
	   	UART_CPutString("\r\n e2prom > ERROR: can't save pid cfg to flash :( ");
		#endif
		}
	



}
*/