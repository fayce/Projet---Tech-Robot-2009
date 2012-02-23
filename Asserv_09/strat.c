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
 
 
 #include "strat.h"


void strat_init(struct strategy * strat){
	strat->flags = 0;
	strat->loaded_elts = 0;
	rand_disp_position=0;
	strat_flags=0;
	setBit(strat_flags,LINT_IS_LOADED); //we got one at the beginning...
	
	//Strat_State = STRAT_GET_ELT_ON_TABLE;
	/*everything is fucked up ... playing with emergency behaviour :'( */
	Strat_State = STRAT_EMERGENCY_BEHAVIOUR; 
	
	current_building_area = 1; //we start by building in our area...

 }
 
int8_t strat_store_elt(uint8_t nb_elements){  //return 1 if not finished, and 0 if not
 /*here we have nb elts, and we want to store them:
 1- prepare pince
 2- store 1st elts
 3 - prepare pince
 4 -store 2nd elts
 5 - there is no 5, the maximum storage capacity is 2!
 */
 static int8_t state;
 
 	switch (state){
		case 0:
			if(i2c_send_and_receive(I2C_PINCE_ADDR,I2C_PINCE_PREPARE)==1){
				state=1; //the pince is ready
				return 0;
			}
		break;
	
		case 1:
			if(i2c_send_and_receive(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_STOCK)==1){
				state=2;
				if(nb_elements == 1)
					return 1;
				else
					return 0;	
			}
		break;
		
		case 2: //prep the pince for the 2nd elt
			if(i2c_send_and_receive(I2C_PINCE_ADDR,I2C_PINCE_PREPARE)==1){
				state=3;
				return 0;
			}
		break;
		
		case 3:
			if(i2c_send_and_receive(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_STOCK)==1){
				// tapis has stored another one..
				return 1;
			}
		break;
	}
	return 0;
 }
 
void strat_go_get_elts_on_table(struct strategy * strat){
static uint8_t elts_to_store;
static uint24_t starttime; 
static uint8_t storage_lock;

	switch (Strat_Action) {
		case 0:
		/*Go p1*/
			i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_TAPIS_BOUFFE);
			if(beudabot.color==COLOR_GREEN)
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_G1_X,ON_TABLE_ELT_G1_Y);
			else
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_R1_X,ON_TABLE_ELT_R1_Y);
			Strat_Action++;
		break;
		
		case 1:
		/*Go p2*/	
			if(beudabot.color==COLOR_GREEN)
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_G1_X+60,ON_TABLE_ELT_G1_Y);
			else
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_R1_X+60,ON_TABLE_ELT_R1_Y);
			Strat_Action++;
		break;
		
		case 2:
			Strat_Action=0;
			Strat_State=STRAT_EMERGENCY_BEHAVIOUR;
		break;
		/*
		case 2:
			//before continuing the seq, we check how many elts we got
			elts_to_store = i2c_send_and_receive(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_GET_NB_ELTS_IN);
			Strat_Action++;
		break;*/
		
		case 3:
			if(elts_to_store==2){
				if(strat_store_elt(elts_to_store)){
					Strat_Action++;	
					setBit(strat_flags,TWO_ELTS_LOADED);
					i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_TAPIS_BOUFFE); //after storage, it stops bouffing ??
			
				}
			}
			else //if we don't have 2elts we just continu the seq.
				Strat_Action++;
		break;
		
		case 4:
		/*here, if we got 1 or 2 elements, they're stored, and the pince is well placed*/
			quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_SLOW, QR_SPEED_D_NEG_SLOW); // Vitesse 
			if(beudabot.color==COLOR_GREEN)
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_G1_X+60,ON_TABLE_ELT_G1_Y+25);
			else	
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_R1_X+60,ON_TABLE_ELT_R1_Y-25);
			Strat_Action++;	
		break;
		
		case 5:
			elts_to_store = i2c_send_and_receive(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_GET_NB_ELTS_IN);
			Strat_Action++;	
		break;
		
		case 6:
			if(elts_to_store==2){
				if(strat_store_elt(elts_to_store)){
				/*Warning: no timeout here, we trust the tapis-roulo at this time...*/
					if(strat_flags & TWO_ELTS_LOADED){
					//Yes !! we got 4 elts, lets build a real temple!
						i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_PROTECT); //we don't want more for the moment
						Strat_Action = 0;
						Strat_State++; 
						setBit(strat_flags,I_CAN_BUILD_A_TEMPLE); //for lint deposit operation...
						clearBit(strat_flags,TWO_ELTS_LOADED);
					}
					
				}
			}
			else 
				Strat_Action++;
			
		break;
		
		case 7:
		/*
		here the situation is:
		- we got 2 elts stored, and we are looking for 2 others...
		- we have nothing.. (check strat_flags & TWO_ELTS_LOADED the know the answer...)
		*/
			if(beudabot.color==COLOR_GREEN)		
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_G1_X,ON_TABLE_ELT_G1_Y+25);
			else
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_R1_X,ON_TABLE_ELT_R1_Y-25);
			Strat_Action++;
		break;

		case 8:
			elts_to_store = i2c_send_and_receive(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_GET_NB_ELTS_IN);
			Strat_Action++;	
		break;
		
		case 9:
			if(elts_to_store==2){ //owkey, we have now 2 elts 
				if(strat_store_elt(elts_to_store)){
					if(strat_flags & TWO_ELTS_LOADED){ //AWESOME, we got 2elts inside, and 2elts stored...
						i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_PROTECT); //we don't want more for the moment
						Strat_Action = 0;
						Strat_State++; //let's build !!
						clearBit(strat_flags,TWO_ELTS_LOADED);
						setBit(strat_flags,I_CAN_BUILD_A_TEMPLE); //for lint deposit operation lock...
					}
				}
			}
			else 
				Strat_Action++;
		break;
		case 10:
			if(beudabot.color==COLOR_GREEN)
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_G1_X,ON_TABLE_ELT_G1_Y+50);
			else
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_R1_X,ON_TABLE_ELT_R1_Y-50);
			Strat_Action++;	
		break;
		
		case 11:
			elts_to_store = i2c_send_and_receive(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_GET_NB_ELTS_IN);
			Strat_Action++;	
		break;
		
		case 12:
			if(elts_to_store==2){
				if(strat_store_elt(elts_to_store)){
					if(strat_flags & TWO_ELTS_LOADED){  //AWESOME, we got 2elts inside, and 2elts stored...
						Strat_Action = 0;
						i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_PROTECT); //we don't want more for the moment
						Strat_State++; //next op is building...
						setBit(strat_flags,I_CAN_BUILD_A_TEMPLE); //for lint deposit operation...
						clearBit(strat_flags,TWO_ELTS_LOADED);
					}	
				}
			}
			else 
				Strat_Action++;
		break;
			
		case 13:
			if(beudabot.color==COLOR_GREEN)
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_G1_X+60,ON_TABLE_ELT_G1_Y+50);	
			else
				trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_R1_X+60,ON_TABLE_ELT_R1_Y-50);
			Strat_Action++;	
		break;
		
		case 14:	
			/*I still don't have 4 elts :(, let's build with what we have*/
			UART_CPutString("Going to build with less than 4elts :( go back and try again ???\r\n");
			Strat_Action=0;
			Strat_State++;	
			clearBit(strat_flags,TWO_ELTS_LOADED);
		break;
	}
}
 
void strat_construct_temple(struct strategy * strat,uint8_t build_area,uint8_t half){
static uint24_t starttime;

	if(beudatraj.state==READY){
	
		switch(Strat_Action){	
		
			case 0:
				if(build_area==3){
					if(beudabot.color==COLOR_GREEN)
						oa_set_target(&beudaoa, BUILD_AREA_3b1_X,BUILD_AREA_3b1_Y);
					else
						oa_set_target(&beudaoa, BUILD_AREA_3b2_X,BUILD_AREA_3b2_Y);
				}
				else if(build_area==2){ 
					if(half == 1)
						oa_set_target(&beudaoa, BUILD_AREA_2b1_X,BUILD_AREA_2b1_Y);
					else
						oa_set_target(&beudaoa, BUILD_AREA_2b2_X,BUILD_AREA_2b2_Y);
				}
				else if(build_area==1){
					/*here we should check our pos and determinate the closest one*/
					if(half == 1)
						oa_set_target(&beudaoa, BUILD_AREA_1b1_X,BUILD_AREA_1b1_Y);
					else if(half == 2)
						oa_set_target(&beudaoa, BUILD_AREA_1b2_X,BUILD_AREA_1b2_Y);
					
					else if(half == 3)
						oa_set_target(&beudaoa, BUILD_AREA_1b3_X,BUILD_AREA_1b3_Y);
					
					else if(half == 4)
						oa_set_target(&beudaoa, BUILD_AREA_1b4_X,BUILD_AREA_1b4_Y);
					
				}
				Strat_Action++;
			break;
		
			case 1:
				if(beudaoa.state==OA_IN_TARGET){
					//trajectory_turnto_xy_behind(&beudatraj, 105, 150);
					if(build_area==3){
						if(beudabot.color==COLOR_GREEN)
							trajectory_a_abs(&beudatraj,-90);
						else
							trajectory_a_abs(&beudatraj,90);
					}
					else if(build_area==2 || build_area==1){
						trajectory_a_abs(&beudatraj,180);
					}
				}
				Strat_Action++;
			break;
		
			case 2:
			//here whe sould give the back to the build area x
				quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_VERY_SLOW, QR_SPEED_D_NEG_VERY_SLOW); // Vitesse 
				trajectory_d_a_rel(&beudatraj, -20, 0);
				Strat_Action++;
			break;
		
			case 3:
				//envoi d'ordres i2c Ã  la pinc pour construire
				i2c_send_only(I2C_PINCE_ADDR,I2C_PINCE_CONSTRUCT);
				starttime = gettime();
				Strat_Action++;
			break;

			case 4:
				
				if((gettime() > starttime + DELAY_PINCE_CONSTRUCTION)){

					if(i2c_send_and_receive(I2C_PINCE_ADDR,I2C_PINCE_DID_YOU_BUILD_SMTG)==0){
						//we wait more
						if(gettime() > (starttime + 2*DELAY_PINCE_CONSTRUCTION)){
							//there is a problem here... 
							//drop the shit and let's continue the sequ...
							//pince is maybe just fucked up ...
							/*A CHANGER !!!!!*/
							Strat_State = STRAT_EMERGENCY_BEHAVIOUR;
							
							
							#ifdef DEBUG_STRAT
								UART_CPutString("Pince is down... !\r\n");
							#endif
								setBit(strat_flags,PINCE_IS_SHITTY);
						}
							
					}
					else if (i2c_send_and_receive(I2C_PINCE_ADDR,I2C_PINCE_DID_YOU_BUILD_SMTG)==2){
						//Damn it! there is already a construction here... let's move and try somewhere else!..
							Strat_Action = 0;
						//if(Strat_State <STRAT_CONSTRUCT_TEMPLE_1_2)
							strat_go_next_build_area(); //this function will change strat state to build somewhere else...
							current_building_area++; //for next call of strat_go_next_build_area();
							#ifdef DEBUG_STRAT
								UART_CPutString("Playing against RCVA || Microb ???!\r\n");
							#endif			 
					}
					else if(i2c_send_and_receive(I2C_PINCE_ADDR,I2C_PINCE_DID_YOU_BUILD_SMTG)==1){
						//finish building, let's put the lint if flag I_CAN_BUILD_A_TEMPLE is set
						Strat_Action++;
					}
				}
			break;
			
			case 5:
				if(strat_flags & I_CAN_BUILD_A_TEMPLE){
					quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_VERY_SLOW, QR_SPEED_D_NEG_VERY_SLOW); // Vitesse 
					trajectory_d_a_rel(&beudatraj, -20, 0);
					
				}
				Strat_Action++;
			break;
			
			case 6:
				if(strat_flags & I_CAN_BUILD_A_TEMPLE){
					trajectory_d_a_rel(&beudatraj, 0, 180);
				}
				Strat_Action++;
			break;
			
			case 7:
				if(strat_flags & I_CAN_BUILD_A_TEMPLE){
					trajectory_d_a_rel(&beudatraj, 20, 0);
				}
				Strat_Action++;
			break;
			
			case 8:
				if(strat_flags & I_CAN_BUILD_A_TEMPLE){
					i2c_send_only(I2C_LINTO_ADDR, I2C_LINT_DEPOSIT_DOWN);
					starttime = gettime();
				}
				Strat_Action++;
				
			break;
			
			case 9:
				if(strat_flags & I_CAN_BUILD_A_TEMPLE){
					if(gettime() > starttime + DELAY_LINT_DOWN){
						Strat_State=STRAT_GET_LINT_1;
					}
				}
				else
					Strat_State=STRAT_GET_LINT_1;
				 //get a lint, then elts, to build another one...
			break;

		}
	}
 }
 
void strat_go_next_build_area(void){

	switch(current_building_area){
	
	
		case BUILD_AREA_3:
		Strat_State = STRAT_CONSTRUCT_TEMPLE_2_1 ;
		break;
		
		case BUILD_AREA_2_1:
		Strat_State = STRAT_CONSTRUCT_TEMPLE_2_2 ;
		break;
		
		case BUILD_AREA_2_2:
		Strat_State = STRAT_CONSTRUCT_TEMPLE_1_1 ;
		break;
		
		case BUILD_AREA_1_1:
		Strat_State = STRAT_CONSTRUCT_TEMPLE_1_1 ;
		break;
	
			
		case BUILD_AREA_1_2:
		Strat_State = STRAT_CONSTRUCT_TEMPLE_1_2 ;
		break;
		
			
		case BUILD_AREA_1_3:
		Strat_State = STRAT_CONSTRUCT_TEMPLE_1_3 ;
		break;
		
			
		case BUILD_AREA_1_4:
		Strat_State = STRAT_CONSTRUCT_TEMPLE_1_4 ;
		break;
	
	}

}

void strat_go_get_lint(struct strategy * strat,uint8_t position){
static uint24_t starttime;

	if(beudatraj.state==READY){
		switch(Strat_Action){
		
			case 0:
				quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_A_POS_SLOW, QR_SPEED_A_NEG_SLOW); // Vitesse 
					if(position==1){
						if(beudabot.color==COLOR_GREEN)
							oa_set_target(&beudaoa, G_LINT_DISP_1_X-20,G_LINT_DISP_1_Y);
						else
							oa_set_target(&beudaoa, R_LINT_DISP_1_X-20,R_LINT_DISP_1_Y);
					}
					else if(position==2){
						if(beudabot.color==COLOR_GREEN)
							oa_set_target(&beudaoa, G_LINT_DISP_2_X-20,G_LINT_DISP_2_Y);
						else
							oa_set_target(&beudaoa, R_LINT_DISP_2_X-20,R_LINT_DISP_2_Y);
					}
				Strat_Action++;
			break;
			
			case 1:
				if(beudaoa.state==OA_IN_TARGET){
					i2c_send_only(I2C_LINTO_ADDR,I2C_LINT_GET_DOWN);
					Strat_Action++;
				}
			break;
			
			case 2:
				quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_VERY_SLOW, QR_SPEED_D_NEG_VERY_SLOW); // Vitesse 
				trajectory_d_a_rel(&beudatraj, 20, 0);	
				Strat_Action++;
			break;
			
			case 3:
				//i2c lint remont
				i2c_send_only(I2C_LINTO_ADDR,I2C_LINT_GET_UP);
				starttime = gettime();
				Strat_Action++;
			break;
			
			case 4:
				if(gettime() > starttime + DELAY_LINT_UP){
					if(i2c_send_and_receive(I2C_LINTO_ADDR, I2C_GOT_SMTG)==1){
					//go get another one, somewhere else...
						Strat_Action = 0;
						Strat_State = STRAT_GET_LINT_2;
					}
					else{ //2
						#ifdef DEBUG_STRAT
						UART_CPutString("strat> we got a lint !! \r\n");
						#endif
						Strat_Action = 0;
						Strat_State = STRAT_GET_ELT_ON_FIXED_DISP;
					}
				
				}
			break;
			
		}
	}	

}

void strat_go_get_elts_disp(struct strategy *strat,int8_t type){ //type = 0 FIXED,1 RAND1,2 RAND2

static uint8_t elts_to_store;
uint24_t starttime;
 	if(beudatraj.state==READY){
		switch(Strat_Action){
		
			case 0:
				quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_A_POS_SLOW, QR_SPEED_A_NEG_SLOW); // Vitesse 
				
				if(beudabot.color==COLOR_GREEN){
					if(type==0)
						oa_set_target(&beudaoa, G_ELT_DISP_FIXED_X-30, G_ELT_DISP_FIXED_Y);
					else if(type==1)
						oa_set_target(&beudaoa, G_ELT_DISP_RAND_1_X, G_ELT_DISP_RAND_1_Y-30);
					else if(type==2)
						oa_set_target(&beudaoa, G_ELT_DISP_RAND_2_X, G_ELT_DISP_RAND_2_Y-30);
				}	
				else{
					if(type==0)
						oa_set_target(&beudaoa, R_ELT_DISP_FIXED_X-30, R_ELT_DISP_FIXED_Y);
					else if(type==1)
						oa_set_target(&beudaoa, R_ELT_DISP_RAND_1_X, R_ELT_DISP_RAND_1_Y-30);
					else if(type==2)
						oa_set_target(&beudaoa, R_ELT_DISP_RAND_2_X, R_ELT_DISP_RAND_2_Y-30);
				}	
				
				Strat_Action++;
			break;
			
			case 1:
				if(beudaoa.state == OA_IN_TARGET)
					if(beudabot.color ==COLOR_GREEN){
						if(type==0)
							trajectory_goto_xy_abs(&beudatraj,G_ELT_DISP_FIXED_X, G_ELT_DISP_FIXED_Y);
						else if(type==1)
							trajectory_goto_xy_abs(&beudatraj,G_ELT_DISP_RAND_1_X, G_ELT_DISP_RAND_1_Y);
						else if (type==2)
							trajectory_goto_xy_abs(&beudatraj,G_ELT_DISP_RAND_2_X, G_ELT_DISP_RAND_2_Y);
						
					}
					else{
						if(type==0)
							trajectory_goto_xy_abs(&beudatraj,R_ELT_DISP_FIXED_X, R_ELT_DISP_FIXED_Y);
						else if(type==1)
							trajectory_goto_xy_abs(&beudatraj,R_ELT_DISP_RAND_1_X, R_ELT_DISP_RAND_1_Y);
						else if (type==2)
							trajectory_goto_xy_abs(&beudatraj,R_ELT_DISP_RAND_2_X, R_ELT_DISP_RAND_2_Y);
				
					}
					Strat_Action++;
			
			break;
		
			case 2:
				quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_VERY_SLOW, QR_SPEED_D_NEG_VERY_SLOW); // Vitesse 
				i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_TAPIS_BOUFFE);
				trajectory_d_a_rel(&beudatraj, ELT_DISP_IN_OUT_DIST, 0); //we move till the disp
				starttime = gettime();
				Strat_Action++;
				clearBit(strat_flags,TWO_ELTS_LOADED); //we prepare the flag...
			break;

			
			case 3:
				elts_to_store = i2c_send_and_receive(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_GET_NB_ELTS_IN);
				if(elts_to_store==2){
				/*Woow I got 2 elts, let's store them now, I suppose the tapis-roulo stoped rolling by himself now..*/
					if(strat_store_elt(elts_to_store)){
					/*if storage proc ended...*/
						Strat_Action++; //we got 2, let's take some other two..
						setBit(strat_flags,TWO_ELTS_LOADED);
					}
				}
				else{
				//if it's taking too much time we go builg smtg
				/*we make a special move to get them....or not*/
					if(gettime()> starttime + DELAY_TAPIS_BOUFFE)
						Strat_Action++;
				}
			break;
			
			case 4:
			/*we suck again here...*/
				i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_TAPIS_BOUFFE);
				Strat_Action++;	
			break;
			
			case 5:
				elts_to_store = i2c_send_and_receive(I2C_TAPIS_ROULEAUX_ADDR,I2C_TAPIS_GET_NB_ELTS_IN);
				
				if(elts_to_store==2){
				
				/*Woow I got 2 elts, let's store them now, I suppose the tapis-roulo stoped rolling by himself now..*/
					if(strat_store_elt(elts_to_store)){
					/*if storage proc ended...*/
						Strat_State=STRAT_CONSTRUCT_TEMPLE_2_1; //we got 2, let's build smtg
						setBit(strat_flags,I_CAN_BUILD_A_TEMPLE);
					}
				}
				else{
				//if it's taking too much time we go builg smtg
				/*we make a special move to get them....or not*/
					if(gettime()> starttime + DELAY_TAPIS_BOUFFE){
						Strat_State=STRAT_CONSTRUCT_TEMPLE_2_1;
					/*we can't wait more... let's just build smtg*/
						clearBit(strat_flags,I_CAN_BUILD_A_TEMPLE);//without a lint...
					}
				}
			
				
			break;
		}
	}
 }
 
void strat_get_elt_rand_disp_position(void){
int8_t i2c_ans;
	if(beudatraj.state==READY){
		switch(Strat_Action){
			case 0:
				quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_SLOW, QR_SPEED_D_NEG_SLOW); // Vitesse 
			
				if(beudabot.color==COLOR_GREEN)
					oa_set_target(&beudaoa, R_ELT_DISP_RAND_1_X, R_ELT_DISP_RAND_1_Y+30);	
				else
					oa_set_target(&beudaoa, G_ELT_DISP_RAND_1_X, G_ELT_DISP_RAND_1_Y-30);
				Strat_Action++;
			break;
			
			case 1:
				if(beudaoa.state == OA_IN_TARGET){
					quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_VERY_SLOW, QR_SPEED_D_NEG_VERY_SLOW); // Vitesse 
						trajectory_d_a_rel(&beudatraj,30,0);
						
				}
				Strat_Action++;
			break;
			
			case 2:
				i2c_ans=i2c_send_and_receive(I2C_CAPTEUR_IR_ADDR,I2C_IR_CHECK_IN_FRONT);
				if(i2c_ans==1){
					/*disp is not here*/
					//rand_disp_position=ELT_LOCATION_RAND2;
					Strat_Action++;
					}
				else if(i2c_ans==2) {
					/*Yeay we got it ! !*/
					Strat_Action=0;
					Strat_State = STRAT_CONSTRUCT_TEMPLE_2_1;
					//rand_disp_position=ELT_LOCATION_RAND1;
				}
			break;
			
			case 3:
				trajectory_d_a_rel(&beudatraj,-50,0);
				Strat_Action++;
			break;
			
			case 4:
				if(beudabot.color==COLOR_GREEN)
					trajectory_goto_xy_abs(&beudatraj, G_ELT_DISP_RAND_2_X, G_ELT_DISP_RAND_2_Y-30);	
				else
					trajectory_goto_xy_abs(&beudatraj, G_ELT_DISP_RAND_2_X, G_ELT_DISP_RAND_2_Y-30);	
			
			break;

			
			case 5:
				/*the disp isn't the 1st, we get to the 2nd*/
				if(beudabot.color==COLOR_GREEN)
					oa_set_target(&beudaoa, R_ELT_DISP_RAND_1_X, R_ELT_DISP_RAND_1_Y+30);	
				else
					oa_set_target(&beudaoa, G_ELT_DISP_RAND_1_X, G_ELT_DISP_RAND_1_Y-30);
				Strat_Action++;
			break;
		}
	}
} 


void strat_do_emergency_behaviour(void){
//when pince is down... :s
static uint24_t starttime;
	if(beudatraj.state==READY){	
		int16_t my_y = position_get_y_s16(&beudabot.posr);
		
		switch (Strat_Action) {
			case 0:
			/*Go p1*/
				i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_TAPIS_BOUFFE);
				if(beudabot.color==COLOR_GREEN)
					trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_G1_X,ON_TABLE_ELT_G1_Y);
				else
					trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_R1_X,ON_TABLE_ELT_R1_Y);
				Strat_Action++;
			break;
			
			case 1:
			/*Go p2*/	
			i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_TAPIS_BOUFFE);
				if(beudabot.color==COLOR_GREEN)
					trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_G1_X+60,ON_TABLE_ELT_G1_Y);
				else
					trajectory_goto_xy_abs(&beudatraj,ON_TABLE_ELT_R1_X+60,ON_TABLE_ELT_R1_Y);
				Strat_Action++;
			break;
			
			case 2:
			i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_TAPIS_BOUFFE);
			
				if(beudabot.color==COLOR_GREEN)
					trajectory_goto_xy_abs(&beudatraj, BUILD_AREA_1b1_X-30,BUILD_AREA_1b1_Y);
				else
					trajectory_goto_xy_abs(&beudatraj, BUILD_AREA_1b4_X-30,BUILD_AREA_1b4_Y);
					
				Strat_Action++;
			break;
			
			case 3:
			
				trajectory_a_abs(&beudatraj,0);
				Strat_Action++;
			break;
			
			case 4:
			quadramp_set_1st_order_vars(&beudabot.qr_d, QR_SPEED_D_POS_VERY_SLOW, QR_SPEED_D_NEG_VERY_SLOW); // Vitesse 
				
				trajectory_d_a_rel(&beudatraj, 30, 0);
				Strat_Action++;
			break;
			
			case 5:
				i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_TAPIS_EJECT);	
				starttime = gettime();
				Strat_Action++;
			break;
			
			case 6:
				if(gettime()>starttime + DELAY_TAPIS_EJECT)
					Strat_Action++;
			break;
			
			case 7:
				trajectory_d_a_rel(&beudatraj, -30, 0);
				Strat_Action++;
			break;
			
			case 8:
				Strat_Action=0;
				Strat_State=STRAT_EXIT;
			break;
		}
	}
}

void strat_manage(struct strategy * strat){
	static uint24_t starttime;
	
	if( beudatraj.state==READY){
	
			switch(Strat_State){
				case STRAT_GET_ELT_ON_TABLE:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> get elt on table\r\n");
				#endif
					strat_go_get_elts_on_table(strat);
				break;
			
					 
				case STRAT_CONSTRUCT_TEMPLE_3:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> hav to build a temple 3\r\n");
				#endif
					strat_construct_temple(strat,3,0); //0 it depends on our color
				break;
				
				case STRAT_GET_LINT_1:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> get lint area 1\r\n");
				#endif
					strat_go_get_lint(strat,LINT_DISP_L1);
				break;
				
				case STRAT_GET_LINT_2:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> get lint area 2\r\n");
				#endif
					strat_go_get_lint(strat,LINT_DISP_L2);
				break;
				
				case STRAT_GET_ELT_ON_FIXED_DISP:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> get elt on fixed disp\r\n");
				#endif
					strat_go_get_elts_disp(strat,0);
				break;
				
				
				case STRAT_GET_ELT_ON_RAND_DISP_1:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> get elt on rand 1\r\n");
				#endif
					strat_go_get_elts_disp(strat,1);
				break;
				
				case STRAT_GET_ELT_ON_RAND_DISP_2:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> get elt on disp 2\r\n");
				#endif
					strat_go_get_elts_disp(strat,2);
				break;
				 
				case STRAT_CONSTRUCT_TEMPLE_2_1:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> constr temp 2 1\r\n");
				#endif
					strat_construct_temple(strat,2,1);

				break;
				
				case STRAT_CONSTRUCT_TEMPLE_2_2:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> constr temp 2 2\r\n");
				#endif
					strat_construct_temple(strat,2,2);
				break;
			
				case STRAT_CONSTRUCT_TEMPLE_1_1:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> constr temp 1 1\r\n");
				#endif
					strat_construct_temple(strat,1,1);
				break;
				
				case STRAT_CONSTRUCT_TEMPLE_1_2:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> constr temp 1 2\r\n");
				#endif
					strat_construct_temple(strat,1,2);
				break;
				
				case STRAT_CONSTRUCT_TEMPLE_1_3:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> constr temp 1 3\r\n");
				#endif
					strat_construct_temple(strat,1,3);
				break;
				
				case STRAT_CONSTRUCT_TEMPLE_1_4:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> constr temp 1 4\r\n");
				#endif
					strat_construct_temple(strat,1,4);
				break;
				
				case STRAT_EMERGENCY_BEHAVIOUR:
				#ifdef DEBUG_STRAT
				UART_CPutString("strat> emergency behaviour on\r\n");
				#endif
					strat_do_emergency_behaviour();
					PRT2DR ^=0x80;
				break;
				
				case STRAT_EXIT:
			
					#ifdef DEBUG_STRAT
					UART_CPutString("strat> exit\r\n");
					#endif
					pwm_left(0);
					pwm_right(0);
					
					//ordre i2c pour sortir et ouvrir la pince
					i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_TAPIS_EJECT);
					i2c_send_only(I2C_LINTO_ADDR,I2C_LINT_DROP);
					i2c_send_only(I2C_PINCE_ADDR,I2C_PINCE_DROP);
					
					starttime = gettime();
					while(gettime() < (starttime + DELAY_TAPIS_EJECT));
						i2c_send_only(I2C_TAPIS_ROULEAUX_ADDR, I2C_TAPIS_STOP);
						clearBit(beudabot.events,EVENT_DO_STRAT);
					while(1);
				break;
			}
	}
}