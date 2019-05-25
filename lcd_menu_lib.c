/*******************************************************************
  Simple program to check LCD functionality on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  mzapo_lcdtest.c       - main and only file

  (C) Copyright 2004 - 2017 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

#include "lcd_menu_lib.h"
#include "convert_lib.h"
#include "write2lcd_lib.h"

void menu(int rotate1, int rotate2, int rotate3, int button1, int button2, int button3, int contrast_lcd)
{
	int color1, color2 = 0;
	if(contrast_lcd==1) {color1=0x0000; color2=0xFFFF;}
	else if(contrast_lcd==0) {color1=0xFFFF; color2=0x0000;}

	switch (((int)(((double)6*rotate2)/10.2))%37)
	{
		case 0 ... 4:
			btn2 = 1*24;
			break;
		case 5 ... 9:
			btn2 = 2*24;
			break;
		case 10 ... 14:
			btn2 = 3*24;
			break;
		case 15 ... 20:
			btn2 = 4*24;
			break;
		case 21 ... 25:
			btn2 = 5*24;
			break;
		default:
			break;
	}
	/*
	if(btn2<24) btn2=24;
	if(button2==1) {
		time2 = getMicrotime();
		if(time2>=time1+300000) {
			btn2+=24;
			if(btn2>120) btn2=24;
			time2=0; time1 = getMicrotime();
			}
		}
	*/
	//int posun=0;
	string2frame_menu("******************** KOREK ********************", 0, 0, color1, color2);

	for(int i=1;i<=5;i++) string2frame_menu("   ", i*24, 10, color1, color2);
	string2frame_menu(">>", btn2, 10, color1, color2);
	
	
	/*posun = -12+3*string2frame_menu("KNOB_1: ", 32, 0, 0, 0xFFF0);
	char rotate1_str[3]; sprintf(rotate1_str,"%d", rotate1);
	string2frame_menu("    ", 32, posun+4, 0, 0xFFF0);
	string2frame_menu(rotate1_str, 32, posun+4, 0, 0xFFF0);

	posun = 150-12+3*string2frame_menu("BUTTON_1: ", 32, 150, 0, 0x0FFF);
	char button1_str[3]; sprintf(button1_str,"%d", button1);
	string2frame_menu(button1_str, 32, posun+4, 0, 0x0FFF);


	posun = -12+3*string2frame_menu("KNOB_2: ", 48, 0, 0, 0xFFF0);
	char rotate2_str[3]; sprintf(rotate2_str,"%d", rotate2);
	string2frame_menu("    ", 48, posun+4, 0, 0xFFF0);
	string2frame_menu(rotate2_str, 48, posun+4, 0, 0xFFF0);

	posun = 150-12+3*string2frame_menu("BUTTON_2: ", 48, 150, 0, 0x0FFF);
	char button2_str[3]; sprintf(button2_str,"%d", button2);
	string2frame_menu(button2_str, 48, posun+4, 0, 0x0FFF);


	posun = -12+3*string2frame_menu("KNOB_3: ", 64, 0, 0, 0xFFF0);
	char rotate3_str[3]; sprintf(rotate3_str,"%d", rotate3);
	string2frame_menu("    ", 64, posun+4, 0, 0xFFF0);
	string2frame_menu(rotate3_str, 64, posun+4, 0, 0xFFF0);

	posun = 150-12+3*string2frame_menu("BUTTON_3: ", 64, 150, 0, 0x0FFF);
	char button3_str[3]; sprintf(button3_str,"%d", button3);
	string2frame_menu(button3_str, 64, posun+4, 0, 0x0FFF);*/
}



double* strip(int yrow, int xcolumn, int posuvnik1, int posuvnik2, int contrast_lcd)
{
	int color1, color2 = 0;
	if(contrast_lcd==1) {color1=0x0000; color2=0xFFFF;}
	else if(contrast_lcd==0) {color1=0xFFFF; color2=0x0000;}

	static double returned[6];
	posuvnik1 = (int)(((double)posuvnik1/255)*460);
	posuvnik2 = (int)(((double)posuvnik2/255)*460);
	int x,y;
	static double *rgb;
	uint16_t hex;
	for(y=0; y<15; y++)
	{
		for(x=0; x<460; x++)
		{
			rgb = HSV_to_RGB(((double)x/460)*360, 1, 1);
			hex = RGB_to_hex(rgb[0], rgb[1], rgb[2]);
			if((int)(((double)x/460)*360) >= posuvnik1-1 && (int)(((double)x/460)*360) <= posuvnik1+1) frame[yrow + y][xcolumn + x] = color1;
			else frame[yrow + y][xcolumn + x] = hex;
			if((int)(((double)x/460)*360)==posuvnik1) {returned[0] = rgb[0]; returned[1] = rgb[1]; returned[2] = rgb[2];} 
		}	
	}

	for(y=0; y<15; y++)
	{
		for(x=0; x<460; x++)
		{
			rgb = HSV_to_RGB(((double)x/460)*360, 1, 1);
			hex = RGB_to_hex(rgb[0], rgb[1], rgb[2]);
			if((int)(((double)x/460)*360) >= posuvnik2-1 && (int)(((double)x/460)*360) <= posuvnik2+1) frame[yrow+16 + y][xcolumn + x] = color1;
			else frame[yrow+16 + y][xcolumn + x] = hex;
			if((int)(((double)x/460)*360)==posuvnik2) {returned[3] = rgb[0]; returned[4] = rgb[1]; returned[5] = rgb[2];} 
		}
	}
	return returned;
}

void down_controll_panel(int L_rotate, int L_push, int M_rotate, int M_push,int R_rotate, int R_push, double*leds, int contrast_lcd)
{
	int color1, color2 = 0;
	if(contrast_lcd==1) {color1=0x0000; color2=0xFFFF;}
	else if(contrast_lcd==0) {color1=0xFFFF; color2=0x0000;}

	int posun=0;
	char str[255];
	for(int i=0; i<480; i++) {frame[234][i] = color1; frame[235][i] = color1; frame[236][i] = color1;}
	
	posun = -12+3*string2frame_menu("Led 1 (r, g, b): ", 237, 0, color1, color2);
	sprintf(str, "%d", (int)leds[0]);
	string2frame_menu("    ", 237, 160, color1, color2);
	posun = posun + 9 - 12 + 3*string2frame_menu(str, 237, 160, color1, color2);
	sprintf(str, "%d", (int)leds[1]);
	string2frame_menu("    ", 237, 210, color1, color2);
	posun = posun + 9 - 12 + 3*string2frame_menu(str, 237, 210, color1, color2);
	sprintf(str, "%d", (int)leds[2]);
	string2frame_menu("    ", 237, 260, color1, color2);
	posun = posun + 9 - 12 + 3*string2frame_menu(str, 237, 260, color1, color2);

	posun = -12+3*string2frame_menu("Led 2 (r, g, b): ", 253, 0, color1, color2);
	sprintf(str, "%d", (int)leds[3]);
	string2frame_menu("    ", 253, 160, color1, color2);
	posun = posun + 9 - 12 + 3*string2frame_menu(str, 253, 160, color1, color2);
	sprintf(str, "%d", (int)leds[4]);
	string2frame_menu("    ", 253, 210, color1, color2);
	posun = posun + 9 - 12 + 3*string2frame_menu(str, 253, 210, color1, color2);
	sprintf(str, "%d", (int)leds[5]);
	string2frame_menu("    ", 253, 260, color1, color2);
	posun = posun + 9 - 12 + 3*string2frame_menu(str, 253, 260, color1, color2);
	

	sprintf(str, "%d", L_push);
	string2frame_menu("    ", 270, 80, color1, color2);
	string2frame_menu(str, 270, 80, color1, color2);
	sprintf(str, "%d", M_push);
	string2frame_menu("    ", 270, 200, color1, color2);
	string2frame_menu(str, 270, 200, color1, color2);
	sprintf(str, "%d", R_push);
	string2frame_menu("    ", 270, 350, color1, color2);
	string2frame_menu(str, 270, 350, color1, color2);

	sprintf(str, "%d", L_rotate);
	string2frame_menu("    ", 290, 80, color1, color2);
	string2frame_menu(str, 290, 80, color1, color2);
	sprintf(str, "%d", M_rotate);
	string2frame_menu("    ", 290, 200, color1, color2);
	string2frame_menu(str, 290, 200, color1, color2);
	sprintf(str, "%d", R_rotate);
	string2frame_menu("    ", 290, 350, color1, color2);
	string2frame_menu(str, 290, 350, color1, color2);
}

