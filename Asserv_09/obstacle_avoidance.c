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
 *  Remy NOEL <mocramis@gmail.com> (brainstorming assistant ;)
 *
 */
 
#include "obstacle_avoidance.h"

void oa_init(struct obstacle_avoidance * oa){
    memset(oa,0,sizeof(* oa));
   // oa->traj = NULL;
    /*we fill the forbidden areas, that are in fact the build areas*/
    
    oa->forbidden_area[0].x = FB_AREA_0_X;
    oa->forbidden_area[0].y = FB_AREA_0_Y;
    
    oa->forbidden_area[1].x = FB_AREA_1_X;
    oa->forbidden_area[1].y = FB_AREA_1_Y;
    
    oa->forbidden_area[2].x = FB_AREA_2_X;
    oa->forbidden_area[2].y = FB_AREA_2_Y;
    
    oa->forbidden_area[3].x = FB_AREA_3_X;
    oa->forbidden_area[3].y = FB_AREA_3_Y;
    
    oa->forbidden_area[4].x = FB_AREA_4_X;
    oa->forbidden_area[4].y = FB_AREA_4_Y;
    
    oa->forbidden_area[5].x = FB_AREA_5_X;
    oa->forbidden_area[5].y = FB_AREA_5_Y;
	
    oa->forbidden_area[6].x = FB_AREA_6_X;
    oa->forbidden_area[6].y = FB_AREA_6_Y;
    
    oa->forbidden_area[7].x = FB_AREA_7_X;
    oa->forbidden_area[7].y = FB_AREA_7_Y;
    
    oa->forbidden_area[8].x = FB_AREA_8_X;
    oa->forbidden_area[8].y = FB_AREA_8_Y;
    
    oa->forbidden_area[9].x = FB_AREA_9_X;
    oa->forbidden_area[9].y = FB_AREA_9_Y;
    
    oa->forbidden_area[10].x = FB_AREA_10_X;
    oa->forbidden_area[10].y = FB_AREA_10_Y;
    
    oa->forbidden_area[11].x = FB_AREA_11_X;
    oa->forbidden_area[11].y = FB_AREA_11_Y;
	    
	oa->state = OA_READY_NO_TARGET;
}

play_area oa_xy_to_area(int16_t x,int16_t y){
    play_area area;
    area.x=1;
    area.y=1;
       
    while(x>area.x*SQ_SIZE_X){
        area.x++;      
    }
    while(y>area.y*SQ_SIZE_Y){
        area.y++;      
    }    
    
return area;
}

xy_position oa_area_to_xy(play_area area){
    xy_position centered_pos; 
    centered_pos.x = (int16_t)(SQ_SIZE_X*area.x - SQ_SIZE_X/2);
    centered_pos.y = (int16_t)(SQ_SIZE_Y*area.y - SQ_SIZE_Y/2);
return centered_pos;
}

void oa_set_opp(struct obstacle_avoidance * oa,int16_t x,int16_t y){
	oa->opponent_xy.x = x;
	oa->opponent_xy.y = y;
}

void oa_find_adj_areas_to_target(struct obstacle_avoidance * oa,play_area * my_area,play_area * target_area){
  
    int8_t sens_x,sens_y;
   
	sens_x = target_area->x - my_area->x; 
	sens_y = target_area->y - my_area->y; 
		
    if(sens_x>0){ //we have to increase area x to get to target
        oa->adj_area_tab[0].x = my_area->x+1;
		sens_x=1;
	}
    else if(sens_x<0){
        oa->adj_area_tab[0].x = my_area->x-1;
		sens_x = -1;
	}
    else //==0
        oa->adj_area_tab[0].x = my_area->x;
        
    if(sens_y>0){
        oa->adj_area_tab[0].y = my_area->y+1;
		sens_y = 1;
	}
    
	else if(sens_y<0){
        oa->adj_area_tab[0].y = my_area->y-1;
		sens_y = -1;
	}
    else
        oa->adj_area_tab[0].y = my_area->y;
/*
Here we have to differenciate two cases:
    1st: (x: target R: my robot, the . are the adj_areas to targed)  the move is straight
        -------------------
        |     |   x  |     |
        |-----|------|-----|
        |  .  |   .  |  .  |     
        |-----|------|-----|       
        |     |   R  |     |
        |-----|------|-----|
        
    2nd: the move is diagonal
        -------------------
        |   R |   .  |     |
        |-----|------|-----|
        |  .  |   .  |     |     
        |-----|------|-----|       
        |     |      |  x  |
        |-----|------|-----|
*/
    if(sens_x == 0 || sens_y == 0){ //we are in the 1st case here
        oa->adj_area_tab[1].x =  oa->adj_area_tab[0].x - sens_y;
        oa->adj_area_tab[1].y = oa->adj_area_tab[0].y - sens_x;
 
        oa->adj_area_tab[2].x = oa->adj_area_tab[0].x + sens_y;
        oa->adj_area_tab[2].y = oa->adj_area_tab[0].y + sens_x;
    }
    
    else{ //none of them are 0, so we are in the 2nd case
        //we save the 2 other possible combinations
        oa->adj_area_tab[1].x =  oa->adj_area_tab[0].x;
        oa->adj_area_tab[1].y = my_area->y;
 
        oa->adj_area_tab[2].x = my_area->x;
        oa->adj_area_tab[2].y = oa->adj_area_tab[0].y;
    }
	
	//sprintf(uart_out_buffer,"oa> my area {%d,%d} target {%d,%d}",my_area.x,my_area.y,target_area.x,target_area.y);
	//UART_PutString(&uart_out_buffer);
	sprintf(uart_out_buffer,"next areas: {%d,%d}{%d,%d}{%d,%d} \r\n",oa->adj_area_tab[0].x,oa->adj_area_tab[0].y,oa->adj_area_tab[1].x,oa->adj_area_tab[1].y,oa->adj_area_tab[2].x,oa->adj_area_tab[2].y);
	UART_PutString(&uart_out_buffer);
}

void oa_set_target(struct obstacle_avoidance * oa,int16_t x,int16_t y){
    oa->target_xy.x = x;
	oa->target_xy.y = y;
	oa->state = OA_TARGET_SET;
	
	#ifdef DEBUG_OA
	sprintf(uart_out_buffer,"oa> New target! -> {%d,%d}\r\n", oa->target_xy.x,oa->target_xy.y);
	UART_PutString(&uart_out_buffer);
	#endif
}
      
void oa_sort_tab3(int16_t * tabtosort,uint8_t * sortedindex){
	
	if(tabtosort[0]>tabtosort[2]){
		if(tabtosort[1]>tabtosort[0]){
			sortedindex[0]=2;
			sortedindex[1]=0;
			sortedindex[2]=1;
		}
		else{
			sortedindex[2]=0;      
			if(tabtosort[1]>tabtosort[2]){
				sortedindex[0]=2;
				sortedindex[1]=1;        
			}
			else{
				sortedindex[0]=1;
				sortedindex[1]=2;      
			}
		}   
	}
	else{
		if(tabtosort[1]<tabtosort[2]){
			if(tabtosort[1]<tabtosort[0]){
				sortedindex[0]=1;
				sortedindex[1]=0;
				sortedindex[2]=2;
			}    
			else{
				sortedindex[0]=0;
				sortedindex[1]=1;
				sortedindex[2]=2;  
			}
		}
		else{
			sortedindex[0]=0;
			sortedindex[1]=2;
			sortedindex[2]=1;    
		}
	}

}



//Warning, this function does a lot of calculus, execute it while the robot doesn't move, to avoid any x,y deriv

void oa_sixt(struct obstacle_avoidance * oa){
/*we check different possible "next_area" that are in fact the adj_areas
and try to find the shortest, and the most safe one*/

    //we start by computing the possibles next areas
    int16_t my_pos_x,my_pos_y;
	play_area my_area,next_area,target_area,opp_area;
	uint8_t i,j;
    int16_t d_nxt_area_opp[3],d_nxt_area_target[3];
	float tmp;
	uint8_t index0,index1,index2;
	xy_position my_pos_xy;
	
	next_area.x=0;
	next_area.y=0;
	
	my_pos_x=position_get_x_s16(oa->traj->position);
    my_pos_y=position_get_y_s16(oa->traj->position);
    
	my_pos_xy.x = my_pos_x;
	my_pos_xy.y = my_pos_y;
	
	my_area = oa_xy_to_area(my_pos_x,my_pos_y);
	target_area = oa_xy_to_area(oa->target_xy.x,oa->target_xy.y);
	opp_area = oa_xy_to_area(oa->opponent_xy.x,oa->opponent_xy.y);
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	char is_opp_adj=oa_is_in_v4(&my_area,&opp_area);
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	
	#ifdef DEBUG_OA
	sprintf(uart_out_buffer,"oa> my area {%d,%d}\r\n",my_area.x,my_area.y);
	UART_PutString(&uart_out_buffer);	
	sprintf(uart_out_buffer,"oa> target area {%d,%d}\r\n",target_area.x,target_area.y);
	UART_PutString(&uart_out_buffer);	
	sprintf(uart_out_buffer,"oa> opp area {%d,%d}\r\n",opp_area.x,opp_area.y);
	UART_PutString(&uart_out_buffer);
	#endif
	/*let's calc adj areas now..*/
	
	oa_find_adj_areas_to_target(oa,&my_area,&target_area);
    
   
    
    /*we check if one or more, of the computed adj areas isn't/aren't one of the forbidden areas
    or just out of the game_area*/
 
    for(i=0;i<3;i++){
        for(j=0;j<NB_FORBIDDEN_AREAS;j++){
		xy_position pos1 =  oa_area_to_xy(oa->adj_area_tab[i]);
	//	xy_position pos2 =  oa_area_to_xy(target_area);
		
            if( ( (oa->adj_area_tab[i].x == oa->forbidden_area[j].x) 
			&& (oa->adj_area_tab[i].y == oa->forbidden_area[j].y)) 
			|| oa_is_out_of_table(oa->adj_area_tab[i]) 
			|| !( oa_get_com_dt(pos1,oa->target_xy)) 
			|| !( oa_get_com_dt(my_pos_xy,pos1))
			|| ((oa->adj_area_tab[i].x ==opp_area.x)&&(oa->adj_area_tab[i].y==opp_area.y))
			){
				/*if adj area is target, just forget about the forbidden areas...*/
					
					
					if((oa->adj_area_tab[i].x == target_area.x) && (oa->adj_area_tab[i].y == target_area.y)){
					//not forbidden areas when it's out target!
						d_nxt_area_opp[i] = 0;
						d_nxt_area_target[i] = 0;
					}
					else {
						//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
						if (is_opp_adj)
						{
							if (oa_is_in_v4(&oa->adj_area_tab[i],&opp_area))
							{
								//on a une case en diagonale, on veut la remplacer
								if( oa->adj_area_tab[i].x==opp_area.x)
									//on remplace par une case du voisinage de my_area qui n'est pas l'adacent
									oa->adj_area_tab[i].x=my_area.x;
								else
									//oa->adj_area_tab[i].y==opp_area.y
									oa->adj_area_tab[i].y=my_area.y;
							}
							else
							{
								d_nxt_area_opp[i] = MARKED_FORBIDDEN;
								d_nxt_area_target[i] = MARKED_FORBIDDEN;
							}
								
						}
						else
						{
							d_nxt_area_opp[i] = MARKED_FORBIDDEN;
							d_nxt_area_target[i] = MARKED_FORBIDDEN;
						}
						//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
					}
					//if((oa->adj_area_tab[i].x != target_area.x) && (oa->adj_area_tab[i].y != target_area.y)){}
					#ifdef DEBUG_OA
					sprintf(uart_out_buffer,"oa> area %d is forb. or out of table!\r\n",i);
					UART_PutString(&uart_out_buffer);
					#endif
				//}
			}
        }     	
    }
   
    /*we calc the dist from opponent and from target for authorized next areas
	we'll choose the shortest one from the target AND the most far from the opponent
	-(NO YET)distance from target has a coeff of 1/2
	-distance from opponent has a coeff 1
	we'll choose the smallest result[]
	*/
	
	uint16_t dist_target_opp = ABS(target_area.x - opp_area.x)*ABS(target_area.x - opp_area.x) + ABS(target_area.y - opp_area.y)*ABS(target_area.y - opp_area.y);
	uint16_t dist_me_opp = ABS(opp_area.x - my_area.x)*ABS(opp_area.x - my_area.x) + ABS(opp_area.y - my_area.y)*ABS(opp_area.y - my_area.y);
	
	if((dist_me_opp > MIN_DIST_BEFORE_IGNORING_OPP) ){ //&& (dist_me_opp > MIN_DIST_BEFORE_IGNORING_OPP)
		/*if we, and the target are too far from the opponent, we ignore the opponent for the next_area choice*/
		#ifdef DEBUG_OA
		sprintf(uart_out_buffer,"oa> opp too far!  d:%d(^1/2) check. d aj<->targ.!\r\n",dist_target_opp);
		UART_PutString(&uart_out_buffer);
		#endif
		
		for(i=0;i<3;i++){
			if( d_nxt_area_target[i] != MARKED_FORBIDDEN){
				d_nxt_area_target[i] = (int16_t)((oa->adj_area_tab[i].x - target_area.x)*(oa->adj_area_tab[i].x - target_area.x) + (oa->adj_area_tab[i].y - target_area.y)*(oa->adj_area_tab[i].y - target_area.y));
			}
		}
		
			/*sorting d_my_area_target ascending (0 shortest, 3 longest)*/
		uint8_t sorted_d_nxt_area_target[3];

		oa_sort_tab3(d_nxt_area_target,sorted_d_nxt_area_target);

		
		index0=sorted_d_nxt_area_target[0];
		index1=sorted_d_nxt_area_target[1];
		index2=sorted_d_nxt_area_target[2];
		
		#ifdef DEBUG_OA
		sprintf(uart_out_buffer,"oa> sorted areas {%d,%d}{%d,%d}{%d,%d}\r\n",
		oa->adj_area_tab[index0].x,oa->adj_area_tab[index0].y,
		oa->adj_area_tab[index1].x,oa->adj_area_tab[index1].y,
		oa->adj_area_tab[index2].x,oa->adj_area_tab[index2].y);
		UART_PutString(&uart_out_buffer);
		#endif
				
		/*electing next area (we have to choose the shortest)*/
		for(i=0;i<3;i++){
		j = sorted_d_nxt_area_target[2-i]; //var reuse
			if(d_nxt_area_target[j]!=MARKED_FORBIDDEN){
				
				next_area = oa->adj_area_tab[j];
			}
		}
	}
	else{
		if(dist_target_opp==0){ //really close
			if(oa->wait<TIMEOUT_WAIT_IN_PLACE){
				#ifdef DEBUG_OA
				sprintf(uart_out_buffer,"oa> opp in same place than target! waiting %d \r\n",oa->wait);
				UART_PutString(&uart_out_buffer);
				#endif
				
				next_area.x = 0;
				next_area.y = 0;
				oa->wait++;
			}
			else{
				/**/
				#ifdef DEBUG_OA
				UART_CPutString("oa> waiting for soooo long... going back to prev quad\r\n");
				#endif
				oa_goto_quad(oa,oa->prev_quad);
			}
		}
		else{
			#ifdef DEBUG_OA
			sprintf(uart_out_buffer,"oa> opp too close d:%d(^1/2) checking dist op<->adj\r\n",dist_me_opp);
			UART_PutString(&uart_out_buffer);
			#endif
			
			for(i=0;i<3;i++){
				if( d_nxt_area_opp[i] != MARKED_FORBIDDEN)
					d_nxt_area_opp[i] = (int16_t)((oa->adj_area_tab[i].x - opp_area.x)*(oa->adj_area_tab[i].x - opp_area.x) + (oa->adj_area_tab[i].y - opp_area.y)*(oa->adj_area_tab[i].y - opp_area.y));
			}
			
			uint8_t sorted_d_nxt_area_opp[3];
	
			oa_sort_tab3(d_nxt_area_opp,sorted_d_nxt_area_opp);
			
			index0= sorted_d_nxt_area_opp[0];
			index1= sorted_d_nxt_area_opp[1];
			index2= sorted_d_nxt_area_opp[2];
			
			#ifdef DEBUG_OA
			sprintf(uart_out_buffer,"oa> sorted areas {%d,%d}{%d,%d}{%d,%d}\r\n",
			oa->adj_area_tab[index0].x,oa->adj_area_tab[index0].y,
			oa->adj_area_tab[index1].x,oa->adj_area_tab[index1].y,
			oa->adj_area_tab[index2].x,oa->adj_area_tab[index2].y);
			
			UART_PutString(&uart_out_buffer);
			#endif
			
			//electing next area (we have to choose the longest)
			for(i=0;i<3;i++){
			j = sorted_d_nxt_area_opp[2-i];
				if(d_nxt_area_opp[j]!=MARKED_FORBIDDEN){
					
					next_area = oa->adj_area_tab[j];
				}
			}
	
		}
	}
	
	/*
	We have to take the farest from the opp, and the shortest from target
	if opp is too far(), we have to ignore it
	*/
	

	//UART_CPutString("oa>election done..\r\n");
	if((next_area.x == target_area.x) && (next_area.y == target_area.y)){
		trajectory_goto_xy_abs(oa->traj,(double)oa->target_xy.x,(double)oa->target_xy.y);	
		oa->state=OA_IN_TARGET;
	}
	else{
		if(next_area.x==0 && next_area.y==0){
			//no one has been elected, stay in place for the moment
			#ifdef DEBUG_OA
			UART_CPutString("oa> sixt: nowhere to go, going back \r\n");
			#endif
			
			next_area.x= oa->prev_area.x;
			next_area.y= oa->prev_area.y;
			
		}
		else{
			#ifdef DEBUG_OA
			sprintf(uart_out_buffer,"oa> sixt: going to area {%d,%d}\r\n",next_area.x,next_area.y);
			UART_PutString(&uart_out_buffer);
			#endif
		}
		xy_position pos;
		pos = oa_area_to_xy(next_area);
		
		/*we save our previous area, is next time we have nowhere to go..*/
		
		trajectory_goto_xy_abs(oa->traj,(double)pos.x,(double)pos.y);
	}
	oa->prev_area.x = next_area.x;
	oa->prev_area.y = next_area.y;
}

uint8_t oa_get_quad(xy_position pos){

    if(pos.x<=PLAY_AREA_MAX_X/2 && pos.y<=PLAY_AREA_MAX_Y/2)
        return QUAD_1;
    if(pos.x>=PLAY_AREA_MAX_X/2 && pos.y<=PLAY_AREA_MAX_Y/2)
        return QUAD_2;
    if(pos.x<=PLAY_AREA_MAX_X/2 && pos.y>=PLAY_AREA_MAX_Y/2)
        return QUAD_4;
    if(pos.x>=PLAY_AREA_MAX_X/2 && pos.y>=PLAY_AREA_MAX_Y/2)
        return QUAD_3;
    return 0; //out of the table ???!!!!!!
}
    
uint8_t oa_get_com_dt(xy_position pos1,xy_position pos2){
    /*
   the only thing that matter is if the return value is ==0 or !=0
    */
    if(pos1.y < DT_DC_NB_SQ_Y*SQ_SIZE_Y && pos2.y < DT_DC_NB_SQ_Y*SQ_SIZE_Y)    
        return DT_DC1;
    if(pos1.y > (MAX_Y_AREAS-DT_DC_NB_SQ_Y)*SQ_SIZE_Y && pos2.y > (MAX_Y_AREAS-DT_DC_NB_SQ_Y)*SQ_SIZE_Y)    
        return DT_DC2;
    
    if(pos1.x < DT_DL_NB_SQ_X*SQ_SIZE_X && pos2.x < DT_DL_NB_SQ_X*SQ_SIZE_X)    
        return DT_DL1;
    if(pos1.x > (MAX_X_AREAS-DT_DL_NB_SQ_X)*SQ_SIZE_X && pos2.x > (MAX_X_AREAS-DT_DL_NB_SQ_X)*SQ_SIZE_X)    
        return DT_DL2;
    
    return 0; //no common DT !
}

uint8_t oa_is_adj(uint8_t quad1,uint8_t quad2){
/*
La faÃ§on dont les quad sont numÃ©rotÃ©s fait que s'ils sont
adj, alors leur somme est impaire, sinon paire
*/    
if((quad1+quad2)%2==0) //paire
    return 0; //non-adj
else   
    return 1; //adj!
}

xy_position oa_quad_to_xy(uint8_t destquad){
    xy_position retpos;
    switch(destquad){
        
        case QUAD_1:
        retpos.x = PLAY_AREA_MAX_X/4;
        retpos.y = PLAY_AREA_MAX_Y/4;
        break;
        
        case QUAD_2:
        retpos.x = 3*PLAY_AREA_MAX_X/4;
        retpos.y = PLAY_AREA_MAX_Y/4;
        break;
        
        case QUAD_3:
        retpos.x = 3*PLAY_AREA_MAX_X/4;
        retpos.y = 3*PLAY_AREA_MAX_Y/4;
        break;
        
        case QUAD_4:
        retpos.x = PLAY_AREA_MAX_X/4;
        retpos.y = 3*PLAY_AREA_MAX_Y/4;
        break;
       
    }
	
	return retpos;

}

void oa_goto_quad(struct obstacle_avoidance * oa,uint8_t destquad){
    xy_position dest;
    dest = oa_quad_to_xy(destquad);  
    trajectory_goto_xy_abs(oa->traj,dest.x,dest.y);
}

uint8_t oa_is_in_v4(struct play_area * ar1, struct play_area  * ar2)
{
//renvoir 1 si dans le V4 0 sinon
	if (ar1->x==ar2->x)
	{
		if (abs(ar1->y - ar2->y)==1)
			return 1;
	}
	else if (ar1->y==ar2->y)
		{
			if (abs(ar1->x - ar2->x)==1)
				return 1;
		}
	return 0;
}


int8_t oa_get_no_half_quad(xy_position opp_pos,uint8_t opp_quad,uint8_t my_quad){
 float y_of_x;
 uint8_t retquad;
 int8_t sens;
 /*
    if(opp_quad%2==0){ //paire (2 ou 4)
        y_of_x = (-PLAY_AREA_MAX_Y/PLAY_AREA_MAX_X)*opp_pos.x + PLAY_AREA_MAX_Y;
    	if(y_of_x>0){
		retquad = QUAD_1;
		else retquad = QUAD_3;
	}
	else{
        y_of_x = (PLAY_AREA_MAX_Y/PLAY_AREA_MAX_X)*opp_pos.x;
		if(y_of_x>0) retquad = QUAD_2;
		else retquad = QUAD_4;
   }
   */
   
   if(opp_quad%2==0){ //paire (2 ou 4)
        y_of_x = (-PLAY_AREA_MAX_Y/PLAY_AREA_MAX_X)*opp_pos.x + PLAY_AREA_MAX_Y;
    	if(y_of_x>0) sens = 1;
		else sens= -1;
	}
	else{
        y_of_x = (PLAY_AREA_MAX_Y/PLAY_AREA_MAX_X)*opp_pos.x;
		if(y_of_x>0) sens = 1;
		else sens = -1;
   }
   
   if(my_quad == 1 || my_quad == 4) return ((my_quad+3-sens)%4+1);
   else return ((my_quad+3+sens)%4+1);

}

uint8_t oa_get_sym_quad(uint8_t quad){
	uint8_t ret;
    switch(quad){
        case QUAD_1:
            ret = QUAD_3;
        break;
		
        case QUAD_2:
            ret = QUAD_4;
        break;    
        
        case QUAD_3:
            ret = QUAD_1;
        break;    
        
        case QUAD_4:
            ret = QUAD_2;
        break;        
    }
	return ret;
}

uint8_t oa_is_out_of_table(play_area area){
    if(ABS(area.x)>MAX_X_AREAS || ABS(area.y)>MAX_Y_AREAS || area.x==0 || area.y==0)
        return 1;
    else
        return 0;    
}

void oa_set_traj(struct obstacle_avoidance * oa,struct trajectory * traject){
	oa->traj = traject;
}

uint8_t oa_do_i_need_to_stop(struct obstacle_avoidance * oa){
	/*ir detected something... do we neeed to stop ???*/
	struct vect2_cart cart_opp_rel, cart_opp_abs;
	struct vect2_pol pol;
	
	pol.r = 20; // if we've detected it, it's 20cm far only
	pol.theta = position_get_a_rad_double(oa->traj->position); 
	
	//we get the cart from the pol and see if the cart is an element or outside of the table
	vect2_pol2cart(&pol,&cart_opp_rel);
	
	//now we got a relative position... let's check the absolute one..
	cart_opp_abs.x = cart_opp_rel.x +  position_get_x_double(oa->traj->position);
	cart_opp_abs.y = cart_opp_rel.y +  position_get_y_double(oa->traj->position);
	
	//sprintf(uart_out_buffer,"oa> Opp @ x %d,y%d \r\n",(int16_t)cart_opp_abs.x,(int16_t)cart_opp_abs.y);
	//UART_PutString(uart_out_buffer);
	if(cart_opp_abs.x > 150 || cart_opp_abs.x < 50|| cart_opp_abs.y > 250 || cart_opp_abs.y < 50)
		return 0;
	else
		return 1; //got to avoid it !!!
}


void oa_manage(struct obstacle_avoidance *oa){
   	
	xy_position my_pos_xy;
	my_pos_xy.x = position_get_x_s16(oa->traj->position);
	my_pos_xy.y = position_get_y_s16(oa->traj->position);
	uint8_t my_quad = oa_get_quad(my_pos_xy);
	
   	if(oa->state==OA_TARGET_SET && oa->traj->state==READY){ //new target where to go !
   		
		uint8_t opp_quad = oa_get_quad(oa->opponent_xy);
		uint8_t target_quad = oa_get_quad(oa->target_xy);
	
		//oa->state==OA_PROCESSING;
		uint8_t tmpquad;
		#ifdef DEBUG_OA
    	UART_CPutString("oa> processing new target\r\n");
		#endif
		
		if(oa_get_com_dt(my_pos_xy,oa->target_xy)!=0){  //!=0
			#ifdef DEBUG_OA
			UART_CPutString("oa> same dt than target\r\n");
			#endif
		
			if(oa_get_com_dt(my_pos_xy,oa->opponent_xy)!=0){
				#ifdef DEBUG_OA
				UART_CPutString("oa> same dt than opp-> sixt\r\n");
				#endif
				oa_sixt(oa);
			}
			else{
				#ifdef DEBUG_OA
				UART_CPutString("oa> not same dt than opp ->xy!\r\n");
				#endif
				trajectory_goto_xy_abs(oa->traj,oa->target_xy.x,oa->target_xy.y);
				oa->state=OA_IN_TARGET;
			}
		}
		
		else{ //==0
			#ifdef DEBUG_OA
			UART_CPutString("oa> not in same dt than target\r\n");
			#endif
	
			#ifdef DEBUG_OA
			sprintf(uart_out_buffer,"oa> quads: me %d opp %d targ %d\r\n",my_quad,opp_quad,target_quad);
			UART_PutString(&uart_out_buffer);
			#endif
			
			if(oa_is_adj(my_quad,opp_quad)){ //me & opp are adj
				#ifdef DEBUG_OA
				UART_CPutString("oa> I'm adj to opponent\r\n");
				#endif
				
				if(my_quad==target_quad){ //my_quad==target_quad
					#ifdef DEBUG_OA
					UART_CPutString("oa> my_qd=tg_qd->my_qd center\r\n");
					#endif
					oa_goto_quad(oa,my_quad);
				}
				else{ //my_quad!=target_quad
					#ifdef DEBUG_OA
					UART_CPutString("oa> my_qd!=tg_qd\r\n");
					#endif
					
					if(target_quad==opp_quad){ //target_quad==opp_quad
						#ifdef DEBUG_OA
						UART_CPutString("oa> tg_qd=op_qd->!opp half\r\n");
						#endif
						
						tmpquad = oa_get_no_half_quad(oa->opponent_xy,target_quad,my_quad);
						oa_goto_quad(oa,tmpquad); 
						
						#ifdef DEBUG_OA
						sprintf(uart_out_buffer,"oa> !opp half is qd %d\r\n",tmpquad);
						UART_PutString(&uart_out_buffer);
						#endif
					}
					else{//target_quad!=opp_quad
						#ifdef DEBUG_OA
						UART_CPutString("oa> tg_qd!=op_qd->sym opp quad (but first, my quad)\r\n");
						#endif
						//   
		/*last modified stuff: before: oa_goto_quad(oa,oa_get_sym_quad(opp_quad)); only..*/				
						if(!(oa->flags & OA_FLAG_GOSYMQD_PART1_DONE)){
							setBit(oa->flags,OA_FLAG_GOSYMQD_PART1_DONE);
							#ifdef DEBUG_OA
							UART_CPutString("oa> my quad...\r\n");
							#endif
							oa_goto_quad(oa,my_quad); 
							}
						else {
							clearBit(oa->flags,OA_FLAG_GOSYMQD_PART1_DONE);
							#ifdef DEBUG_OA
							UART_CPutString("oa> opp sym quad...\r\n");
							#endif
							oa_goto_quad(oa,oa_get_sym_quad(opp_quad));
						}
						
					}
				}
					
			}
			else{  //me & opp aren't adj
				#ifdef DEBUG_OA
				UART_CPutString("oa> I'm not adj to opponent\r\n");
				#endif
				
				if(my_quad==opp_quad){ //my_quad==opp_quad
					#ifdef DEBUG_OA
					UART_CPutString("oa> my_qd=opp_qd->sixt\r\n");
					#endif
					oa_sixt(oa); //apply the celebre "Sixt()" algorithm..
				}
				else{ // !my_quad==opp_quad
					#ifdef DEBUG_OA
					UART_CPutString("oa> my_qd!=opp_qd\r\n");
					#endif
					
					if(target_quad==opp_quad){ //target_quad==opp_quad
						#ifdef DEBUG_OA
						UART_CPutString("oa> tg_qd=opp_qd->!opp half\r\n");
						#endif 
						//tmpquad = (my_quad-1+oa_get_no_half_quad(oa->opponent_xy,target_quad))%4+1;
						tmpquad = oa_get_no_half_quad(oa->opponent_xy,target_quad,my_quad);
						
						//oa_goto_quad(oa,tmpquad); 
						if(!(oa->flags & OA_FLAG_GOOPPQD_PART1_DONE)){
							setBit(oa->flags,OA_FLAG_GOOPPQD_PART1_DONE);
							#ifdef DEBUG_OA
							UART_CPutString("oa> my quad...\r\n");
							#endif
							oa_goto_quad(oa,my_quad); 
							}
						else {
							clearBit(oa->flags,OA_FLAG_GOOPPQD_PART1_DONE);
							#ifdef DEBUG_OA
							UART_CPutString("oa> !opp quad...\r\n");
							#endif
							oa_goto_quad(oa,tmpquad);
						}
						
						//-1 because we don't want to go to the same half quad than the opponent
						#ifdef DEBUG_OA
						sprintf(uart_out_buffer,"oa> !opp half is qd %d\r\n",tmpquad);
						UART_PutString(&uart_out_buffer);
						#endif
					}
					else{ // !target_quad==opp_quad
						#ifdef DEBUG_OA
						UART_CPutString("oa> tg_qd!=opp_qd->tg qd\r\n");
						#endif
						oa_goto_quad(oa,target_quad); //0 middle, +1 upper half, -1 lower half
					}
				}
			}
		
		}
	}
	
	/*
	//we calc the dist between me and the opp.
	oa->curr_dist_me_opp =(int16_t)(abs(my_pos_xy.x - oa->opponent_xy.x)*abs(my_pos_xy.x - oa->opponent_xy.x)+abs(my_pos_xy.y - oa->opponent_xy.y)*abs(my_pos_xy.y - oa->opponent_xy.y));
	
	//if we are getting closer, and we are tooo close, we just stop, wait, 
	if((oa->curr_dist_me_opp < oa->prev_dist_me_opp) && (oa->curr_dist_me_opp < MIN_COLLISION_DIST)){
		#ifdef DEBUG_OA
		sprintf(uart_out_buffer,"oa> opp @ %d(^1/2) cm !!!! \r\n",oa->curr_dist_me_opp);
		UART_PutString(&uart_out_buffer);
	#endif
			
	
		trajectory_hardstop(oa->traj);
		oa->state=OA_COLLISION_AVOIDED;
		oa->timeout=gettime();
		
	}
	
	if(oa->state==OA_COLLISION_AVOIDED && (gettime()-oa->timeout)>COLLISION_AVOIDED_TIMEOUT ){
	//hope nothing's broken...
	#ifdef DEBUG_OA
	UART_CPutString("oa> collision avoided ?\r\n");
	#endif
	
	oa->state=OA_TARGET_SET;
	oa->traj->state=READY;
	oa->timeout = 0;
	}
*/
	oa->prev_dist_me_opp = oa->curr_dist_me_opp;
	oa->prev_quad = my_quad;
}

