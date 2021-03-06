//*****************************************************************************
//*****************************************************************************
//  FILENAME: DigBuf_1.h
//  Version: 1.3, Updated on 2009/3/31 at 12:4:33
//  Generated by PSoC Designer 5.0.423.0
//
//  DESCRIPTION: DigitalBuffers User Module C Language interface file
//               for the 22/24/27/28xxx PSoC family of devices
//-----------------------------------------------------------------------------
//  Copyright (c) Cypress MicroSystems 2000-2003. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************

#include <m8c.h>

#pragma fastcall16 DigBuf_1_EnableInt
#pragma fastcall16 DigBuf_1_DisableInt
#pragma fastcall16 DigBuf_1_Start
#pragma fastcall16 DigBuf_1_Stop

//-------------------------------------------------
// Prototypes of the DigBuf_1 API.
//-------------------------------------------------

extern void DigBuf_1_EnableInt(void);
extern void DigBuf_1_DisableInt(void);
extern void DigBuf_1_Start(void);
extern void DigBuf_1_Stop(void);


//--------------------------------------------------
// Constants for DigBuf_1 API's.
//--------------------------------------------------
#define DigBuf_1_CONTROL_REG_START_BIT         ( 0x01 )
#define DigBuf_1_INT_REG_ADDR                  ( 0x0e1 )
#define DigBuf_1_INT_MASK                      ( 0x01 )


//--------------------------------------------------
// Constants for DigBuf_1 user defined values
//--------------------------------------------------
#pragma ioport  DigBuf_1_DATA_0_REG:    0x020              //DR0 Count register
BYTE            DigBuf_1_DATA_0_REG;
#pragma ioport  DigBuf_1_DATA_1_REG:    0x021              //DR1 Period register
BYTE            DigBuf_1_DATA_1_REG;
#pragma ioport  DigBuf_1_DATA_2_REG:    0x022              //DR2 Compare register
BYTE            DigBuf_1_DATA_2_REG;
#pragma ioport  DigBuf_1_CONTROL_REG:   0x023              //Control register
BYTE            DigBuf_1_CONTROL_REG;
#pragma ioport  DigBuf_1_FUNC_REG:  0x120                  //Function register
BYTE            DigBuf_1_FUNC_REG;
#pragma ioport  DigBuf_1_INPUT_REG: 0x121                  //Input register
BYTE            DigBuf_1_INPUT_REG;
#pragma ioport  DigBuf_1_OUTPUT_REG:    0x122              //Output register
BYTE            DigBuf_1_OUTPUT_REG;
#pragma ioport  DigBuf_1_INT_REG:       0x0e1              //Interrupt Mask Register
BYTE            DigBuf_1_INT_REG;


//-------------------------------------------------
// Register Addresses for DigBuf_1
//-------------------------------------------------


//-------------------------------------------------
// DigBuf_1 Macro 'Functions'
//-------------------------------------------------

#define DigBuf_1_Start_M \
   DigBuf_1_CONTROL_REG |=  DigBuf_1_CONTROL_REG_START_BIT

#define DigBuf_1_Stop_M  \
   DigBuf_1_CONTROL_REG &= ~DigBuf_1_CONTROL_REG_START_BIT

#define DigBuf_1_EnableInt_M   \
   M8C_EnableIntMask(DigBuf_1_INT_REG, DigBuf_1_INT_MASK)

#define DigBuf_1_DisableInt_M  \
   M8C_DisableIntMask(DigBuf_1_INT_REG, DigBuf_1_INT_MASK)

// end of file DigBuf_1.h
