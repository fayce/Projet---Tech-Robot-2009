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
 
 #ifndef UART_MIRR
 #define UART_MIRR
 
#include <stdlib.h>
#include "string.h"

 #include "PSoCAPI.h"
 #include "types.h"
 #include "obstacle_avoidance.h"

#define LED_2 0x08 //3

 #define UART_MIRR_MAX_CMD 3
 #define UART_MIRR_MAX_CMD_CHAR 8
 
 int16_t uart_mirr_s16_buff[2];
 
 typedef void (*uart_mirr_func)(uint8_t * str);
 
void uart_setconstr(uint8_t *str);
void uart_setposa(uint8_t *str);
 
 void uart_mirr_check(void);
 uint8_t uart_mirr_checkcmd(uint8_t *cmd);
 
 static const uart_mirr_func uart_mirr_func_tab[UART_MIRR_MAX_CMD] = {
(uart_mirr_func) uart_setposa,
(uart_mirr_func) uart_setconstr
};

 
 #endif