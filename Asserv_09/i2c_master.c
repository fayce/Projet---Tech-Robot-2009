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
 *	FayÃ§al GUENNAR <f.guennar@gmail.com>
 */
 
#include "i2c_master.h"
#include "PSoCAPI.h"
#include "glob_vars.h"

void init_i2c_master(void){
	I2CHW_1_EnableMstr();
	I2CHW_1_EnableInt();
}


int8_t i2c_send_and_receive(int8_t slave_addr, int8_t Cmd){
int8_t i2c_answer=0;
uint8_t timeout=0;

	I2CHW_1_bWriteBytes(slave_addr,&Cmd,1,I2CHW_1_CompleteXfer);		

			while (!(I2CHW_1_bReadI2CStatus() & I2CHW_WR_COMPLETE) && timeout<I2C_TIMEOUT){
			timeout++;
				if(timeout>I2C_TIMEOUT){
					UART_CPutString("i2c> write timed out!\r\n");
					return 99;
				}
			}

			timeout=0;
			I2CHW_1_ClrWrStatus(); 
			I2CHW_1_fReadBytes(slave_addr,&i2c_answer,1,I2CHW_1_CompleteXfer );
			
			while (!(I2CHW_1_bReadI2CStatus() & I2CHW_RD_COMPLETE) && timeout<I2C_TIMEOUT){
			timeout++;
			}
				if(timeout>I2C_TIMEOUT){
					UART_CPutString("i2c> read timed out!\r\n");
					return 99;
				}
			
			I2CHW_1_ClrRdStatus();


	return i2c_answer;				
}

void i2c_send_only(int8_t slave_addr,int8_t Cmd){
uint8_t timeout=0;
	I2CHW_1_bWriteBytes(slave_addr,&Cmd,1,I2CHW_1_CompleteXfer);		

			while (!(I2CHW_1_bReadI2CStatus() & I2CHW_WR_COMPLETE) && timeout<I2C_TIMEOUT){
			timeout++;
			}	
			if(timeout == I2C_TIMEOUT)
				UART_CPutString("i2c> operation timed out :( !\r\n");
			I2CHW_1_ClrWrStatus(); 
				
}

