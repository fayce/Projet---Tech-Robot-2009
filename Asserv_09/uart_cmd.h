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

#ifndef _UART_CMD_H_
#define _UART_CMD_H_

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "string.h"
#include "flash.h"
#include "i2c_master.h"
#include "robot.h"
#include "time_machine.h"
#include "obstacle_avoidance.h"
#include "strat.h"

#ifdef UART_CONF_ROBOT
	#define UART_MAX_CMD 10
#endif

#ifndef UART_CONF_ROBOT
	#define UART_MAX_CMD 16
#endif

#define UART_MAX_CMD_CHAR 12
#define UART_MAX_CMD_ARGS 4
#define UART_MAX_BUFFER_SIZE 32

typedef void (*uart_func)(uint8_t * str);


uint8_t ret[UART_MAX_BUFFER_SIZE];
uint16_t uart_u16_buff[UART_MAX_CMD_ARGS];
int16_t uart_s16_buff[UART_MAX_CMD_ARGS];



void uart_welcome(void);
void uart_prompt(void);
void uart_check(void);

uint8_t uart_checkcmd(uint8_t *cmd);



void uart_setpid(uint8_t *str);
void uart_getpid(uint8_t *str);
void uart_dotest(uint8_t *str);

void uart_getstate(uint8_t *str);
void uart_getpos(uint8_t *str);

void uart_stop(uint8_t *str);
void uart_pwm(uint8_t *str);

void uart_signal(uint8_t *str);
void uart_strat(uint8_t *str);

void uart_setpidshift(uint8_t *str);
void uart_getpidshift(uint8_t *str);

void uart_debug_robot(uint8_t *str);

void uart_setpidmax(uint8_t *str);
void uart_getpidmax(uint8_t *str);

void uart_setspeed(uint8_t *str);
void uart_getspeed(uint8_t *str);
//void uart_help(int8_t *str);
void uart_target(uint8_t *str);


void uart_goxy(uint8_t *str);
void uart_gotoda(uint8_t *str);
//void uart_setphyparam(int8_t *str);
void uart_i2c(uint8_t *str);

void uart_opp(uint8_t *str);

static const uart_func uart_func_tab[UART_MAX_CMD] = {
/*#ifdef UART_CONF_ROBOT
(uart_func) uart_setpid,
(uart_func) uart_getpid,
(uart_func) uart_dotest,
#endif
*/
(uart_func) uart_getpos,
(uart_func) uart_target,
(uart_func) uart_opp,
(uart_func) uart_signal,
(uart_func) uart_strat,
(uart_func) uart_debug_robot,
/*
#ifdef UART_CONF_ROBOT
(uart_func) uart_setspeed,
(uart_func) uart_getspeed,
(uart_func) uart_pwm,
#endif
*/
(uart_func) uart_goxy,
(uart_func) uart_gotoda,
(uart_func) uart_i2c

};



#endif