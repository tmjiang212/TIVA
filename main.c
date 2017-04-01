//*****************************************************************************
//
// grlib_demo.c - Demonstration of the Stellaris Graphics Library.
//
// Copyright (c) 2008-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 6075 of the DK-LM3S9B96 Firmware Package.
//
//*****************************************************************************

#include "stdint.h"
#include "stdbool.h"
#include "time.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/rom.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
#include "utils/ustdlib.h"
#include "Kentec320x240x16_ssd2119_8bit.h"
#include "touch.h"
#include "images.h"


#include "driverlib/adc.h"



//*****************************************************************************
//
// The DMA control structure table.
//
//*****************************************************************************
#ifdef ewarm
#pragma data_alignment=1024
tDMAControlTable sDMAControlTable[64];
#elif defined(ccs)
#pragma DATA_ALIGN(sDMAControlTable, 1024)
tDMAControlTable sDMAControlTable[64];
#else
tDMAControlTable sDMAControlTable[64] __attribute__ ((aligned(1024)));
#endif

//*****************************************************************************
//
// Forward declarations for the globals required to define the widgets at
// compile-time.
//
//*****************************************************************************
void OnPrevious(tWidget *pWidget);
void OnNext(tWidget *pWidget);
void OnIntroPaint(tWidget *pWidget, tContext *pContext);
void OnPrimitivePaint(tWidget *pWidget, tContext *pContext);
void OnCanvasPaint(tWidget *pWidget, tContext *pContext);
void OnCheckChange(tWidget *pWidget, uint32_t bSelected);
void OnButtonPress(tWidget *pWidget);

void OnRadioChange(tWidget *pWidget, uint32_t bSelected);
void OnSliderChange(tWidget *pWidget, int32_t lValue);
extern tCanvasWidget g_psPanels[];

int32_t VolumeIndex = 50;
int32_t BalanceIndex = 50;
int32_t MidIndex = 0;
int32_t BassIndex = 0;
int32_t TrebleIndex = 0;
int32_t VUIndex = 50;


//*****************************************************************************
//
// The first panel, which contains introductory text explaining the
// application.
//
//*****************************************************************************


tCanvasWidget baseLayer[] ={


		                       CanvasStruct( g_psPanels, baseLayer + 1, 0, &g_sKentec320x240x16_SSD2119, 29,
						                     44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
						                      0, ClrGold, 0, 0, 0, 0, 0),

							   CanvasStruct( g_psPanels, baseLayer + 2, 0, &g_sKentec320x240x16_SSD2119, 54,
								              44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
											  0, ClrGold, 0, 0, 0, 0, 0),

							   CanvasStruct( g_psPanels, baseLayer + 3, 0, &g_sKentec320x240x16_SSD2119, 79,
								               44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
											   0, ClrGold, 0, 0, 0, 0, 0),

						       CanvasStruct( g_psPanels, baseLayer + 4, 0, &g_sKentec320x240x16_SSD2119, 104,
											 44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
											    0, ClrGold, 0, 0, 0, 0, 0),

							   CanvasStruct( g_psPanels, baseLayer + 5, 0, &g_sKentec320x240x16_SSD2119, 129,
											  44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
										     0, ClrGold, 0, 0, 0, 0, 0),

								CanvasStruct( g_psPanels, baseLayer + 6, 0, &g_sKentec320x240x16_SSD2119, 154,
									         44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
											 0, ClrGold, 0, 0, 0, 0, 0),

								CanvasStruct( g_psPanels, baseLayer + 7, 0, &g_sKentec320x240x16_SSD2119, 179,
											 44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
											 0, ClrGold, 0, 0, 0, 0, 0),

								CanvasStruct( g_psPanels,baseLayer + 8, 0, &g_sKentec320x240x16_SSD2119, 204,
											 44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
											  0, ClrGold, 0, 0, 0, 0, 0),


								CanvasStruct( g_psPanels,baseLayer + 9, 0, &g_sKentec320x240x16_SSD2119, 229,
											   44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
											  	0, ClrGold, 0, 0, 0, 0, 0),

								 CanvasStruct( g_psPanels,baseLayer + 10, 0, &g_sKentec320x240x16_SSD2119, 254,
											 44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
											 0, ClrGold, 0, 0, 0, 0, 0),

								CanvasStruct( g_psPanels,0, 0, &g_sKentec320x240x16_SSD2119, 279,
											  44, 22, 42, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
											 0, ClrGold, 0, 0, 0, 0, 0),

};

tCanvasWidget VULabel [] ={
		CanvasStruct( g_psPanels, baseLayer, 0, &g_sKentec320x240x16_SSD2119, 5,
				       95, 300, 20, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
				       ClrBlack, ClrBlack, ClrGold, g_psFontCm20, "DB -20 -15 -10 -6  -3 0  +2 +4", 0, 0),
};


tCanvasWidget g_psVUmeter [] ={

		//Left Chanel

		CanvasStruct( g_psPanels, g_psVUmeter + 1, 0, &g_sKentec320x240x16_SSD2119, 5,
		       55, 20, 20, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
		       ClrBlack, ClrBlack, ClrGold, g_psFontCm20, "L", 0, 0),

		CanvasStruct( g_psPanels, g_psVUmeter + 2, 0, &g_sKentec320x240x16_SSD2119, 30,
				       45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
				       ClrGreen, 0, 0, 0, 0, 0, 0),

	   CanvasStruct( g_psPanels, g_psVUmeter + 3, 0, &g_sKentec320x240x16_SSD2119, 55,
		              45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
					  ClrGreen, 0, 0, 0, 0, 0, 0),

	   CanvasStruct( g_psPanels, g_psVUmeter + 4, 0, &g_sKentec320x240x16_SSD2119, 80,
		               45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
					   ClrGreen, 0, 0, 0, 0, 0, 0),

       CanvasStruct( g_psPanels, g_psVUmeter + 5, 0, &g_sKentec320x240x16_SSD2119, 105,
					 45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
					    ClrGreen, 0, 0, 0, 0, 0, 0),

	   CanvasStruct( g_psPanels, g_psVUmeter + 6, 0, &g_sKentec320x240x16_SSD2119, 130,
					  45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
				     ClrGreen, 0, 0, 0, 0, 0, 0),

		CanvasStruct( g_psPanels, g_psVUmeter + 7, 0, &g_sKentec320x240x16_SSD2119, 155,
			         45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
					 ClrGreen, 0, 0, 0, 0, 0, 0),

		CanvasStruct( g_psPanels, g_psVUmeter + 8, 0, &g_sKentec320x240x16_SSD2119, 180,
					 45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
					 ClrYellow, 0, 0, 0, 0, 0, 0),

		CanvasStruct( g_psPanels, g_psVUmeter + 9, 0, &g_sKentec320x240x16_SSD2119, 205,
					 45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
					  ClrYellow, 0, 0, 0, 0, 0, 0),


		CanvasStruct( g_psPanels, g_psVUmeter + 10, 0, &g_sKentec320x240x16_SSD2119, 230,
					   45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
					  	ClrYellow, 0, 0, 0, 0, 0, 0),

		 CanvasStruct( g_psPanels,g_psVUmeter + 11, 0, &g_sKentec320x240x16_SSD2119, 255,
					 45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
					 ClrRed, 0, 0, 0, 0, 0, 0),

		CanvasStruct( g_psPanels,g_psVUmeter + 12, 0, &g_sKentec320x240x16_SSD2119, 280,
					  45, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
					 ClrRed, 0, 0, 0, 0, 0, 0),


	//Right Chanel

						CanvasStruct( g_psPanels, g_psVUmeter + 13, 0, &g_sKentec320x240x16_SSD2119, 5,
						       145, 20, 20, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
						       ClrBlack, ClrBlack, ClrGold, g_psFontCm20, "R", 0, 0),

						CanvasStruct( g_psPanels, g_psVUmeter + 14, 0, &g_sKentec320x240x16_SSD2119, 30,
								       130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
								       ClrGreen, ClrGold, 0, 0, 0, 0, 0),

					   CanvasStruct( g_psPanels, g_psVUmeter + 15, 0, &g_sKentec320x240x16_SSD2119, 55,
						              130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
									  ClrGreen, ClrGold, 0, 0, 0, 0, 0),

					   CanvasStruct( g_psPanels, g_psVUmeter + 16, 0, &g_sKentec320x240x16_SSD2119, 80,
						               130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
									   ClrGreen, ClrGold, 0, 0, 0, 0, 0),

				       CanvasStruct( g_psPanels, g_psVUmeter + 17, 0, &g_sKentec320x240x16_SSD2119, 105,
									 130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
									    ClrGreen, ClrGold, 0, 0, 0, 0, 0),

					   CanvasStruct( g_psPanels, g_psVUmeter + 18, 0, &g_sKentec320x240x16_SSD2119, 130,
									  130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
								     ClrGreen, ClrGold, 0, 0, 0, 0, 0),

						CanvasStruct( g_psPanels, g_psVUmeter + 19, 0, &g_sKentec320x240x16_SSD2119, 155,
							         130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
									 ClrGreen, ClrGold, 0, 0, 0, 0, 0),

						CanvasStruct( g_psPanels, g_psVUmeter + 20, 0, &g_sKentec320x240x16_SSD2119, 180,
									 130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
									 ClrYellow, ClrGold, 0, 0, 0, 0, 0),

						CanvasStruct( g_psPanels, g_psVUmeter + 21, 0, &g_sKentec320x240x16_SSD2119, 205,
									 130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
									  ClrYellow, ClrGold, 0, 0, 0, 0, 0),


						CanvasStruct( g_psPanels, g_psVUmeter + 22, 0, &g_sKentec320x240x16_SSD2119, 230,
									   130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
									  	ClrYellow, ClrGold, 0, 0, 0, 0, 0),

						 CanvasStruct( g_psPanels,g_psVUmeter + 23, 0, &g_sKentec320x240x16_SSD2119, 255,
									 130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
									 ClrRed, ClrGold, 0, 0, 0, 0, 0),

						CanvasStruct( g_psPanels,VULabel, 0, &g_sKentec320x240x16_SSD2119, 280,
									  130, 20, 40, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE ),
									 ClrRed, ClrGold, 0, 0, 0, 0, 0),


};





//*****************************************************************************
//
// The second panel, which demonstrates the graphics primitives.
//
//*****************************************************************************


Canvas(textBalance, g_psPanels + 1, 0, 0, &g_sKentec320x240x16_SSD2119, 120,
       155, 75, 20, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
       ClrBlack, ClrBlack, ClrGold, g_psFontCm20, "Balance", 0, 0);

Canvas(textVolume, g_psPanels + 1, &textBalance, 0, &g_sKentec320x240x16_SSD2119, 120,
       80, 75, 20, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
       ClrBlack, ClrBlack, ClrGold, g_psFontCm20, "Volume", 0, 0);

tSliderWidget g_psControls[] ={

		 SliderStruct(g_psPanels + 1,g_psControls + 1, 0,
		                 &g_sKentec320x240x16_SSD2119, 65, 115, 190, 30, 0, 100, 50,
						 (SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
					    SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
		                 ClrGold, ClrBlack, ClrGold, ClrWhite, ClrWhite,
		                 &g_sFontCm18, "Right 50%      50% Left", 0, 0,
		                 OnSliderChange),

		SliderStruct(g_psPanels + 1, &textVolume , 0,
		                 &g_sKentec320x240x16_SSD2119, 65,45, 190, 30, 0, 100, 50,
		                 (SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
		                  SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
		                 ClrGold, ClrBlack, ClrGold, ClrWhite, ClrWhite,
		                 &g_sFontCm20, "50%", 0, 0, OnSliderChange),

};

tPushButtonWidget g_psPush[] = {
		 RectangularButtonStruct(g_psPanels + 1, g_psPush + 1, 0,
		                            &g_sKentec320x240x16_SSD2119, 15, 40, 40, 40,
									PB_STYLE_OUTLINE | PB_STYLE_TEXT | PB_STYLE_FILL, ClrGold, ClrGold, ClrGold, ClrBlack,
								    &g_sFontCm22, "-", 0,
								    0, 0, 0, OnButtonPress),

		 RectangularButtonStruct( g_psPanels + 1, g_psPush + 2,0,
								    &g_sKentec320x240x16_SSD2119, 265, 40, 40, 40,
									PB_STYLE_OUTLINE | PB_STYLE_TEXT | PB_STYLE_FILL, ClrGold, ClrGold, ClrGold, ClrBlack,
								    &g_sFontCm22, "+", 0,
								    0, 0, 0, OnButtonPress),

		 RectangularButtonStruct( g_psPanels + 1, g_psPush + 3,0,
								    &g_sKentec320x240x16_SSD2119, 15, 110, 40, 40,
									PB_STYLE_OUTLINE | PB_STYLE_TEXT | PB_STYLE_FILL, ClrGold, ClrGold, ClrGold, ClrBlack,
								    &g_sFontCm22, "-", 0,
									0, 0, 0, OnButtonPress),

	     RectangularButtonStruct( g_psPanels + 1, g_psControls,0,
								     &g_sKentec320x240x16_SSD2119,265, 110, 40, 40,
										PB_STYLE_OUTLINE | PB_STYLE_TEXT | PB_STYLE_FILL, ClrGold, ClrGold, ClrGold, ClrBlack,
									 &g_sFontCm22, "+", 0,
									0, 0, 0, OnButtonPress)


};


#define BalanceValue   0
#define VolumeValue     1

#define VolumeIncrease  1
#define VolumeDecrease  0

#define BalanceIncrease 3
#define BalanceDecrease  2


//*****************************************************************************
//
// The third panel, which demonstrates the canvas widget.
//
//*****************************************************************************



tPushButtonWidget g_psFreqPush[] = {
		 RectangularButtonStruct(g_psPanels + 2, g_psFreqPush + 1, 0,
		                            &g_sKentec320x240x16_SSD2119, 10, 35, 30, 30,
									PB_STYLE_OUTLINE | PB_STYLE_TEXT | PB_STYLE_FILL, ClrGold, ClrGold, ClrGold, ClrBlack,
								    &g_sFontCm22, "-", g_pucBlue50x50,
								    g_pucBlue50x50Press, 0, 0, OnButtonPress),

		 RectangularButtonStruct( g_psPanels + 2, g_psFreqPush + 2,0,
								    &g_sKentec320x240x16_SSD2119, 280, 35, 30, 30,
									PB_STYLE_OUTLINE | PB_STYLE_TEXT | PB_STYLE_FILL, ClrGold, ClrGold, ClrGold, ClrBlack,
								    &g_sFontCm22, "+", g_pucBlue50x50,
								    g_pucBlue50x50Press, 0, 0, OnButtonPress),

		 RectangularButtonStruct( g_psPanels + 2, g_psFreqPush + 3,0,
								    &g_sKentec320x240x16_SSD2119, 10, 90, 30, 30,
									PB_STYLE_OUTLINE | PB_STYLE_TEXT | PB_STYLE_FILL, ClrGold, ClrGold, ClrGold, ClrBlack,
								    &g_sFontCm22, "-", g_pucBlue50x50,
									g_pucBlue50x50Press, 0, 0, OnButtonPress),

	     RectangularButtonStruct( g_psPanels + 2, g_psFreqPush + 4,0,
								     &g_sKentec320x240x16_SSD2119,280, 90, 30, 30,
										PB_STYLE_OUTLINE | PB_STYLE_TEXT | PB_STYLE_FILL, ClrGold, ClrGold, ClrGold, ClrBlack,
									 &g_sFontCm22, "+", g_pucBlue50x50,
									g_pucBlue50x50Press, 0, 0, OnButtonPress),

	     RectangularButtonStruct( g_psPanels + 2, g_psFreqPush + 5,0,
							       &g_sKentec320x240x16_SSD2119, 10, 145, 30, 30,
									PB_STYLE_OUTLINE | PB_STYLE_TEXT | PB_STYLE_FILL, ClrGold, ClrGold, ClrGold, ClrBlack,
								    &g_sFontCm22, "-", g_pucBlue50x50,
								g_pucBlue50x50Press, 0, 0, OnButtonPress),

		 RectangularButtonStruct( g_psPanels + 2, 0,0,
							     &g_sKentec320x240x16_SSD2119,280, 145, 30, 30,
									PB_STYLE_OUTLINE | PB_STYLE_TEXT | PB_STYLE_FILL, ClrGold, ClrGold, ClrGold, ClrBlack,
								 &g_sFontCm22, "+", g_pucBlue50x50,
								g_pucBlue50x50Press, 0, 0, OnButtonPress)

};


tSliderWidget g_psFrequency[] ={

		SliderStruct(g_psPanels + 2, g_psFrequency + 1, 0,
				                 &g_sKentec320x240x16_SSD2119, 50, 35, 215, 30, -6, 6, 0,
								 (SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
							    SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
				                 ClrGold, ClrBlack, ClrGold, ClrWhite, ClrWhite,
				                 &g_sFontCm20, "Bass 0 dB", 0, 0, OnSliderChange),

	    SliderStruct(g_psPanels + 2,  g_psFrequency + 2, 0,
		                        &g_sKentec320x240x16_SSD2119, 50, 90, 215, 30, -6, 6, 0,
								 (SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
							    SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
				                 ClrGold, ClrBlack, ClrGold, ClrWhite, ClrWhite,
					            &g_sFontCm20, "Mid 0 dB", 0, 0, OnSliderChange),

	   SliderStruct(g_psPanels + 2,  g_psFreqPush, 0,
                                &g_sKentec320x240x16_SSD2119, 50, 145, 215, 30, -6, 6, 0,
								 (SL_STYLE_FILL | SL_STYLE_BACKG_FILL | SL_STYLE_OUTLINE |
							    SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT),
				                 ClrGold, ClrBlack, ClrGold, ClrWhite, ClrWhite,
								 &g_sFontCm20, "Treble 0 dB", 0, 0, OnSliderChange)

};



#define BassValue   0
#define MidValue    1
#define TrebleValue 2

#define BassDecrease 0
#define BassIncrease 1
#define MidDecrease 2
#define MidIncrease 3
#define TrebleDecrease 4
#define TrebleIncrease 5






//*****************************************************************************
//
// An array of canvas widgets, one per panel.  Each canvas is filled with
// black, overwriting the contents of the previous panel.
//
//*****************************************************************************
tCanvasWidget g_psPanels[] =
{
    CanvasStruct(0, 0, g_psVUmeter, &g_sKentec320x240x16_SSD2119, 0, 24,
                 320, 166, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0),
    CanvasStruct(0, 0, g_psPush, &g_sKentec320x240x16_SSD2119, 0, 24,
                 320, 166, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0),
    CanvasStruct(0, 0, g_psFrequency, &g_sKentec320x240x16_SSD2119, 0, 24, 320,
                 166, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0),


};

//*****************************************************************************
//
// The number of panels.
//
//*****************************************************************************
#define NUM_PANELS              (sizeof(g_psPanels) / sizeof(g_psPanels[0]))

//*****************************************************************************
//
// The names for each of the panels, which is displayed at the bottom of the
// screen.
//
//*****************************************************************************
char *g_pcPanelNames[] =
{
    "     VU Meter     ",
    "     Sound Control     ",
    "     Frequency Control     ",

};

//*****************************************************************************
//
// The buttons and text across the bottom of the screen.
//
//*****************************************************************************
RectangularButton(g_sPrevious, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 0, 190,
                  50, 50, PB_STYLE_FILL, ClrBlack, ClrBlack, 0, ClrWhite,
                  &g_sFontCm20, "Prev", g_pucBlue50x50, g_pucBlue50x50Press, 0, 0,
                  OnPrevious);

Canvas(g_sTitle, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 50, 190, 220, 50,
       CANVAS_STYLE_TEXT | CANVAS_STYLE_TEXT_OPAQUE, 0, 0, ClrGold,
       &g_sFontCm20, 0, 0, 0);

RectangularButton(g_sNext, 0, 0, 0, &g_sKentec320x240x16_SSD2119, 270, 190,
                  50, 50, PB_STYLE_IMG | PB_STYLE_TEXT, ClrBlack, ClrBlack, 0,
                  ClrWhite, &g_sFontCm20, "Next", g_pucBlue50x50,
                  g_pucBlue50x50Press, 0, 0, OnNext);

//*****************************************************************************
//
// The panel that is currently being displayed.
//
//*****************************************************************************
uint32_t g_ulPanel;

//*****************************************************************************
//
// Handles presses of the previous panel button.
//
//*****************************************************************************
void
OnPrevious(tWidget *pWidget)
{
    //
    // There is nothing to be done if the first panel is already being
    // displayed.
    //
    if(g_ulPanel == 0)
    {
        return;
    }

    //
    // Remove the current panel.
    //
    WidgetRemove((tWidget *)(g_psPanels + g_ulPanel));

    //
    // Decrement the panel index.
    //
    g_ulPanel--;

    //
    // Add and draw the new panel.
    //
    WidgetAdd(WIDGET_ROOT, (tWidget *)(g_psPanels + g_ulPanel));
    WidgetPaint((tWidget *)(g_psPanels + g_ulPanel));

    //
    // Set the title of this panel.
    //
    CanvasTextSet(&g_sTitle, g_pcPanelNames[g_ulPanel]);
    WidgetPaint((tWidget *)&g_sTitle);

    //
    // See if this is the first panel.
    //
    if(g_ulPanel == 0)
    {
        //
        // Clear the previous button from the display since the first panel is
        // being displayed.
        //
        PushButtonImageOff(&g_sPrevious);
        PushButtonTextOff(&g_sPrevious);
        PushButtonFillOn(&g_sPrevious);
        WidgetPaint((tWidget *)&g_sPrevious);
    }

    //
    // See if the previous panel was the last panel.
    //
    if(g_ulPanel == (NUM_PANELS - 2))
    {
        //
        // Display the next button.
        //
        PushButtonImageOn(&g_sNext);
        PushButtonTextOn(&g_sNext);
        PushButtonFillOff(&g_sNext);
        WidgetPaint((tWidget *)&g_sNext);
    }

}

//*****************************************************************************
//
// Handles presses of the next panel button.
//
//*****************************************************************************
void
OnNext(tWidget *pWidget)
{
    //
    // There is nothing to be done if the last panel is already being
    // displayed.
    //
    if(g_ulPanel == (NUM_PANELS - 1))
    {
        return;
    }

    //
    // Remove the current panel.
    //
    WidgetRemove((tWidget *)(g_psPanels + g_ulPanel));

    //
    // Increment the panel index.
    //
    g_ulPanel++;

    //
    // Add and draw the new panel.
    //
    WidgetAdd(WIDGET_ROOT, (tWidget *)(g_psPanels + g_ulPanel));
    WidgetPaint((tWidget *)(g_psPanels + g_ulPanel));

    //
    // Set the title of this panel.
    //
    CanvasTextSet(&g_sTitle, g_pcPanelNames[g_ulPanel]);
    WidgetPaint((tWidget *)&g_sTitle);

    //
    // See if the previous panel was the first panel.
    //
    if(g_ulPanel == 1)
    {
        //
        // Display the previous button.
        //
        PushButtonImageOn(&g_sPrevious);
        PushButtonTextOn(&g_sPrevious);
        PushButtonFillOff(&g_sPrevious);
        WidgetPaint((tWidget *)&g_sPrevious);
    }

    //
    // See if this is the last panel.
    //
    if(g_ulPanel == (NUM_PANELS - 1))
    {
        //
        // Clear the next button from the display since the last panel is being
        // displayed.
        //
        PushButtonImageOff(&g_sNext);
        PushButtonTextOff(&g_sNext);
        PushButtonFillOn(&g_sNext);
        WidgetPaint((tWidget *)&g_sNext);
    }

}






//*****************************************************************************
//
// Handles press notifications for the push button widgets.
//
//*****************************************************************************
void
OnButtonPress(tWidget *pWidget)
{
	static char VolumeText[40] ;
	static char BalanceText[40];
	static char BassText[40];
	static char MidText[40];
	static char TrebleText[40];



	 if(pWidget == (tWidget *)&g_psPush[VolumeIncrease])
	    {
		 	if (VolumeIndex <= 90 ){
		 		VolumeIndex = VolumeIndex + 10;
		 	}
	    	usprintf(VolumeText, "%3d%%", VolumeIndex);
	    	SliderTextSet(&g_psControls[VolumeValue], VolumeText);
	    	SliderValueSet(&g_psControls[VolumeValue],VolumeIndex);
	    	WidgetPaint((tWidget *)&g_psControls[VolumeValue]);

	    }

	 if(pWidget == (tWidget *)&g_psPush[VolumeDecrease])
	 	    {
	 		 	if (VolumeIndex >= 10 ){
	 		 		VolumeIndex = VolumeIndex - 10;
	 		 	}
	 	    	usprintf(VolumeText, "%3d%%", VolumeIndex);
	 	    	SliderTextSet(&g_psControls[VolumeValue], VolumeText);
	 	    	SliderValueSet(&g_psControls[VolumeValue],VolumeIndex);
	 	    	WidgetPaint((tWidget *)&g_psControls[VolumeValue]);

	 	    }


	 if(pWidget == (tWidget *)&g_psPush[BalanceIncrease])
	 	 	    {
	 	 		 	if (BalanceIndex <= 90 ){
	 	 		 		BalanceIndex = BalanceIndex + 10;
	 	 		 	}
	 	 	    	usprintf(BalanceText, "Right %3d%%    %3d%% Left", BalanceIndex, 100 - BalanceIndex);
	 	 	    	SliderTextSet(&g_psControls[BalanceValue], BalanceText);
	 	 	    	SliderValueSet(&g_psControls[BalanceValue],BalanceIndex);
	 	 	    	WidgetPaint((tWidget *)&g_psControls[BalanceValue]);

	 	 	    }

	 if(pWidget == (tWidget *)&g_psPush[BalanceDecrease])
		 	 	    {
		 	 		 	if (BalanceIndex >= 10 ){
		 	 		 		BalanceIndex = BalanceIndex - 10;
		 	 		 	}
		 	 	    	usprintf(BalanceText, "Right %3d%%    %3d%% Left", BalanceIndex, 100 - BalanceIndex);
		 	 	    	SliderTextSet(&g_psControls[BalanceValue], BalanceText);
		 	 	    	SliderValueSet(&g_psControls[BalanceValue],BalanceIndex);
		 	 	    	WidgetPaint((tWidget *)&g_psControls[BalanceValue]);

		 	 	    }



	 if(pWidget == (tWidget *)&g_psFreqPush[BassIncrease])
		 	 	    {
		 	 		 	if (BassIndex <= 5 ){
		 	 		 		BassIndex = BassIndex + 1;
		 	 		 	}
		 	 	    	usprintf(BassText, "Bass %3d dB", BassIndex);
		 	 	    	SliderTextSet(&g_psFrequency[BassValue], BassText);
		 	 	    	SliderValueSet(&g_psFrequency[BalanceValue],BassIndex);
		 	 	    	WidgetPaint((tWidget *)&g_psFrequency[BassValue]);

		 	 	    }

	 if(pWidget == (tWidget *)&g_psFreqPush[BassDecrease])
	 		 	 	    {
	 		 	 		 	if (BassIndex >= -5 ){
	 		 	 		 		BassIndex = BassIndex - 1;
	 		 	 		 	}
	 		 	 	    	usprintf(BassText, "Bass %3d dB", BassIndex);
	 		 	 	    	SliderTextSet(&g_psFrequency[BassValue], BassText);
	 		 	 	    	SliderValueSet(&g_psFrequency[BalanceValue],BassIndex);
	 		 	 	    	WidgetPaint((tWidget *)&g_psFrequency[BassValue]);

	 		 	 	    }

	 if(pWidget == (tWidget *)&g_psFreqPush[MidIncrease])
	 	 		 	 	    {
	 	 		 	 		 	if (MidIndex <= 5 ){
	 	 		 	 		 		MidIndex = MidIndex + 1;
	 	 		 	 		 	}
	 	 		 	 	    	usprintf(MidText, "Mid %3d dB", MidIndex);
	 	 		 	 	    	SliderTextSet(&g_psFrequency[MidValue], MidText);
	 	 		 	 	    	SliderValueSet(&g_psFrequency[MidValue],MidIndex);
	 	 		 	 	    	WidgetPaint((tWidget *)&g_psFrequency[MidValue]);

	 	 		 	 	    }

	 if(pWidget == (tWidget *)&g_psFreqPush[MidDecrease])
	 	 	 		 	 	    {
	 	 	 		 	 		 	if (MidIndex >= -5 ){
	 	 	 		 	 		 		MidIndex = MidIndex - 1;
	 	 	 		 	 		 	}
	 	 	 		 	 	    	usprintf(MidText, "Mid %3d dB", MidIndex);
	 	 	 		 	 	    	SliderTextSet(&g_psFrequency[MidValue], MidText);
	 	 	 		 	 	    	SliderValueSet(&g_psFrequency[MidValue],MidIndex);
	 	 	 		 	 	    	WidgetPaint((tWidget *)&g_psFrequency[MidValue]);

	 	 	 		 	 	    }

	 if(pWidget == (tWidget *)&g_psFreqPush[TrebleIncrease])
	 	 	 		 	 	    {
	 	 	 		 	 		 	if (TrebleIndex <= 5 ){
	 	 	 		 	 		 		TrebleIndex = TrebleIndex + 1;
	 	 	 		 	 		 	}
	 	 	 		 	 	    	usprintf(TrebleText, "Treble %3d dB", TrebleIndex);
	 	 	 		 	 	    	SliderTextSet(&g_psFrequency[TrebleValue], TrebleText);
	 	 	 		 	 	    	SliderValueSet(&g_psFrequency[TrebleValue],TrebleIndex);
	 	 	 		 	 	    	WidgetPaint((tWidget *)&g_psFrequency[TrebleValue]);

	 	 	 		 	 	    }

	 if(pWidget == (tWidget *)&g_psFreqPush[TrebleDecrease])
	 	 	 	 		 	 	    {
	 	 	 	 		 	 		 	if (TrebleIndex >= -5 ){
	 	 	 	 		 	 		 		TrebleIndex = TrebleIndex - 1;
	 	 	 	 		 	 		 	}
	 	 	 	 		 	 	    	usprintf(TrebleText, "Treble %3d dB", TrebleIndex);
	 	 	 	 		 	 	    	SliderTextSet(&g_psFrequency[TrebleValue], TrebleText);
	 	 	 	 		 	 	    	SliderValueSet(&g_psFrequency[TrebleValue],TrebleIndex);
	 	 	 	 		 	 	    	WidgetPaint((tWidget *)&g_psFrequency[TrebleValue]);

	 	 	 	 		 	 	    }




}

//*****************************************************************************
//
// Handles notifications from the slider controls.
//
//*****************************************************************************
void
OnSliderChange(tWidget *pWidget, int32_t lValue)//, int32_t lValue2,int32_t lValue3,int32_t lValue4,int32_t lValue5,int32_t lValue6)
{
  //  static char pcCanvasText[5];
    static char VolumeText[40] ;
    static char BalanceText[40];
    static char BassText[40];
    static char MidText[40];
    static char TrebleText[40];



    if(pWidget == (tWidget *)&g_psControls[VolumeValue])
    {

    	VolumeIndex = lValue;
    	usprintf(VolumeText, "%3d%%", lValue);
    	SliderTextSet(&g_psControls[VolumeValue], VolumeText);
    	WidgetPaint((tWidget *)&g_psControls[VolumeValue]);

    }

    if(pWidget == (tWidget *)&g_psControls[BalanceValue])
    {
        //
        // Yes - update the canvas to show the slider value.
        //
        usprintf(BalanceText, "Right %3d%%    %3d%% Left", lValue, 100-lValue);
        SliderTextSet(&g_psControls[BalanceValue], BalanceText);
       // SliderValueSet(&g_psControls[SLIDER_TEXT_VAL_INDEX], lValue);
        WidgetPaint((tWidget *)&g_psControls[BalanceValue]);
    }

    if(pWidget == (tWidget *)&g_psFrequency[BassValue])
       {
           //
           // Yes - update the canvas to show the slider value.
           //

    	 usprintf(BassText, "Bass %3d dB", lValue);
           SliderTextSet(&g_psFrequency[BassValue], BassText);
          // SliderValueSet(&g_psControls[BassValue], lValue3);
           WidgetPaint((tWidget *)&g_psFrequency[BassValue]);
       }

    if(pWidget == (tWidget *)&g_psFrequency[MidValue])
          {
              //
              // Yes - update the canvas to show the slider value.
              //
              usprintf(MidText, "Mid %3d dB", lValue);
              SliderTextSet(&g_psFrequency[MidValue], MidText);
            //  SliderValueSet(&g_psControls[MidValue], lValue4);
              WidgetPaint((tWidget *)&g_psFrequency[MidValue]);
          }

    if(pWidget == (tWidget *)&g_psFrequency[TrebleValue])
              {
                  //
                  // Yes - update the canvas to show the slider value.
                  //
                  usprintf(TrebleText, "Treble %3d dB", lValue);
                  SliderTextSet(&g_psFrequency[TrebleValue], TrebleText);
                //  SliderValueSet(&g_psControls[TrebleValue], lValue5);
                  WidgetPaint((tWidget *)&g_psFrequency[TrebleValue]);
              }


}









//*****************************************************************************
//
// A simple demonstration of the features of the Stellaris Graphics Library.
//
//*****************************************************************************
void vuUpdate(){
	uint32_t voltageInput = 50;
	uint32_t voltageCutoff = voltageInput / 11;
   // static char VolumeText[40];
  //  usprintf(VolumeText, "%3d%%", VolumeIndex);
//	SliderValueSet(&g_psControls[VolumeValue],VolumeIndex);
	//SliderTextSet(&g_psControls[VolumeValue], VolumeText);


	if(VUIndex < voltageCutoff)
	{
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
	}

	else if(VUIndex >= voltageCutoff && VUIndex < (voltageCutoff * 2) )
	{
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);

		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
	}

	else if(VUIndex >= (voltageCutoff * 2) && VUIndex < (voltageCutoff * 3) )
	{
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrGreen);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrBlack);
		CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
	}


	else if(VUIndex >= (voltageCutoff * 3) && VUIndex < (voltageCutoff * 4) )
		{
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrGreen);
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrGreen);
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrBlack);
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrBlack);
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrBlack);
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrBlack);
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrBlack);
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrBlack);
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrBlack);
			CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
		}


	else if(VUIndex >= (voltageCutoff * 4) && VUIndex < (voltageCutoff * 5) )
			{
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
			}




	else if(VUIndex >= (voltageCutoff * 5) && VUIndex < (voltageCutoff * 6) )
			{
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
			}



	else if(VUIndex >= (voltageCutoff * 6) && VUIndex < (voltageCutoff * 7) )
			{
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
			}

	else if(VUIndex >= (voltageCutoff * 7) && VUIndex < (voltageCutoff * 8) )
			{
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrGreen);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrYellow);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrBlack);
				CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
			}

	else if(VUIndex >= (voltageCutoff * 8) && VUIndex < (voltageCutoff * 9) )
				{
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrGreen);
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrGreen);
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrGreen);
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrGreen);
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrGreen);
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrYellow);
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrYellow);
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrBlack);
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrBlack);
					CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
				}



	else if(VUIndex >= (voltageCutoff * 9) && VUIndex < (voltageCutoff * 10) )
					{
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrYellow);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrYellow);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrYellow);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrBlack);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
					}


	else if(VUIndex >= (voltageCutoff * 10) && VUIndex < (voltageCutoff * 11) )
					{
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrGreen);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrYellow);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrYellow);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrYellow);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrRed);
						CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrBlack);
					}


	else
						{
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[1],ClrGreen);
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[2],ClrGreen);
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[3],ClrGreen);
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[4],ClrGreen);
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[5],ClrGreen);
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[6],ClrGreen);
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[7],ClrYellow);
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[8],ClrYellow);
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[9],ClrYellow);
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[10],ClrRed);
							CanvasFillColorSet((tCanvasWidget *)&g_psVUmeter[11],ClrRed);
						}






}
void vuDisplay()
{




//	 if (g_ulPanel == 1){
  //  WidgetPaint((tWidget *)&g_psControls[VolumeValue]);

//	}



	if (g_ulPanel == 0){


	    WidgetPaint((tWidget *)&g_psVUmeter[1]);
	    WidgetPaint((tWidget *)&g_psVUmeter[2]);
	    WidgetPaint((tWidget *)&g_psVUmeter[3]);
	    WidgetPaint((tWidget *)&g_psVUmeter[4]);
	    WidgetPaint((tWidget *)&g_psVUmeter[5]);
	    WidgetPaint((tWidget *)&g_psVUmeter[6]);
	    WidgetPaint((tWidget *)&g_psVUmeter[7]);
	    WidgetPaint((tWidget *)&g_psVUmeter[8]);
	    WidgetPaint((tWidget *)&g_psVUmeter[9]);
	    WidgetPaint((tWidget *)&g_psVUmeter[10]);
	    WidgetPaint((tWidget *)&g_psVUmeter[11]);

	}

}





int
main(void)
{
    tContext sContext;
    tRectangle sRect;


    //
    // The FPU should be enabled because some compilers will use floating-
    // point registers, even for non-floating-point code.  If the FPU is not
    // enabled this will cause a fault.  This also ensures that floating-
    // point operations could be added to this application and would work
    // correctly and use the hardware floating-point unit.  Finally, lazy
    // stacking is enabled for interrupt handlers.  This allows floating-
    // point instructions to be used within interrupt handlers, but at the
    // expense of extra stack usage.
    //
    FPUEnable();
    FPULazyStackingEnable();

    volatile uint32_t ui32ADC0Value[4];
    volatile uint32_t ui32RadioAvg;
    volatile uint32_t ui32RadioValue;

    //
    // Set the clock to 40Mhz derived from the PLL and the external oscillator
    //
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);


    // enable the ADC0
   SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
   //SysCtlDelay();
   ADCHardwareOversampleConfigure(ADC0_BASE, 64);


   //configure the ADC sequencer
   ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);

   //configure all four steps in the ADC sequencer
   ADCSequenceStepConfigure(ADC0_BASE, 1 ,0, ADC_CTL_CH0);
   ADCSequenceStepConfigure(ADC0_BASE, 1 ,1, ADC_CTL_CH0);
   ADCSequenceStepConfigure(ADC0_BASE, 1 ,2, ADC_CTL_CH0);
   ADCSequenceStepConfigure(ADC0_BASE, 1 ,3, ADC_CTL_CH0|ADC_CTL_IE|ADC_CTL_END);
   ADCSequenceEnable(ADC0_BASE,1);


   //ADC_CTL_TS sets the channel to be of the temperature sensor. For other channels you would use ADC_CTL_CH0 through ADC_CTL_CH23.
  // ADC_CTL_IE makes it so a interrupt is generated when the sequencer is done. Though a interrupt will not be used, the interrupt status will  be used to check if the sequence is done.
 //  ADC_CTL_END defines this step as the last of the sequence. Always use this when you don't want to use multiple steps.
//   Additionally you can add ADC_CTL_CMP0 through ADC_CTL_CMP7 so the ADC sends the value sampled to one of the comparators.



    //
    // Initialize the display driver.
    //
    Kentec320x240x16_SSD2119Init();

    //
    // Initialize the graphics context.
    //
    GrContextInit(&sContext, &g_sKentec320x240x16_SSD2119);

    //
    // Fill the top 24 rows of the screen with blue to create the banner.
    //
    sRect.i16XMin = 0;
    sRect.i16YMin = 0;
    sRect.i16XMax = GrContextDpyWidthGet(&sContext) - 1;
    sRect.i16YMax = 23;
    GrContextForegroundSet(&sContext, ClrGold);
    GrRectFill(&sContext, &sRect);

    //
    // Put a white box around the banner.
    //
    GrContextForegroundSet(&sContext, ClrWhite);
    GrRectDraw(&sContext, &sRect);

    //
    // Put the application name in the middle of the banner.
    //
    GrContextFontSet(&sContext, &g_sFontCm20);
    GrStringDrawCentered(&sContext, "Audio Control System", -1,
                         GrContextDpyWidthGet(&sContext) / 2, 8, 0);

    //
    // Configure and enable uDMA
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    SysCtlDelay(10);
    uDMAControlBaseSet(&sDMAControlTable[0]);
    uDMAEnable();

    //
    // Initialize the touch screen driver and have it route its messages to the
    // widget tree.
    //
    TouchScreenInit();
    TouchScreenCallbackSet(WidgetPointerMessage);

    //
    // Add the title block and the previous and next buttons to the widget
    // tree.
    //
    WidgetAdd(WIDGET_ROOT, (tWidget *)&g_sPrevious);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&g_sTitle);
    WidgetAdd(WIDGET_ROOT, (tWidget *)&g_sNext);

    //
    // Add the first panel to the widget tree.
    //
    g_ulPanel = 0;
    WidgetAdd(WIDGET_ROOT, (tWidget *)g_psPanels);
    CanvasTextSet(&g_sTitle, g_pcPanelNames[0]);

    //
    // Issue the initial paint request to the widgets.
    //
    WidgetPaint(WIDGET_ROOT);

    //
    // Loop forever handling widget messages.
    //


    while(1)
    {
        //
    	ADCIntClear(ADC0_BASE ,1 );
    	ADCProcessorTrigger(ADC0_BASE, 1);

    	//wait for the conversion to complete
    	while (!ADCIntStatus(ADC0_BASE, 1, false))
    	{

    	}
    	ADCIntClear(ADC0_BASE, 1);
    	ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
    	ui32RadioAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
    	ui32RadioValue = ui32RadioAvg * 3.3 /1024 /4 * 100;

       //UARTprintf("Temperature = %3d*C ", ui32RadioValue);
    	//if (VUIndex != ui32RadioValue){

    		VUIndex = ui32RadioValue;

    	    vuUpdate();
    		vuDisplay();


    //	}
    		SysCtlDelay(80000000 / 24);
       // SysCtlDelay(100);
    	// Process any messages in the widget message queue.
        //
        WidgetMessageQueueProcess();
    }
}
