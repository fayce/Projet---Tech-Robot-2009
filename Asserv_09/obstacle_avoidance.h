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
 
 #ifndef OBSTACLE_AVOIDANCE
 #define OBSTACLE_AVOIDANCE

#include "trajectory_manager.h"
#include "time_machine.h"

#define PLAY_AREA_MAX_X 210
#define PLAY_AREA_MAX_Y 300

#define MAX_X_AREAS 5
#define MAX_Y_AREAS 7

#define SQ_SIZE_X 42//PLAY_AREA_MAX_X/MAX_X_AREAS  //42
#define SQ_SIZE_Y 42.8586//PLAY_AREA_MAX_Y/MAX_Y_AREAS  //42.8586868493

#define FB_AREA_0_X 3   //Build area 3
#define FB_AREA_0_Y 4

#define FB_AREA_1_X 5   //Build area 1
#define FB_AREA_1_Y 2

#define FB_AREA_2_X 5   //Build area 1 2
#define FB_AREA_2_Y 3

#define FB_AREA_3_X 5   //Build area 2
#define FB_AREA_3_Y 4

#define FB_AREA_4_X 5   //Build area 1 3
#define FB_AREA_4_Y 5

#define FB_AREA_5_X 5   //Build area 1 4
#define FB_AREA_5_Y 6

#define FB_AREA_6_X 2   //Container GD_Rand1
#define FB_AREA_6_Y 7

#define FB_AREA_7_X 4   //Container GD_Rand2
#define FB_AREA_7_Y 7

#define FB_AREA_8_X 2   //Container RD_Rand1
#define FB_AREA_8_Y 1

#define FB_AREA_9_X 4   //Container RD_Rand2
#define FB_AREA_9_Y 1

#define FB_AREA_10_X 5   //Container RD_Fixed
#define FB_AREA_10_Y 1

#define FB_AREA_11_X 5   //Container GD_Fixed
#define FB_AREA_11_Y 7

#define OPPONENT_X 190
#define OPPONENT_Y 70

#define MYPOS_X 35
#define MYPOS_Y 200

#define DT_DC_NB_SQ_X 5 //une DC fait 5sq de long (en x)
#define DT_DC_NB_SQ_Y 3 //une DC fait 5sq de long (en y)

#define DT_DL_NB_SQ_X 2
#define DT_DL_NB_SQ_Y 7

#define DT_DC1 1
#define DT_DC2 2
#define DT_DL1 3
#define DT_DL2 4

//Quadrans definition
#define QUAD_1 1
#define QUAD_2 2
#define QUAD_3 3
#define QUAD_4 4

#define QUAD_UPPER_HALF 1
#define QUAD_LOWER_HALF -1
#define QUAD_Y_DIST_FROM_HALF 20 //cm

#define NB_FORBIDDEN_AREAS 12
//#define ABS(x) ((x) < 0 ? - (x) : (x))
#define MIN_DIST_BEFORE_IGNORING_OPP 1 //1squares
#define MARKED_FORBIDDEN -1
#define MIN_COLLISION_DIST 2500 //40cm^2
#define COLLISION_AVOIDED_TIMEOUT 1000 // 800ms
#define TIMEOUT_WAIT_IN_PLACE 5 

//Flags
#define OA_FLAG_GOSYMQD_PART1_DONE 0x01
#define OA_FLAG_GOOPPQD_PART1_DONE 0x02
/*
Quad representation
o-----------o
|     |     |  
|  Q4 | Q3  |
|_____|_____|
|     |     |
|  Q1 | Q2  |
|     |     |
o-----------o
*/
#include <string.h>
typedef struct xy_position{
        int16_t x;
        int16_t y;        
}xy_position;

typedef struct play_area{
       int8_t x; //must be signed to detect out of table adj suggestions
       int8_t y;       
}play_area;

enum oa_state {
    OA_READY_NO_TARGET,
	OA_TARGET_SET,
	OA_IN_TARGET,
	OA_COLLISION_AVOIDED,
	OA_PROCESSING,
};

struct obstacle_avoidance{
 	enum oa_state state;
	struct trajectory * traj;    /* gives acces to: traj states, robot positions */
	xy_position opponent_xy;
	xy_position target_xy;
    play_area forbidden_area[NB_FORBIDDEN_AREAS]; /*central build area and the other build areas...*/
    play_area adj_area_tab[3];
	play_area prev_area;
	uint8_t prev_quad;
	uint16_t prev_dist_me_opp;
	uint24_t timeout ;
	int8_t flags;
	uint8_t wait; 
	int16_t curr_dist_me_opp;
};


struct obstacle_avoidance beudaoa;

/*init structure*/
void oa_init(struct obstacle_avoidance * oa);


/*Useful conversion*/
play_area oa_xy_to_area(int16_t x,int16_t y);
xy_position oa_area_to_xy(play_area area);
xy_position oa_quad_to_xy(uint8_t destquad);
//float vect2_norm(float x1,float x2,float y1,float y2);

/*give next possible area, depending on obstacles*/
play_area oa_next_area_to(int16_t x,int16_t y);

void oa_set_target(struct obstacle_avoidance * oa,int16_t x,int16_t y);
void oa_set_traj(struct obstacle_avoidance * oa,struct trajectory * traject);
void oa_set_opp(struct obstacle_avoidance * oa,int16_t x,int16_t y);

void oa_manage(struct obstacle_avoidance * oa);
void oa_find_adj_areas_to_target(struct obstacle_avoidance * oa,play_area * my_area,play_area * target_area);  //retourne le nb de pos
void oa_sixt(struct obstacle_avoidance * oa);

uint8_t oa_get_quad(xy_position pos);
uint8_t oa_get_com_dt(xy_position pos1,xy_position pos2);
int8_t oa_get_no_half_quad(xy_position pos,uint8_t quad,uint8_t myquad);
uint8_t oa_get_sym_quad(uint8_t quad);

uint8_t oa_is_in_v4(struct play_area * ar1, struct play_area  * ar2);

uint8_t oa_is_adj(uint8_t quad1,uint8_t quad2);
uint8_t oa_is_out_of_table(play_area area);

void oa_sort_tab3(int16_t * tabtosort,uint8_t * sortedindex);
uint8_t oa_do_i_need_to_stop(struct obstacle_avoidance * oa);
void oa_goto_quad(struct obstacle_avoidance * oa,uint8_t destquad);

#endif
