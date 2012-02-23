/*  
 *  Copyright Droids Corporation, Microb Technology, Eirbot (2005), Projet & Tech' (2008-2009)
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
 *  Olivier MATZ <zer0@droids-corp.org> 
 *	Fayçal GUENNAR <f.guennar@gmail.com>
 */
 
#include "pid.h"


/** initialisation de la struct a 0 */
void pid_init (struct pid_filter * p)
{
  //dgi();
    memset(p, 0, sizeof(*p));
    p->gain_P = 1 ;
  //egi();
}

void pid_set_gains(struct pid_filter *p, int32_t gp, int32_t gi, int32_t gd)
{
  //dgi();
    p->gain_P  = gp;
    p->gain_I  = gi;
    p->gain_D  = gd;
  // egi();
}

void pid_set_maximums(struct pid_filter *p, int32_t max_in, int32_t max_I, int32_t max_out)
{
// dgi();
    p->max_in  = max_in;
    p->max_I   = max_I;
    p->max_out = max_out;
//  egi();
}

void pid_set_out_shift(struct pid_filter *p, uint8_t out_shift)
{
 // dgi();
    p->out_shift=out_shift;
 //  egi();
}

int32_t pid_get_gain_P(struct pid_filter *p)
{
	return (p->gain_P);
}

int32_t pid_get_gain_I(struct pid_filter *p)
{
	return (p->gain_I);
}

int32_t pid_get_gain_D(struct pid_filter *p)
{
	return (p->gain_D);
}


int32_t pid_get_max_in(struct pid_filter *p)
{
	return (p->max_in);
}

int32_t pid_get_max_I(struct pid_filter *p)
{
	return (p->max_I);
}

int32_t pid_get_max_out(struct pid_filter *p)
{
	return (p->max_out);
}


uint8_t pid_get_out_shift(struct pid_filter *p)
{
	return (p->out_shift);
}


int32_t pid_get_value_I(struct pid_filter *p)
{
	int32_t ret; 
	dgi();
	ret = (p->integral);
	egi();
	return ret;
}

int32_t pid_get_value_D(struct pid_filter *p){
int32_t ret; 
	dgi();
	ret = p->prev_D;
	egi();
	return ret;
}

int32_t pid_get_value_out(struct pid_filter *p){
int32_t ret; 
	dgi();
	ret = (p->prev_out);
	egi();
	return ret;
}


/* le premier param doit etre une structure pid_filter *) */

int32_t pid_do_filter(void * data, int32_t in)
{
    int32_t derivate ;
    int32_t command ;
    struct pid_filter * p = data;

    /*
     * integral = integral precedente + valeur actuelle
     */

    /* derivee                                                             */
    /*             f(t+h) - f(t)        avec f(t+h) = valeur actuelle  */
    /*  derivee = -------------              f(t)   = valeur precedente */
    /*                    h                                          */
    /* donc derivee = erreur actuelle - erreur precedente                  */
    derivate = in - p->prev_in ;

    /* saturation de l'entree... cela influence l'integrale */
    if (p->max_in)
        S_MAX(in, p->max_in) ;

    p->integral += in ;

    if (p->max_I)
        S_MAX(p->integral, p->max_I) ;

    /* la commande = P.coef_P + I.coef_I + D.coef_D */
     command =  (int32_t)(in * p->gain_P) +
        (int32_t)(p->integral * p->gain_I) +
        (int32_t)(derivate * p->gain_D) ;



    if ( command < 0 )
        command = -( -command >> p->out_shift );
    else
        command = command >> p->out_shift ;

    if (p->max_out)
        S_MAX (command, p->max_out) ;


    // sauvegarde de l'erreur actuelle (pour le prochain calcul de l'erreur)
    p->prev_in = in ;
    p->prev_D = derivate ;
    p->prev_out = command ;

    return command;
}
