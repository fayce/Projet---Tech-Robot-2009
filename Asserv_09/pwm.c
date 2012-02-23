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
 
#include "pwm.h"

void pwm_left(int16_t value){
static int8_t pwmg_prev_sens=RECULE;

	if(value<0){
 		if(pwmg_prev_sens!=RECULE){
			PWM16_1_Stop(); 
			clearBit(PRT1DR,PWM1_A);
 			clearBit(DCB03OU,0x01); //to P1_2
			setBit(DCB03OU,0x02);
			pwmg_prev_sens = RECULE;
		}
		
		value=-value;
	}
 
 	else{
 		if(pwmg_prev_sens!=AVANCE){
			PWM16_1_Stop(); 
 			clearBit(PRT1DR,PWM1_B);
 			setBit(DCB03OU,0x01); //to P1_1
			clearBit(DCB03OU,0x02);
			pwmg_prev_sens = AVANCE;
		}
	
	}

	if(value>PWM_MAX) value=PWM_MAX;  //ecretage

 	PWM16_1_WritePulseWidth(value);  
	PWM16_1_Start(); 
  
}

void pwm_right(int16_t value){
static int8_t pwmd_prev_sens=AVANCE;

	if(value<0){
 		if(pwmd_prev_sens!=RECULE){
 			PWM16_2_Stop(); 
			clearBit(PRT1DR,PWM2_A);
 			setBit(DBB11OU,0x01); //to P1_4
			setBit(DBB11OU,0x02);
			pwmd_prev_sens = RECULE;
		}
		value=-value;
	}
	
	else{
		if(pwmd_prev_sens!=AVANCE){
			PWM16_2_Stop(); 
			clearBit(PRT1DR,PWM2_B);
 			clearBit(DBB11OU,0x01); //to P1_3
			clearBit(DBB11OU,0x02);
			pwmd_prev_sens = AVANCE;
		}
	}
	
	if(value>PWM_MAX) value=PWM_MAX;  //ecretage
 	
	PWM16_2_WritePulseWidth(value);  
 	PWM16_2_Start(); 
}

void pwm_stop(void){
 PWM16_1_Stop(); 
 PWM16_2_Stop(); 
}