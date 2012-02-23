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
 
#include "init.h"


void init_robot(struct robot *bot){

	rs_init(&bot->rs);
   
    cs_init(&bot->cs_a);
    cs_init(&bot->cs_d);

    quadramp_init(&bot->qr_a);
    quadramp_init(&bot->qr_d);

    pid_init(&bot->pid_a);
    pid_init(&bot->pid_d);

   position_init(&bot->posr); //nous
   
//	bot->EVENT_DO_CS =1;
	bot->events = EVENT_DO_CS | EVENT_DO_POS;
	
    quadramp_init(&bot->qr_a);
    quadramp_init(&bot->qr_d);
	
	#ifdef UART_VERBOSE
	UART_CPutString("\r\n init robot structures : [OK]");
	#endif
}



void init_encoders(void){
	//Digbufz pour coder1
	TopG=0;
	TopD=0;
	DigBuf_1_EnableInt();  
   	DigBuf_1_Start();  
	DigBuf_2_EnableInt();  
   	DigBuf_2_Start(); 

	
}




void init_uart(void){
	///UART////      

    UART_CmdReset(); 
    UART_IntCntl(UART_ENABLE_RX_INT);       
    UART_Start(UART_PARITY_NONE);            
    UART_PutChar(12);
	
	#ifdef UART_VERBOSE
	UART_CPutString("\r\n\r\n\r\n\r\n\r\n\r\n waking up......................................."); 
	#endif
}

void init_uart_mirr(void){
	///UART////      

    UART_MIRR_CmdReset(); 
    UART_MIRR_IntCntl(UART_ENABLE_RX_INT);       
    UART_MIRR_Start(UART_PARITY_NONE);            
}

void init_pwm(void){
	PWM16_1_DisableInt(); 
	PWM16_2_DisableInt();

  	clearBit(PRT1DR,PWM1_A);
 	clearBit(DCB03OU,0x01); //to P1_2
	setBit(DCB03OU,0x02);
	
	clearBit(PRT1DR,PWM1_B);
 	setBit(DCB03OU,0x01); //to P1_1
	clearBit(DCB03OU,0x02);
	
	clearBit(PRT1DR,PWM2_A);
 	setBit(DBB11OU,0x01); //to P1_4
	setBit(DBB11OU,0x02);

	clearBit(PRT1DR,PWM2_B);
 	clearBit(DBB11OU,0x01); //to P1_3
	clearBit(DBB11OU,0x02);
	
}

void init_posr(struct robot_position * _posr){
	struct robot_position * posr = _posr;
	position_init(posr);

}

void init_timer(void){
	Timer8_1_EnableInt();
	Timer8_1_Start();
}