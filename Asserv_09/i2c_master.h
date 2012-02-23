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
#ifndef _I2C_MASTER_H_
#define _I2C_MASTER_H_
#include "types.h"
#include "robot.h"

#define I2C_TIMEOUT 250


//I2C Commands list

/*Tapis rouleaux board*/

#define I2C_TAPIS_BOUFFE 1 
#define I2C_TAPIS_GET_NB_ELTS_IN 5
#define I2C_TAPIS_STOCK 6  //il n'est censÃ© en stocker qu'un seul
#define I2C_TAPIS_EJECT 2 
#define I2C_TAPIS_PROTECT 4
#define I2C_TAPIS_STOP 0


//To tapis-roulo
#define I2C_SET_COLOR_GREEN 70
#define I2C_SET_COLOR_RED 71

//To Lint
#define I2C_LINT_GET_DOWN 1
#define I2C_LINT_GET_UP 2
#define I2C_LINT_DEPOSIT_DOWN 3
#define I2C_LINT_DROP 4
#define I2C_GOT_SMTG 5 // 1 no :( / 2: yes !!!

//To pince
#define I2C_PINCE_PREPARE 1
#define I2C_PINCE_CONSTRUCT 2
#define I2C_PINCE_DROP 3
#define I2C_PINCE_DID_YOU_BUILD_SMTG 4    //answer: 1 ok, 0 not yet, answer 2: cannot :(

//To IR sensors
#define I2C_IR_CHECK_IN_FRONT 3
//////////////////////////////////////////////
//My I2C Slave Muahahahah
#define I2C_TAPIS_ROULEAUX_ADDR 5
#define I2C_LINTO_ADDR 3
#define I2C_PINCE_ADDR 4
#define I2C_CAPTEUR_IR_ADDR 8

void init_i2c_master(void);

int8_t i2c_send_and_receive(int8_t slave_addr, int8_t Commande);
void i2c_send_only(int8_t slave_addr, int8_t Cmd);
void i2c_getposr(struct robot *bot);

////Cmd's func

#endif