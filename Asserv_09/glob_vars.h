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
 
#ifndef _GLOB_VARS_H
#define _GLOB_VARS_H

#include "types.h"
#include <htc.h>


//debug
//#define DEBUG_STRAT
//#define DEBUG_OA
//#define NO_MEM_LEFT

//My leds
#define LED_1 0x02 //1

#define LED_3 0x40 //7 0x01 0x02 0x04 0x08 0x10 0x20 0x40 0x80

// macros
#define setBit( MSKN_REG, MASK )    (MSKN_REG |= MASK)
#define clearBit( MSKN_REG, MASK )   (MSKN_REG &= ~MASK)

//Global Flags
#define FLAG_HANDLE_JACK 0x01
#define FLAG_JACK_OUT 0x02
#define FLAG_FAYCE_FORGOT_TO_PLUG_JACK 0x04
//debug config
#define ALL_STRUCT_DEBUG

//Some ctes
#define M_PI 3.14159265358979323846

//aliases
#define PROCESS_DISTANCE 1
#define PROCESS_ANGLE 2

//default PID Config 
#define PIDA_P 100
#define PIDA_I 0
#define PIDA_D 800
#define PIDA_SH 4

#define PIDD_P 70
#define PIDD_I 0
#define PIDD_D 700
#define PIDD_SH 4


//Quadramp speeds
#define QR_SPEED_A_POS_FAST 800
#define QR_SPEED_A_NEG_FAST 800

#define QR_SPEED_A_NEG_SLOW 50
#define QR_SPEED_A_POS_SLOW 50
#define QR_ACC_A_NEG 2000
#define QR_ACC_A_POS 2000

#define QR_SPEED_D_POS_FAST 60 
#define QR_SPEED_D_NEG_FAST 60
#define QR_SPEED_D_NEG_SLOW 20
#define QR_SPEED_D_POS_SLOW 20
#define QR_SPEED_D_NEG_VERY_SLOW 10
#define QR_SPEED_D_POS_VERY_SLOW 10
#define QR_ACC_D_NEG 4000
#define QR_ACC_D_POS 4000

//UART CFG
#define UART_VERBOSE
#define UART_MAX_ARGS 3
#define UART_MAX_ARG_CHAR 10
#define UART_BUFFER_SIZE 32

//Pin Aliases
#define C1CHB 0x02
#define C2CHB 0x08

#define PWM1_A 0x02
#define PWM1_B 0x04
#define PWM2_A 0x08
#define PWM2_B 0x10

#define LED1 0x01
#define MAX_PID_SAMPLES 100

//Positiion initiale du robot
#define POS_R_INIT_X 50   //unit is cm !
#define POS_R_INIT_Y 250
#define POS_R_INIT_A -90

#define POS_G_INIT_X 50
#define POS_G_INIT_Y 50
#define POS_G_INIT_A 90


//Config robot
#define ENCODER_RESOLUTION 1024
#define TRACK_MM 344  //344
#define TRACK_CM TRACK_MM/10
#define RAYON_ROUE_CODEUSE_MM 28
#define DIST_IMP_PER_MM ENCODER_RESOLUTION/(2*M_PI*RAYON_ROUE_CODEUSE_MM) //5,82...
#define DIST_IMP_PER_CM DIST_IMP_PER_MM*10
#define TRACK_TOP TRACK_MM*DIST_IMP_PER_MM //

//Traj state
#define TRAJ_STATE_READY 1
#define TRAJ_STATE_RUNNING 2
//Mr Jack States
#define JACK_IN 1
#define JACK_OUT 2
//Glob vars
extern int32_t TopG,TopD;
extern uint8_t bError;

extern uint8_t uart_out_buffer[32];
extern int8_t GlobalFlags;
extern uint8_t glob_traj_state;

//Strat vars
extern uint8_t Strat_Action;
extern uint8_t Strat_State;


void init_globals(void);

#endif
