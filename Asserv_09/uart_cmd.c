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
#include "uart_cmd.h"



void uart_welcome(void){
#ifdef NO_MEM_LEFT
		UART_CPutString("\r\n\r\n\r\n\r\n\r\n\r\n"); 
		UART_CPutString("888888b.                          888          888888b.            888\r\n");    
		UART_CPutString("888  \"88b                         888          888  \"88b           888\r\n");    
		UART_CPutString("888  .88P                         888          888  .88P           888\r\n");    
		UART_CPutString("8888888K.   .d88b.  888  888  .d88888  8888b.  8888888K.   .d88b.  888888\r\n"); 
		UART_CPutString("888  \"Y88b d8P  Y8b 888  888 d88\" 888     \"88b 888  \"Y88b d88\"\"88b 888\r\n");    
		UART_CPutString("888    888 88888888 888  888 888  888 .d888888 888    888 888  888 888\r\n");    
		UART_CPutString("888   d88P Y8b.     Y88b 888 Y88b 888 888  888 888   d88P Y88..88P Y88b.\r\n");  
		UART_CPutString("8888888P\"   \"Y8888   \"Y88888  \"Y88888 \"Y888888 8888888P\"   \"Y88P\"   \"Y888\r\n"); 
#endif
		UART_CPutString("The Ultimate Bud-a-Bot!      P&T 2009 - Anouk, Fayce, Juju & Pierre\r\n");
		
		//uart_disp_rand_quote();
		UART_CPutString("Bite My Shiny Metal ASSSS!!!\r\n");

}
void uart_prompt(void){
	UART_CPutString("\r\nfayce@beudabot# ");
}


uint8_t uart_checkcmd(uint8_t *cmd) {

	static uint8_t cmd_list[UART_MAX_CMD][UART_MAX_CMD_CHAR]={
/*	#ifdef UART_CONF_ROBOT
	"setpid",
	"getpid",
	"dotest",
	#endif
*/
	"getpos",
	"target",
	"opp", //opponent position
	"s", //sig -> reset/start/stop/come back...
	"strat",
	"debug",
/*	#ifdef UART_CONF_ROBOT
	"setspeed",
	"getspeed",
	"pwm",
	#endif
*/
	"goxy",
	"gotoda",
	"i2c"
	
	
	
	};
	uint8_t cpt;
	
	for(cpt=0;cpt<UART_MAX_CMD;cpt++){
		if(strcmp(cmd_list[cpt],cmd)==0) 	
			return cpt;
	}

	return 99;

}
void uart_check(void){

	if(UART_bCmdCheck()){
		uint8_t *strPtr; 
		
			if(strPtr = UART_szGetParam())  {		
	
				if(*strPtr!=0){
					uint8_t cmd[10];
					strcpy(cmd,strPtr);
					//pour l'instant strPtr est la commande, on check si elle existe
					uint8_t cmd_id = uart_checkcmd(&cmd);
			
					if(cmd_id!=99){
					(*(uart_func_tab[cmd_id]))(strPtr);
					}
					else
						UART_CPutString("\r\nGni??!");
						
				}
        	}
			
    	UART_CmdReset();
   		uart_prompt();
	}
}

void uart_setpid(uint8_t *str){
uint8_t arg0,cpt=0;

str = UART_szGetParam();
strcpy(&arg0,str);
	
	while(str = UART_szGetParam()){
		uart_u16_buff[cpt]=atoi(str);
		cpt++;
	}
	
	if(arg0=='a')
	pid_set_gains(&beudabot.pid_a, uart_u16_buff[0], uart_u16_buff[1], uart_u16_buff[2]);
	else
	pid_set_gains(&beudabot.pid_d, uart_u16_buff[0], uart_u16_buff[1], uart_u16_buff[2]);
	
	sprintf(ret,"\r\nsp %c %d %d %d",arg0,uart_u16_buff[0],uart_u16_buff[1],uart_u16_buff[2]);
	UART_PutString(&ret);
}

void uart_getpid(uint8_t *str){
uint8_t arg0;

str = UART_szGetParam();
strcpy(&arg0,str);

	if(arg0=='a'){
		uart_u16_buff[0]=pid_get_gain_P(&beudabot.pid_a);
		uart_u16_buff[1]=pid_get_gain_I(&beudabot.pid_a);
		uart_u16_buff[2]=pid_get_gain_D(&beudabot.pid_a);
	}
	else{
		uart_u16_buff[0]=pid_get_gain_P(&beudabot.pid_d);
		uart_u16_buff[1]=pid_get_gain_I(&beudabot.pid_d);
		uart_u16_buff[2]=pid_get_gain_D(&beudabot.pid_d);
	}

	sprintf(ret,"\r\ngp %c %d %d %d",arg0,uart_u16_buff[0],uart_u16_buff[1],uart_u16_buff[2]);
	UART_PutString(&ret);
}

void uart_dotest(uint8_t *str){
uint8_t arg0,cpt=0;

str = UART_szGetParam();
strcpy(&arg0,str);

	while(str = UART_szGetParam()){
		uart_s16_buff[0]=atoi(str);
	}
	
	if(arg0=='a'){
	cs_set_consign(&beudabot.cs_a, uart_s16_buff[0]+rs_get_angle(&beudabot.rs));
	//pidd_debug = 0;
	//pida_debug = 1;
	}
	else{
	cs_set_consign(&beudabot.cs_d, uart_s16_buff[0]+rs_get_distance(&beudabot.rs));
	//pida_debug = 0;
	//pidd_debug = 1;
	}
	
	
	//int8_t ret[UART_BUFFER_SIZE];
	sprintf(ret,"\r\ndo %c %d ",arg0,uart_s16_buff[0]);
	UART_PutString(&ret);
	
}

void uart_getstate(uint8_t *str){}

void uart_getpos(uint8_t *str){	
uint8_t arg0;

str = UART_szGetParam();
strcpy(&arg0,str);

	if(arg0=='r'){
	
	
		uart_s16_buff[0] = position_get_x_s16(&beudabot.posr);
		uart_s16_buff[1] = position_get_y_s16(&beudabot.posr);
		uart_s16_buff[2] = position_get_a_deg_s16(&beudabot.posr);
	}
	
	else{
		uart_s16_buff[0] = beudaoa.opponent_xy.x;
		uart_s16_buff[1] = beudaoa.opponent_xy.y;
		uart_s16_buff[2] = 0;
		
	}

	sprintf(ret,"\r\npos %c x:%d y:%d a:%d",arg0,uart_s16_buff[0],uart_s16_buff[1],uart_s16_buff[2]);
	UART_PutString(&ret);
}

void uart_signal(uint8_t *str){
	uint8_t args[2];
	str = UART_szGetParam();
	strcpy(&args[0],str);	
	str = UART_szGetParam();
	strcpy(&args[1],str);	
	
	if(args[0]=='r'){ //it's an emergency reset
		//trajectory_d_a_rel(&beudatraj,0,0);
		//wait8ms(50);
		clearBit(beudabot.events,EVENT_DO_CS);
		pwm_left(0);
		pwm_right(0);
		UART_CPutString("Woops...\n\r");
	}
	else if(args[0]=='c'){ //start, set color also
		//we start handling jack here
		if(PRT1DR & 0x01){
		setBit(GlobalFlags,FLAG_HANDLE_JACK); 
		}
		else{
		UART_CPutString("Hey dude! where's my Jack ?\n\r");
		setBit(GlobalFlags,FLAG_FAYCE_FORGOT_TO_PLUG_JACK); 
		}
		if(args[1]=='g' ||args[1]=='v'){
			
			beudabot.color= COLOR_GREEN; //color 1 green: 2 red
			position_set(&beudabot.posr, POS_G_INIT_X, POS_G_INIT_Y, POS_G_INIT_A);
			i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_SET_COLOR_GREEN);
			UART_CPutString("Green.. Hope! \n\r");
			
		}
		else if(args[1]=='r'){
			
			beudabot.color= COLOR_RED; //color 1 green: 2 red	
			position_set(&beudabot.posr, POS_R_INIT_X, POS_R_INIT_Y, POS_R_INIT_A);
			i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_SET_COLOR_RED);
			UART_CPutString("Red.. Love! \n\r");
		}
		else{
			UART_CPutString("syn err\n\r");
		}
	}	
	else if(args[0]=='s'){
	 	if(beudabot.color!=0){
			setBit(beudabot.events,EVENT_DO_STRAT);
		}
		else{	
			UART_CPutString("My color ? \n\r");
		}
	}
	else
		UART_CPutString("syn err\n\r");
}

void uart_pwm(uint8_t *str){
uint8_t cpt=0;
	while(str = UART_szGetParam()){
		uart_s16_buff[cpt]=atoi(str);
		cpt++;
	}
	
	pwm_left(uart_s16_buff[0]);
	pwm_right(uart_s16_buff[1]);
	
	sprintf(ret,"\r\npwm %d %d",uart_s16_buff[0],uart_s16_buff[1]);
	UART_PutString(&ret);
}

void uart_strat(uint8_t *str){
/*strat [c/l/e] [1..?]*/
 	str = UART_szGetParam();
	uart_s16_buff[0]=atoi(str);

	
	Strat_State = uart_s16_buff[0];
	UART_CPutString("strat> set\r\n");	
}

void uart_setpidshift(uint8_t *str){
uint8_t cpt=0;

	while(str = UART_szGetParam()){
		uart_u16_buff[cpt]=atoi(str);
		cpt++;
	}
	
	pid_set_out_shift(&beudabot.pid_a, uart_u16_buff[0]);
	pid_set_out_shift(&beudabot.pid_d, uart_u16_buff[1]);

	sprintf(ret,"\r\nspsh  %d %d",uart_u16_buff[0],uart_u16_buff[1]);
	UART_PutString(&ret);
}

void uart_getpidshift(uint8_t *str){
uint8_t res[2];

	res[0]=pid_get_out_shift(&beudabot.pid_a);
	res[1]=pid_get_out_shift(&beudabot.pid_d);

	sprintf(ret,"\r\ngpsh a:%d  d:%d",res[0],res[1]);
	UART_PutString(&ret);
}

void uart_debug_robot(uint8_t *str){
int16_t res[8];

	res[0]=rs_get_ext_left(&beudabot.rs);
	res[1]=rs_get_ext_right(&beudabot.rs);

   	res[2]=rs_get_distance(&beudabot.rs);
	res[3]=rs_get_angle(&beudabot.rs);
	
	res[4]=cs_get_speed(&beudabot.cs_a);
	res[5]=cs_get_speed(&beudabot.cs_d);
	//res[4]=0;
	//res[5]=0;
	
	
	res[6]=cs_get_error(&beudabot.cs_a);
	res[7]=cs_get_error(&beudabot.cs_d);
	
	sprintf(ret,"\r\nDEBUG  tops> l:%d r:%d |d:%d a:%d | speed> a:%d d:%d | pid-err> a:%d  d:%d  ",res[0],res[1],res[2],res[3],res[4],res[5],res[6],res[7]);
	UART_PutString(&ret);

   
}

void uart_setpidmax(uint8_t *str){
 uint8_t arg0,cpt=0;

str = UART_szGetParam();
strcpy(&arg0,str);

	while(str = UART_szGetParam()){
		uart_u16_buff[cpt]=atoi(str);
		cpt++;
	}
	
	if(arg0=='a')
	pid_set_maximums(&beudabot.pid_a, uart_u16_buff[0], uart_u16_buff[1], uart_u16_buff[2]);
	else
	pid_set_maximums(&beudabot.pid_d, uart_u16_buff[0], uart_u16_buff[1], uart_u16_buff[2]);
	
	sprintf(ret,"\r\nspm %c %d %d %d",arg0,uart_u16_buff[0],uart_u16_buff[1],uart_u16_buff[2]);
	UART_PutString(&ret);
}

void uart_getpidmax(uint8_t *str){
uint8_t arg0;

str = UART_szGetParam();
strcpy(&arg0,str);

	if(arg0=='a'){
		uart_u16_buff[0]=pid_get_max_in(&beudabot.pid_a);
		uart_u16_buff[1]=pid_get_max_I(&beudabot.pid_a);
		uart_u16_buff[2]=pid_get_max_out(&beudabot.pid_a);
	}
	else{
		uart_u16_buff[0]=pid_get_max_in(&beudabot.pid_d);
		uart_u16_buff[1]=pid_get_max_I(&beudabot.pid_d);
		uart_u16_buff[2]=pid_get_max_out(&beudabot.pid_d);
	}

	sprintf(ret,"\r\ngpm %c %d %d %d",arg0,uart_u16_buff[0],uart_u16_buff[1],uart_u16_buff[2]);
	UART_PutString(&ret);
	
}

void uart_setspeed(uint8_t *str){
uint8_t arg0,cpt=0;

str = UART_szGetParam();
strcpy(&arg0,str);
	
	while(str = UART_szGetParam()){
		uart_u16_buff[cpt]=atoi(str);
		cpt++;
	}
	
	if(arg0=='a'){
	quadramp_set_1st_order_vars(&beudabot.qr_a, uart_u16_buff[0], uart_u16_buff[1]); // Vitesse 
    quadramp_set_2nd_order_vars(&beudabot.qr_a, uart_u16_buff[2], uart_u16_buff[3]); //Acceleration 
	}
	else{
	quadramp_set_1st_order_vars(&beudabot.qr_d, uart_u16_buff[0], uart_u16_buff[1]); // Vitesse 
    quadramp_set_2nd_order_vars(&beudabot.qr_d, uart_u16_buff[2], uart_u16_buff[3]); //Acceleration 
	}
	
	//int8_t ret[UART_BUFFER_SIZE];
	sprintf(ret,"\r\nssp %c v:%d %d acc:%d %d",arg0,uart_u16_buff[0],uart_u16_buff[1],uart_u16_buff[2],uart_u16_buff[3]);
	UART_PutString(&ret);


}

void uart_getspeed(uint8_t *str){
uint8_t arg0;

str = UART_szGetParam();
strcpy(&arg0,str);

	if(arg0=='a'){
		uart_u16_buff[0]=quadramp_get_2nd_order_vars(&beudabot.qr_a);
		uart_u16_buff[1]=quadramp_get_1st_order_vars(&beudabot.qr_a);
		sprintf(ret,"\r\ngsp %c v:%d a:%d",arg0,uart_u16_buff[0],uart_u16_buff[1]);
	}
	else if(arg0=='d'){
		uart_u16_buff[0]=quadramp_get_2nd_order_vars(&beudabot.qr_d);
		uart_u16_buff[1]=quadramp_get_1st_order_vars(&beudabot.qr_d);
		sprintf(ret,"\r\ngsp %c v:%d a:%d",arg0,uart_u16_buff[0],uart_u16_buff[1]);
	}
	else
		sprintf(ret,"syn err\n\r");

	UART_PutString(&ret);
}

void uart_goxy(uint8_t *str){
	uint8_t cpt=0;

	while(str = UART_szGetParam()){
		uart_s16_buff[cpt]=atoi(str);
		cpt++;
	}

	trajectory_goto_xy_abs(&beudatraj, (double)uart_s16_buff[0],(double)uart_s16_buff[1]);
	sprintf(ret,"\r\nGo %d %d",uart_s16_buff[0],uart_s16_buff[1]);
	UART_PutString(&ret);
}

void uart_opp(uint8_t *str){
	uint8_t cpt=0;

	while(str = UART_szGetParam()){
		uart_s16_buff[cpt]=atoi(str);
		cpt++;
	}
	
	oa_set_opp(&beudaoa,uart_s16_buff[0],uart_s16_buff[1]);

	sprintf(ret,"\r\nOpp %d %d",uart_s16_buff[0],uart_s16_buff[1]);
	UART_PutString(&ret);
}

void uart_target(uint8_t *str){
uint8_t cpt=0;
	while(str = UART_szGetParam()){
		uart_s16_buff[cpt]=atoi(str);
		cpt++;
	}
	

	oa_set_target(&beudaoa, uart_s16_buff[0],uart_s16_buff[1]);
	
	sprintf(ret,"\r\ntarg %d %d",uart_s16_buff[0],uart_s16_buff[1]);
	UART_PutString(&ret);
}

void uart_gotoda(uint8_t *str){
uint8_t cpt=0;
	while(str = UART_szGetParam()){
		uart_s16_buff[cpt]=atoi(str);
		cpt++;
	}
	
	trajectory_d_a_rel(&beudatraj, (double)uart_s16_buff[0], (double)uart_s16_buff[1]);
sprintf(ret,"\r\nGo %d %d",uart_s16_buff[0],uart_s16_buff[1]);
	UART_PutString(&ret);
}

void uart_i2c(uint8_t *str){
	int8_t args[3];
	int8_t answer;
	
	str = UART_szGetParam();
	strcpy(&args[0],str);
	str = UART_szGetParam();
	strcpy(&args[1],str);

	str = UART_szGetParam();
	args[2]=atoi(str);
	
	
	if(args[0]=='>'){
		if(args[1]=='r'){
			i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, args[2]);
			sprintf(ret,"\r\ni2c> >@:%c cmd:%d ",args[1],args[2]);
		}
		else if(args[1]=='l'){
			i2c_send_only(I2C_LINTO_ADDR, args[2]);
			sprintf(ret,"\r\ni2c> >@:%c cmd:%d ",args[1],args[2]);
		}
		else if(args[1]=='p'){
			i2c_send_only(I2C_PINCE_ADDR, args[2]);
			sprintf(ret,"\r\ni2c> >@:%c cmd:%d ",args[1],args[2]);
		}
		else
			sprintf(ret,"syn err\n\r");
	}
	else{
		if(args[1]=='r'){
			answer=i2c_send_and_receive(I2C_TAPIS_ROULEAUX_ADDR, args[2]);
			sprintf(ret,"\r\ni2c> < @:%c cmd:%d ans:%d ",args[1],args[2],answer);
		}
		else if(args[1]=='l'){
			answer=i2c_send_and_receive(I2C_LINTO_ADDR, args[2]);
			sprintf(ret,"\r\ni2c> < @:%c cmd:%d ans:%d ",args[1],args[2],answer);
		}
		else if(args[1]=='p'){
			answer=i2c_send_and_receive(I2C_PINCE_ADDR, args[2]);
			sprintf(ret,"\r\ni2c> < @:%c cmd:%d ans:%d ",args[1],args[2],answer);
		}
		else if(args[1]=='i'){
			answer=i2c_send_and_receive(I2C_CAPTEUR_IR_ADDR, args[2]);
			sprintf(ret,"\r\ni2c> < @:%c cmd:%d ans:%d ",args[1],args[2],answer);
		}
		else
			sprintf(ret,"syn err\n\r");
	
	}
	
	UART_PutString(&ret);
}

