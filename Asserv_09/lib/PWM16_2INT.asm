;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME: PWM16_2INT.asm
;;   Version: 2.5, Updated on 2009/3/31 at 12:6:28
;;  Generated by PSoC Designer 5.0.423.0
;;
;;  DESCRIPTION: PWM16 Interrupt Service Routine
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress MicroSystems 2001-2003. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

include "m8c.inc"
include "PWM16_2.inc"
include "memory.inc"


;-----------------------------------------------
;  Global Symbols
;-----------------------------------------------
export  _PWM16_2_ISR


AREA InterruptRAM (RAM,REL,CON)

;@PSoC_UserCode_INIT@ (Do not change this line.)
;---------------------------------------------------
; Insert your custom declarations below this banner
;---------------------------------------------------

;------------------------
; Includes
;------------------------

	
;------------------------
;  Constant Definitions
;------------------------


;------------------------
; Variable Allocation
;------------------------


;---------------------------------------------------
; Insert your custom declarations above this banner
;---------------------------------------------------
;@PSoC_UserCode_END@ (Do not change this line.)


AREA UserModules (ROM, REL)

;-----------------------------------------------------------------------------
;  FUNCTION NAME: _PWM16_2_ISR
;
;  DESCRIPTION: Unless modified, this implements only a null handler stub.
;
;-----------------------------------------------------------------------------
;

_PWM16_2_ISR:

   ;@PSoC_UserCode_BODY@ (Do not change this line.)
   ;---------------------------------------------------
   ; Insert your custom code below this banner
   ;---------------------------------------------------
   ;   NOTE: interrupt service routines must preserve
   ;   the values of the A and X CPU registers.

   ;---------------------------------------------------
   ; Insert your custom code above this banner
   ;---------------------------------------------------
   ;@PSoC_UserCode_END@ (Do not change this line.)

   reti


; end of file PWM16_2INT.asm
