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
 
#include "robot.h"
#include "init.h"
#include "flash.h"
#include "uart_cmd.h"
#include "time_machine.h"
#include "i2c_master.h"
#include "obstacle_avoidance.h"
#include "strat.h"
#include "uart_mirroir.h"


enum trajectory_state Saved_Traj_State;

void robot_do_cs(void);
void robot_do_pos(void);
void robot_do_bd(void);
void robot_start(void);
void goto_start_pos(void);
void handle_jack(void);

void handle_jack(void){
	if(GlobalFlags & FLAG_HANDLE_JACK && !(PRT1DR & 0x01)){	 
		GlobalFlags = FLAG_JACK_OUT;
		setBit(beudabot.events,EVENT_DO_STRAT);
		clearBit(GlobalFlags,FLAG_HANDLE_JACK);
	}
	if((GlobalFlags & FLAG_FAYCE_FORGOT_TO_PLUG_JACK) && (PRT1DR & 0x01)){
		UART_CPutString("\r\nSo Goood...");
		clearBit(GlobalFlags,FLAG_FAYCE_FORGOT_TO_PLUG_JACK);
		setBit(GlobalFlags,FLAG_HANDLE_JACK);
	}
}

void goto_start_pos(void){
static int8_t state;
	if(beudabot.color==COLOR_RED){
		if(beudatraj.state == READY && state==0){
			trajectory_d_a_rel(&beudatraj, 0, 45);
			state++;
		}
		
		else if(beudatraj.state == READY && state==1){
			trajectory_d_a_rel(&beudatraj, -30, 0);
			state++;
		}
		else if(beudatraj.state == READY && state==2){
			setBit(beudabot.events,EVENT_POSITIONED);
			beudaoa.opponent_xy.x = 1;
			beudaoa.opponent_xy.y = 1;
			quadramp_set_1st_order_vars(&beudabot.qr_a, QR_SPEED_A_POS_FAST, QR_SPEED_A_NEG_FAST); // Vitesse
			quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_FAST, QR_SPEED_D_NEG_FAST); // Vitesse
		}
		
	}
	else if(beudabot.color==COLOR_GREEN){
		if(beudatraj.state == READY && state==0){
			trajectory_d_a_rel(&beudatraj, 0, -45);
			state++;
		}
		
		else if(beudatraj.state == READY && state==1){
			trajectory_d_a_rel(&beudatraj, -30, 0);
			state++;
		}
		else if(beudatraj.state == READY && state==2){
			setBit(beudabot.events,EVENT_POSITIONED);
			
			beudaoa.opponent_xy.x = 1;
			beudaoa.opponent_xy.y = 299;
			quadramp_set_1st_order_vars(&beudabot.qr_a, QR_SPEED_A_POS_FAST, QR_SPEED_A_NEG_FAST); // Vitesse
			quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_FAST, QR_SPEED_D_NEG_FAST); // Vitesse
	
		}
	}
}

void robot_do_cs(void){
	if(beudabot.events & EVENT_DO_CS){
		cs_manage(&beudabot.cs_a, &beudabot.rs); //takes 0.25ms
		cs_manage(&beudabot.cs_d, &beudabot.rs);//takes 0.25ms	
	}
}

void robot_do_pos(void){
	rs_update(&beudabot.rs);
	position_manage(&beudabot.posr); //calc x,y,a
}


void main(){

   	M8C_EnableGInt;

	init_uart();
	init_globals();
	
	init_encoders();
	init_pwm();
	
	init_tm();
	
	init_robot(&beudabot);	
	trajectory_init(&beudatraj);
	oa_init(&beudaoa);
	init_i2c_master();
	init_uart_mirr();
	strat_init(&beudastrat);
	
	/*****************************************************************************************/
	/*							Chargement conf PID						  		 */
	/*****************************************************************************************/
	//flash_load_pid_cfg(&beudabot.pid_a,&beudabot.pid_d);
	pid_set_gains(&beudabot.pid_a, PIDA_P, PIDA_I, PIDA_D);
	pid_set_out_shift(&beudabot.pid_a, PIDA_SH);

	pid_set_gains(&beudabot.pid_d, PIDD_P, PIDD_I, PIDD_D);
	pid_set_out_shift(&beudabot.pid_d, PIDD_SH);
	/*****************************************************************************************/
	/*						Association des filtres 								  		 */
	/*****************************************************************************************/
    cs_set_consign_filter(&beudabot.cs_d, &beudabot.qr_d);
    cs_set_correct_filter(&beudabot.cs_d, &beudabot.pid_d);
    cs_set_process_out(&beudabot.cs_d, PROCESS_DISTANCE);
  	cs_set_consign(&beudabot.cs_d, 0);
    cs_set_consign_filter(&beudabot.cs_a, &beudabot.qr_a);
    cs_set_correct_filter(&beudabot.cs_a, &beudabot.pid_a);
    cs_set_process_out(&beudabot.cs_a, PROCESS_ANGLE);
    cs_set_consign(&beudabot.cs_a, 0);
	
	/*****************************************************************************************/
	/*							Config Rampe Trapezoidale							  		 */
	/*****************************************************************************************/
 	quadramp_set_1st_order_vars(&beudabot.qr_a, QR_SPEED_A_POS_SLOW, QR_SPEED_A_NEG_SLOW); // Vitesse 
    quadramp_set_2nd_order_vars(&beudabot.qr_a, QR_ACC_A_POS, QR_ACC_A_NEG); //Acceleration 
	
 	quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_SLOW, QR_SPEED_D_NEG_SLOW);
    quadramp_set_2nd_order_vars(&beudabot.qr_d, QR_ACC_D_POS, QR_ACC_D_NEG);

	/*****************************************************************************************/
	/*							Config Gestionnaire Position						  		 */
	/*****************************************************************************************/
	position_set_physical_params(&beudabot.posr, TRACK_CM, DIST_IMP_PER_CM);
    position_set_related_robot_system(&beudabot.posr, &beudabot.rs);
	
	/*****************************************************************************************/
	/*							Config Gestionnaire Trajectoire						  		 */
	/*****************************************************************************************/
	trajectory_set_cs(&beudatraj, &beudabot.cs_d, &beudabot.cs_a);
    trajectory_set_robot_params(&beudatraj, &beudabot.rs, &beudabot.posr);   
	
	/*****************************************************************************************/
	/*							Obstacle Avoidance				  					  		 */
	/*****************************************************************************************/
	oa_set_traj(&beudaoa,&beudatraj);
	

	uart_welcome();
	uart_prompt();	

	init_timer();
	
	egi(); //enable global interrupt  
	uint24_t starttime=0;
	while(1){
	
		if(TIME_MACHINE_DO_CS){
			TIME_MACHINE_DO_CS=0; // Clear Time Slot Bit
			robot_do_cs();
		}
		
		if((beudabot.color!=0) && !(beudabot.events & EVENT_POSITIONED))
			goto_start_pos();
	
		if((beudabot.events & EVENT_DO_STRAT) && TIME_MACHINE_DO_STRAT){
			strat_manage(&beudastrat);
			oa_manage(&beudaoa);
	
			
			PRT2DR ^= LED_1;
			TIME_MACHINE_DO_STRAT=0;
			
		}
	
		if(TIME_MACHINE_DO_TRAJ){
			trajectory_manager_event(&beudatraj);
			TIME_MACHINE_DO_TRAJ=0;
		}
		if(beudabot.events & EVENT_DO_POS)
			robot_do_pos();
		
		uart_check();
		
	
		if(TIME_MACHINE_DO_IR){
			if(i2c_send_and_receive(I2C_CAPTEUR_IR_ADDR,I2C_IR_CHECK_IN_FRONT)!=0 && (beudabot.events & EVENT_DO_STRAT)){
				if(oa_do_i_need_to_stop(&beudaoa)){
				//	UART_CPutString("Opp just in front !!!\r\n");
					trajectory_d_a_rel(&beudatraj, 0, 0);
					starttime = gettime();
					Saved_Traj_State = beudatraj.state;
					beudatraj.state = INTERRUPTED;
				}
			}
			TIME_MACHINE_DO_IR=0;
			
		}
	
		if((beudatraj.state == INTERRUPTED) && (gettime() > (starttime + 1000))){
			beudatraj.state = Saved_Traj_State;
		}
		
		//uart_mirr_check();
		handle_jack();
	
		
	}
}



//Comptage des top codeurs
void interrupt DigBuf_1_ISR(void) @0x20 {

	if(PRT0DR & C1CHB)
		TopD--;
		
	else	
		TopD++;

}

void interrupt DigBuf_2_ISR(void)@0x24 {

	if(PRT0DR & C2CHB)
		TopG++;
	else	
		TopG--;

}


void interrupt Timer8_1_ISR(void) @0x38 {
//time 8ms
	time_slice_cpt++;
		TIME_MACHINE_DO_CS = 1;
		
	if(time_slice_cpt%50==0){
		TIME_MACHINE_DO_STRAT = 1;
		TIME_MACHINE_DO_TRAJ=1;
	}
	
	if(time_slice_cpt%20==0){
		TIME_MACHINE_DO_IR=1;
	}
	
	if((beudabot.events & EVENT_DO_STRAT) && (time_slice_cpt>9720)){ //85sec
		//UART_CPutString("Strat exited\r\n");
		//Strat_State = STRAT_EXIT;
		trajectory_d_a_rel(&beudatraj, 0, 0);
		clearBit(beudabot.events,EVENT_DO_CS);
		clearBit(beudabot.events,EVENT_DO_STRAT);
				pwm_left(0);
					pwm_right(0);
		while(1);
	}
}
