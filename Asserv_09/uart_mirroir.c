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
 
 
 #include "uart_mirroir.h"
 
 
uint8_t uart_mirr_checkcmd(uint8_t *cmd) {

	static uint8_t cmd_list[UART_MIRR_MAX_CMD][UART_MIRR_MAX_CMD_CHAR]={
	"pos",
	"constr"
	
	};
	//on compare les elements de cmd_list un à un avec arg0
	//int8_t * pcmd;
	uint8_t cpt;
	for(cpt=0;cpt<UART_MIRR_MAX_CMD;cpt++){
		if(strcmp(cmd_list[cpt],cmd)==0) 	
			return cpt;
	}

	return 99;

}

void uart_mirr_check(void){

	if(UART_MIRR_bCmdCheck()){
		uint8_t *strPtr; 
		
			if(strPtr = UART_MIRR_szGetParam())  {		
	
				if(*strPtr!=0){
					uint8_t cmd[10];
					strcpy(cmd,strPtr);
					//pour l'instant strPtr est la commande, on check si elle existe
					uint8_t cmd_id = uart_mirr_checkcmd(&cmd);
			
					if(cmd_id!=99)
					(*(uart_mirr_func_tab[cmd_id]))(strPtr);
				}	
        	}
    	UART_MIRR_CmdReset();
	}
}

void uart_setposa(uint8_t *str){
uint8_t cpt=0;

	while(str = UART_MIRR_szGetParam()){
		uart_mirr_s16_buff[cpt]=atoi(str);
		cpt++;
	}
	oa_set_opp(&beudaoa,uart_mirr_s16_buff[0],uart_mirr_s16_buff[1]);
	PRT2DR ^=LED_2;
}


void uart_setconstr(uint8_t *str){}