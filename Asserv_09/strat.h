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
 
 #ifndef STRAT_H
 #define STRAT_H
 
#include "types.h"
#include "obstacle_avoidance.h"
#include "trajectory_manager.h"
#include "i2c_master.h"
#include "PSoCAPI.h"
#include "glob_vars.h"
#include "robot.h"


#define NB_ELEMENTS_ON_TABLE 6
#define NB_ELEMENTS_IN_DISP 10
#define NB_LINTELS 4

/*
                          (GD_R2)   
	o----------------------------------------o
	|        |         O  .  O               |
	|   R    |      (GD_R1)                 O|(GD_F)
	|        |                               |
	|--------  .P1  .    .   .P2             |
	|                                1b4-----|
RL1 |-----     .P4  .    .   .P3        -----|
	|                                1b3-----|[BA1_R]
	|        P5.    .    .   .P6        -----| 
    |                                        |
RL2-|-----             __  3b3       2b2-----|
	|                 /       \         -----|
	|              3b4 |[BA3] |3b2   2b1-----|[BA2]
    |                 \_______/         -----|
GL2-|-----            3b1                    |
	|                                        |
	|         P5.    .    .   .P6    1b2-----|
	|                                   -----|[BA1_G]
GL1 |-----    P4.    .    .   .P3    1b1-----|    
	|                                   -----|
	|---------  .P1  .    .   .P2            |
y	|         |                              |
^	|    G    |      (RD_R1)                O|(RD_F)
|	|         |         O  .  O              |
|	o----------------------------------------o
|                           (RD_R2)
0------>x
	

*/



/*Margins*/
#define LINT_DISP_MARGIN 30
#define ELT_SHORT_SIDE MARGIN 30
#define ELT_LONG_SIDE MARGIN 30

/*on table elements*/
#define ON_TABLE_ELT_R1_X 60
#define ON_TABLE_ELT_R1_Y 240

#define ON_TABLE_ELT_G1_X 60
#define ON_TABLE_ELT_G1_Y 60

/* Vertical dispensers*/
//Fixed pos (long side)
#define R_ELT_DISP_FIXED_X 180
#define R_ELT_DISP_FIXED_Y 25

#define G_ELT_DISP_FIXED_X 180
#define G_ELT_DISP_FIXED_Y 275

#define ELT_DISP_IN_OUT_DIST 10
//Rand pos (short side)
#define R_ELT_DISP_RAND_1_X 80
#define R_ELT_DISP_RAND_1_Y 30
#define R_ELT_DISP_RAND_2_X 130
#define R_ELT_DISP_RAND_2_Y 30

#define G_ELT_DISP_RAND_1_X 80
#define G_ELT_DISP_RAND_1_Y 270
#define G_ELT_DISP_RAND_2_X 130
#define G_ELT_DISP_RAND_2_Y 270

/* Lintel dispensers*/
#define R_LINT_DISP_1_X LINT_DISP_MARGIN  //30
#define R_LINT_DISP_1_Y 210
#define R_LINT_DISP_2_X LINT_DISP_MARGIN
#define R_LINT_DISP_2_Y 170

#define G_LINT_DISP_1_X LINT_DISP_MARGIN
#define G_LINT_DISP_1_Y 90
#define G_LINT_DISP_2_X LINT_DISP_MARGIN
#define G_LINT_DISP_2_Y 130

/*Build areas positions*/
#define BUILD_AREA_1b1_X 195
#define BUILD_AREA_1b1_Y 85
#define BUILD_AREA_1b2_X 190
#define BUILD_AREA_1b2_Y 110
#define BUILD_AREA_1b3_X 195
#define BUILD_AREA_1b3_Y 183.5
#define BUILD_AREA_1b4_X 195
#define BUILD_AREA_1b4_Y 196.5

#define BUILD_AREA_2b1_X 190
#define BUILD_AREA_2b1_Y 143.5
#define BUILD_AREA_2b2_X 190
#define BUILD_AREA_2b2_Y 170

#define BUILD_AREA_3b1_X 105 - 30
#define BUILD_AREA_3b1_Y 110
#define BUILD_AREA_3b2_X 105 + 30
#define BUILD_AREA_3b2_Y 300 - BUILD_AREA_3b1_Y


/*Strat states..*/

#define STRAT_GET_ELT_ON_TABLE 1
#define STRAT_CONSTRUCT_TEMPLE_3 2
#define STRAT_GET_LINT_1 3
#define STRAT_GET_LINT_2 4
#define STRAT_GET_ELT_ON_FIXED_DISP 5
#define STRAT_GET_ELT_RAND_DISP_POS 6
#define STRAT_GET_ELT_ON_RAND_DISP_1 7
#define STRAT_GET_ELT_ON_RAND_DISP_2 8
#define STRAT_CONSTRUCT_TEMPLE_2_1 9
#define STRAT_CONSTRUCT_TEMPLE_2_2 10
#define STRAT_CONSTRUCT_TEMPLE_1_1 11
#define STRAT_CONSTRUCT_TEMPLE_1_2 12
#define STRAT_CONSTRUCT_TEMPLE_1_3 13
#define STRAT_CONSTRUCT_TEMPLE_1_4 14
#define STRAT_EMERGENCY_BEHAVIOUR 18
#define STRAT_EXIT 99

			
//Delays and time
#define END_MATCH_TIME 84000

#define DELAY_PINCE_CONSTRUCTION 10000 //on donne 10sec Ã  la pince pour construire

#define DELAY_TAPIS_EJECT 3000 //une ejection total prend 5sec
#define DELAY_TAPIS_BOUFFE 4000 // le tapis met 4sec Ã  bouffer
#define DELAY_TAPIS_STOCK 2000 //le temps que Ã§a met Ã  tout envoyer derriere lui

#define DELAY_LINT_UP 1000 //un servo c'est rapide .. 
#define DELAY_LINT_DOWN 3000 //3 sec avant de reculer leeeeeentement.

//some aliases
#define ELT_LOCATION_RAND1 1
#define ELT_LOCATION_RAND2 2
#define ELT_LOCATION_FIXED 3

#define LINT_DISP_L1 1
#define LINT_DISP_L2 2

#define STORAGE_UNLOCKED 0 
#define STORAGE_LOCKED 1


//Vars
uint8_t rand_disp_position;


//flags
#define I_CAN_BUILD_A_TEMPLE 0x01
#define LINT_IS_LOADED 0x02
#define TWO_ELTS_LOADED 0x04
#define PINCE_IS_SHITTY 0x80

//Building area
#define BUILD_AREA_3 1
#define BUILD_AREA_2_1 2
#define BUILD_AREA_2_2 3

#define BUILD_AREA_1_1 4
#define BUILD_AREA_1_2 5
#define BUILD_AREA_1_3 6
#define BUILD_AREA_1_4 7

int8_t strat_flags;
int8_t current_building_area;

struct strategy{
	uint8_t flags; 
	uint8_t loaded_elts;

};
struct strategy beudastrat;

 void strat_init(struct strategy * strat);
 int8_t strat_store_elt(uint8_t nb_elements);
 void strat_go_get_elts_on_table(struct strategy * strat);
 
 
 void strat_construct_temple(struct strategy * strat,uint8_t build_area,uint8_t half);
 
 //void strat_construct_temple_2(struct strategy * strat,uint8_t area);
 //void strat_construct_temple_1(struct strategy * strat,uint8_t area);
 
 void strat_go_get_elts_disp(struct strategy *strat,int8_t type);
 void strat_go_get_fixed_lint(struct strategy * strat,uint8_t position);
 //void strat_go_get_elts_disp(struct strategy *strat,uint8_t location);
 void strat_get_elt_rand_disp_position(void);
 void strat_manage(struct strategy * strat);
void strat_do_emergency_behaviour(void);
void strat_go_next_build_area(void);

int8_t strat_get_closest_build_area(void);
 #endif