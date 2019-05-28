#define _POSIX_C_SOURCE 200112L

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

#include "lcd_menu_lib.h"
#include "convert_lib.h"
#include "write2lcd_lib.h"
int click = 0;
int nextscreen = 0;
int stripStart = 0;

GUI_set_menu menu(int rotate1, int rotate2, int rotate3, int button1, int button2, int button3, GUI_set_menu menu_arr)
{
	btn2 = 24;
	int color1=0;
	int color2=0;
	if(menu_arr.colourGui==1) {color1=0x0000; color2=0xFFFF;}
	else if(menu_arr.colourGui==0) {color1=0xFFFF; color2=0x0000;}

	if(nextscreen==1) btn2=1*24;
	else{
		if(menu_arr.size==0){
			switch (((int) (6*(double)rotate2/10.2)%37))
			{
				case 0 ... 4:
					btn2 = 1*24;
					break;
				case 5 ... 9:
					btn2 = 2*24;
					break;
				case 10 ... 14:
					btn2 = 3*24;
					if(menu_arr.currentScreen==5) btn2=24;
					break;
				case 15 ... 20:
					btn2 = 4*24;
					if(menu_arr.currentScreen==4) btn2=24;
					else if(menu_arr.currentScreen==5) btn2=2*24;
					break;
				case 21 ... 25:
					btn2 = 5*24;
					if(menu_arr.currentScreen==4) btn2=2*24;
					else if(menu_arr.currentScreen==11) btn2=24;
					else if(menu_arr.currentScreen==5) btn2=24;
					break;
				case 26 ... 30:
					btn2 = 6*24;
					if(menu_arr.currentScreen==0) btn2=1*24;
					else if(menu_arr.currentScreen==11) btn2=24;
					else if(menu_arr.currentScreen==1 || menu_arr.currentScreen==2 || menu_arr.currentScreen==3) btn2=24;
					else if(menu_arr.currentScreen==4) btn2=3*24;
					else if(menu_arr.currentScreen==5) btn2=2*24;
					break;
				default:
					break;
			}
		}
		else
		{
			switch (((int) (6*(double)rotate2/10.2)%37))
			{
				case 0 ... 4:
					btn2 = 24;
					break;
				case 5 ... 9:
					btn2 = 24+1*36;
					break;
				case 10 ... 14:
					btn2 = 24+2*36;
					if(menu_arr.currentScreen==5) btn2=24;
					break;
				case 15 ... 20:
					btn2 = 24+3*36;
					if(menu_arr.currentScreen==4) btn2=24;
					else if(menu_arr.currentScreen==5) btn2=24+36;
					break;
				case 21 ... 25:
					btn2 = 24+4*36;
					if(menu_arr.currentScreen==4) btn2=24+1*36;
					else if(menu_arr.currentScreen==11) btn2=24;
					else if(menu_arr.currentScreen==5) btn2=24;
					break;
				case 26 ... 30:
					btn2 = 24+5*36;
					if(menu_arr.currentScreen==0) btn2=24;
					else if(menu_arr.currentScreen==11) btn2=24;
					else if(menu_arr.currentScreen==1 || menu_arr.currentScreen==2 || menu_arr.currentScreen==3) btn2=24;
					else if(menu_arr.currentScreen==4) btn2=24+2*36;
					else if(menu_arr.currentScreen==5) btn2=24+36;
					break;
				default:
					break;
			}
		}
		//if((menu_arr.currentScreen==11) && (menu_arr.led1.simpleLedSetup!=' ') && (menu_arr.led2.simpleLedSetup!=' ') && (btn2>72) && (menu_arr.size==0)) btn2=1*24;
		//if((menu_arr.currentScreen==11) && (menu_arr.led1.simpleLedSetup!=' ') && (menu_arr.led2.simpleLedSetup!=' ') && (btn2>72) && (menu_arr.size==0)) btn2=1*24;
		
	}
	if(menu_arr.currentScreen!=11)
	{
		for(int i=0; i<230; i++)
		{
			for(int j = 0; j<480; j++) frame[i][j] = color2;
		}
	}
	else
	{
		for(int i=0; i<180; i++)
		{
			for(int j = 0; j<480; j++) frame[i][j] = color2;
		}
	}
	
	
	nextscreen = 0;

	//string2frame_menu("******************** KOREK ********************", 0, 0, color1, color2);

	if(menu_arr.size==0){
		for(int i=1;i<=6;i++) string2frame_menu("   ", i*24, 10, color1, color2);
		string2frame_menu(">>", btn2, 10, color1, color2);
	}
	else
	{
		for(int i=0;i<5;i++)
		{
			string2frame_menu_big("   ", i*36+24, 10, color1, color2);
		}
		string2frame_menu_big(">", btn2, 10, color1, color2);
	}
	
	
	if(menu_arr.currentScreen==0) // start menu
	{
		if(menu_arr.size==0)
		{
			string2frame_menu("Change LEDs", 24, 40, color1, color2);
			string2frame_menu("Special effects", 48, 40, color1, color2);
			string2frame_menu("Ethernet settings", 72, 40, color1, color2);
			string2frame_menu("KOREK settings", 96, 40, color1, color2);
			string2frame_menu("Exit KOREK", 120, 40, color1, color2);
		}
		else
		{
			string2frame_menu_big("Change LEDs", 24, 40, color1, color2);
			string2frame_menu_big("Special effects", 60, 40, color1, color2);
			string2frame_menu_big("Ethernet settings", 96, 40, color1, color2);
			string2frame_menu_big("KOREK settings", 132, 40, color1, color2);
			string2frame_menu_big("Exit KOREK", 168, 40, color1, color2);
		}
		if(button2==1) {
			menu_arr.time2 = getMicrotime();
			if(menu_arr.time2>=menu_arr.time1+300000 || !click) {
				//printf("***\n");
				click = 1;
				if(menu_arr.size==0) menu_arr.currentScreen=btn2/24;
				else{
					if(btn2==24) menu_arr.currentScreen=1;
					else
					{
						btn2-=24;
						menu_arr.currentScreen=(btn2/36)+1;
					}
					
				}
				nextscreen = 1;
				menu_arr.time2=0; 
				menu_arr.time1 = getMicrotime();
				}
		}
	}
	else if(menu_arr.currentScreen==1) // leds menu
	{
		if(menu_arr.size==0)
		{
			string2frame_menu("Change hue (H)", 24, 40, color1, color2);
			string2frame_menu("Change saturation (S)", 48, 40, color1, color2);
			string2frame_menu("Change value (V)", 72, 40, color1, color2);
			string2frame_menu("Change period", 96, 40, color1, color2); //stejne 96 jako value u velkych pismen
			string2frame_menu("Back", 120, 40, color1, color2);
		}
		else
		{
			string2frame_menu_big("Change hue (H)", 24, 40, color1, color2);
			string2frame_menu_big("Change saturation (S)", 60, 40, color1, color2);
			string2frame_menu_big("Change value (V)", 96, 40, color1, color2);
			string2frame_menu_big("Change period", 132, 40, color1, color2);
			string2frame_menu_big("Back", 168, 40, color1, color2);
		}
		
		if(button2==1) {
		menu_arr.time2 = getMicrotime();
		if(menu_arr.time2>=menu_arr.time1+300000) {
			stripStart = 1;
			if(btn2==24) {menu_arr.led1.simpleLedSetup = 'h'; menu_arr.led2.simpleLedSetup = 'h'; menu_arr.currentScreen=11;}
			else if(btn2==48 || btn2==60) {menu_arr.led1.simpleLedSetup = 's'; menu_arr.led2.simpleLedSetup = 's'; menu_arr.currentScreen=11;}
			else if(btn2==72 || btn2==96) {menu_arr.led1.simpleLedSetup = 'v'; menu_arr.led2.simpleLedSetup = 'v'; menu_arr.currentScreen=11;}
			else if(btn2==120 || btn2==168) {menu_arr.currentScreen=0; nextscreen=1; menu_arr.led1.simpleLedSetup=' '; menu_arr.led2.simpleLedSetup=' '; stripStart=0;}
			else {menu_arr.led1.simpleLedSetup=' '; menu_arr.led2.simpleLedSetup=' '; stripStart=0;}
			menu_arr.time2=0; menu_arr.time1 = getMicrotime();
			}
		}
	}
	else if(menu_arr.currentScreen==11) // changing colors
	{
		if(menu_arr.size==0) 
		{	
			if(menu_arr.animation==1) string2frame_menu("Animation off", 24, 40, color1, color2);
			else string2frame_menu("Animation on", 24, 40, color1, color2);
			
			if(menu_arr.led1.staticLight==1) string2frame_menu("Static light 1 off", 48, 40, color1, color2);
			else string2frame_menu("Static light 1 on", 48, 40, color1, color2);
			
			if(menu_arr.led2.staticLight==1) string2frame_menu("Static light 2 off", 72, 40, color1, color2);
			else string2frame_menu("Static light 2 on", 72, 40, color1, color2);
			
			string2frame_menu("Back", 96, 40, color1, color2);
		}
		else 
		{
			if(menu_arr.animation==1) string2frame_menu_big("Animation off", 24, 40, color1, color2);
			else string2frame_menu_big("Animation on", 24, 40, color1, color2);
			
			if(menu_arr.led1.staticLight==1) string2frame_menu_big("Static light 1 off", 60, 40, color1, color2);
			else string2frame_menu_big("Static light 1 on", 60, 40, color1, color2);
			
			if(menu_arr.led2.staticLight==1) string2frame_menu_big("Static light 2 off", 96, 40, color1, color2);
			else string2frame_menu_big("Static light 2 on", 96, 40, color1, color2);
			
			string2frame_menu_big("Back", 132, 40, color1, color2);
		}
		if(button2==1) {
		menu_arr.time2 = getMicrotime();
		if(menu_arr.time2>=menu_arr.time1+300000) {
			stripStart = 1;
			if(btn2==24) menu_arr.animation=!menu_arr.animation;
			else if(btn2==48 || btn2==60) menu_arr.led1.staticLight=!menu_arr.led1.staticLight;
			else if(btn2==72) menu_arr.led2.staticLight=!menu_arr.led2.staticLight;
			else if(btn2==96 && menu_arr.size==1) menu_arr.led2.staticLight=!menu_arr.led2.staticLight;
			else if(btn2==96 && menu_arr.size==0) {menu_arr.currentScreen=1; nextscreen=1; menu_arr.led1.simpleLedSetup=' '; menu_arr.led2.simpleLedSetup=' '; stripStart=0;}
			else if(btn2==132 && menu_arr.size==1) {menu_arr.currentScreen=1; nextscreen=1; menu_arr.led1.simpleLedSetup=' '; menu_arr.led2.simpleLedSetup=' '; stripStart=0;}
			else {menu_arr.led1.simpleLedSetup=' '; menu_arr.led2.simpleLedSetup=' '; stripStart=0;}
			menu_arr.time2=0; menu_arr.time1 = getMicrotime();
			}
		}
	}
	else if(menu_arr.currentScreen==2) //effects
	{
		if(menu_arr.size==0)
		{
			string2frame_menu(" -- ", 24, 40, color1, color2);
			string2frame_menu(" -- ", 48, 40, color1, color2);
			string2frame_menu(" -- ", 72, 40, color1, color2);
			string2frame_menu(" -- ", 96, 40, color1, color2);
			string2frame_menu("Back", 120, 40, color1, color2);
		}
		else
		{
			string2frame_menu_big(" -- ", 24, 40, color1, color2);
			string2frame_menu_big(" -- ", 60, 40, color1, color2);
			string2frame_menu_big(" -- ", 96, 40, color1, color2);
			string2frame_menu_big(" -- ", 132, 40, color1, color2);
			string2frame_menu_big("Back", 168, 40, color1, color2);
		}
		if(button2==1) {
		menu_arr.time2 = getMicrotime();
		if(menu_arr.time2>=menu_arr.time1+300000) {
			if(btn2==120 || btn2==168) {menu_arr.currentScreen=0; nextscreen=1;}
			menu_arr.time2=0; menu_arr.time1 = getMicrotime();
			}
		}
	}
	else if(menu_arr.currentScreen==3) //ethernet
	{
		if(menu_arr.size==0)
		{
			string2frame_menu(" -- ", 24, 40, color1, color2);
			string2frame_menu(" -- ", 48, 40, color1, color2);
			string2frame_menu(" -- ", 72, 40, color1, color2);
			string2frame_menu(" -- ", 96, 40, color1, color2);
			string2frame_menu("Back", 120, 40, color1, color2);
		}
		else
		{
			string2frame_menu_big(" -- ", 24, 40, color1, color2);
			string2frame_menu_big(" -- ", 60, 40, color1, color2);
			string2frame_menu_big(" -- ", 96, 40, color1, color2);
			string2frame_menu_big(" -- ", 132, 40, color1, color2);
			string2frame_menu_big("Back", 168, 40, color1, color2);
		}
		if(button2==1) {
		menu_arr.time2 = getMicrotime();
		if(menu_arr.time2>=menu_arr.time1+300000) {
			if(btn2==120 || btn2==168) {menu_arr.currentScreen=0; nextscreen=1;}
			menu_arr.time2=0; menu_arr.time1 = getMicrotime();
			}
		}
	}
	else if(menu_arr.currentScreen==4) // KOREK settings
	{
		if(menu_arr.size==0){
			string2frame_menu("Invert color of GUI", 24, 40, color1, color2);
			string2frame_menu("Change text size", 48, 40, color1, color2);
			string2frame_menu("Back", 72, 40, color1, color2);
		}
		else{
			string2frame_menu_big("Invert color of GUI", 24, 40, color1, color2);
			string2frame_menu_big("Change text size", 60, 40, color1, color2);
			string2frame_menu_big("Back", 96, 40, color1, color2);
		}
		if(button2==1) {
		menu_arr.time2 = getMicrotime();
		if(menu_arr.time2>=menu_arr.time1+300000) {
			if(btn2==24){
				menu_arr.colourGui=!menu_arr.colourGui; 
				delete_lcd(menu_arr.colourGui);
				if(menu_arr.colourGui==1) {color1=0x0000; color2=0xFFFF;}
				else if(menu_arr.colourGui==0) {color1=0xFFFF; color2=0x0000;}
				if(menu_arr.size==0){
					string2frame_menu("Invert color of GUI", 24, 40, color1, color2);
					string2frame_menu("Change text size", 48, 40, color1, color2);
					string2frame_menu("Back", 72, 40, color1, color2);
				}
				else{
					string2frame_menu_big("Invert color of GUI", 24, 40, color1, color2);
					string2frame_menu_big("Change text size", 60, 40, color1, color2);
					string2frame_menu_big("Back", 96, 40, color1, color2);
				}
			}
			else if(btn2==48 || btn2==60){
				menu_arr.size=!menu_arr.size;
				delete_lcd(menu_arr.colourGui);
				if(menu_arr.size==0){
					string2frame_menu("Invert color of GUI", 24, 40, color1, color2);
					string2frame_menu("Change text size", 48, 40, color1, color2);
					string2frame_menu("Back", 72, 40, color1, color2);
				}
				else{
					string2frame_menu_big("Invert color of GUI", 24, 40, color1, color2);
					string2frame_menu_big("Change text size", 60, 40, color1, color2);
					string2frame_menu_big("Back", 96, 40, color1, color2);
				}
			}
			else if(btn2==72 || btn2==96){
				menu_arr.currentScreen=0; 
				nextscreen=1;
			}
			btn2=24;
			menu_arr.time2=0; menu_arr.time1 = getMicrotime();
			}
		}
	}
	else if(menu_arr.currentScreen==5) // Exit
	{
		if(menu_arr.size==0){
			string2frame_menu(" Back ", 24, 40, color1, color2);
			string2frame_menu(" Confirm exit ", 48, 40, color1, color2);
		}
		else{
			string2frame_menu_big(" Back ", 24, 40, color1, color2);
			string2frame_menu_big(" Confirm exit ", 60, 40, color1, color2);
		}
		if(button2==1) {
		menu_arr.time2 = getMicrotime();
		if(menu_arr.time2>=menu_arr.time1+300000) {
			switch(btn2)
			{
				case 24:
					menu_arr.currentScreen=0; 
					nextscreen=1;
					break;
				case 48:
					menu_arr.exit=1; break;
				case 60:
					menu_arr.exit=1; break;
				default:
					break;
			}
			btn2=24;
			menu_arr.time2=0; menu_arr.time1 = getMicrotime();
			}
		}
	}
	return menu_arr;
}



GUI_set_menu strip(int yrow, int xcolumn, int posuvnik1, int posuvnik2, GUI_set_menu menu_arr)
{
	for(int i=0; i<480; i++)
	{
		for(int j=0; j<200; j++)
		{
			if(menu_arr.colourGui==1) frame[j][i] = 0xFFFF;
			else frame[j][i] = 0;
		}
	}

	posuvnik1 = (int)(((double)posuvnik1/255)*360);
	posuvnik2 = (int)(((double)posuvnik2/255)*360);

	if(menu_arr.animation==0) led1_hsv = RGB_to_HSV(menu_arr.led1.red, menu_arr.led1.green, menu_arr.led1.blue);
	
	int x,y;
	for(y=0; y<15; y++)
	{
		for(x=0; x<460; x++)
		{
			if(menu_arr.led1.simpleLedSetup=='h') rgb1 = HSV_to_RGB(((double)x/460)*360, 1, 1);
			else if(menu_arr.led1.simpleLedSetup=='s') rgb1 = HSV_to_RGB(led1_hsv[0], ((double)x/460) , 1);
			else if(menu_arr.led1.simpleLedSetup=='v') rgb1 = HSV_to_RGB(led1_hsv[0], 1, ((double)x/460) );
			else {
				if(menu_arr.colourGui==1) frame[yrow + y][xcolumn + x] = 0xFFFF;
				else frame[yrow + y][xcolumn + x] = 0;
			}
			if(menu_arr.led1.simpleLedSetup!=' ')
			{
				hex1 = RGB_to_hex(rgb1[0], rgb1[1], rgb1[2]);
				if((int)(((double)x/460)*360) >= posuvnik1-1 && (int)(((double)x/460)*360) <= posuvnik1+1)
				{
				 if(menu_arr.led1.simpleLedSetup=='v') frame[yrow + y][xcolumn + x] = 0xFFFF;
				 else frame[yrow + y][xcolumn + x] = 0;
				}
				else frame[yrow + y][xcolumn + x] = hex1;
				if((int)(((double)x/460)*360)==posuvnik1) {
					menu_arr.led1.red = rgb1[0];
					menu_arr.led1.green = rgb1[1];
					menu_arr.led1.blue = rgb1[2];
				}
			}
		}	
	}

	if(menu_arr.animation==0) led2_hsv = RGB_to_HSV(menu_arr.led2.red, menu_arr.led2.green, menu_arr.led2.blue);

	for(y=0; y<15; y++)
	{
		for(x=0; x<460; x++)
		{
			if(menu_arr.led2.simpleLedSetup=='h') rgb2 = HSV_to_RGB(((double)x/460)*360, 1, 1);
			else if(menu_arr.led2.simpleLedSetup=='s') rgb2 = HSV_to_RGB(led2_hsv[0], ((double)x/460) , 1);
			else if(menu_arr.led2.simpleLedSetup=='v') rgb2 = HSV_to_RGB(led2_hsv[0], 1, ((double)x/460) );
			else {
				if(menu_arr.colourGui==1) frame[yrow+16 + y][xcolumn + x] = 0xFFFF;
				else frame[yrow+16 + y][xcolumn + x] = 0;
			}
			if(menu_arr.led2.simpleLedSetup!=' ')
			{
				hex2 = RGB_to_hex(rgb2[0], rgb2[1], rgb2[2]);
				if((int)(((double)x/460)*360) >= posuvnik2-1 && (int)(((double)x/460)*360) <= posuvnik2+1)
				{
				 if(menu_arr.led2.simpleLedSetup=='v') frame[yrow+16 + y][xcolumn + x] = 0xFFFF;
				 else frame[yrow+16 + y][xcolumn + x] = 0;
				}
				else frame[yrow+16 + y][xcolumn + x] = hex2;
				if((int)(((double)x/460)*360)==posuvnik2) {
					menu_arr.led2.red = rgb2[0];
					menu_arr.led2.green = rgb2[1];
					menu_arr.led2.blue = rgb2[2];
				}
			}
		}	
	}
	return menu_arr;
}

void down_control_panel(int L_rotate, int L_push, int M_rotate, int M_push,int R_rotate, int R_push, GUI_set_menu menu_arr)
{
	int color1=0;
	int color2=0;
	if(menu_arr.colourGui==1) {color1=0x0000; color2=0xFFFF;}
	else if(menu_arr.colourGui==0) {color1=0xFFFF; color2=0x0000;}

	//int posun=0;
	char str[255];
	for(int i=0; i<480; i++) {frame[234][i] = color1; frame[235][i] = color1; frame[236][i] = color1;}
	
	/*
	if(menu_arr.animation==0 && menu_arr.currentScreen==11 && menu_arr.size==0 &&0)
	{
		posun = -12+3*string2frame_menu("Led 1 (r, g, b): ", 237, 0, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led1.red);
		string2frame_menu("    ", 237, 160, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu(str, 237, 160, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led1.green);
		string2frame_menu("    ", 237, 210, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu(str, 237, 210, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led1.blue);
		string2frame_menu("    ", 237, 260, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu(str, 237, 260, color1, color2);
		posun = -12+3*string2frame_menu("Led 2 (r, g, b): ", 253, 0, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led2.red);
		string2frame_menu("    ", 253, 160, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu(str, 253, 160, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led2.green);
		string2frame_menu("    ", 253, 210, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu(str, 253, 210, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led2.blue);
		string2frame_menu("    ", 253, 260, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu(str, 253, 260, color1, color2);
	}
	else if(menu_arr.animation==0 && menu_arr.currentScreen==11 && menu_arr.size==1 &&0)
	{
		posun = -12+3*string2frame_menu_big("Led 1 (r, g, b): ", 237, 0, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led1.red);
		string2frame_menu_big("    ", 237, 160, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu_big(str, 237, 160, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led1.green);
		string2frame_menu_big("    ", 237, 210, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu_big(str, 237, 210, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led1.blue);
		string2frame_menu_big("    ", 237, 260, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu_big(str, 237, 260, color1, color2);
		posun = -12+3*string2frame_menu_big("Led 2 (r, g, b): ", 253, 0, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led2.red);
		string2frame_menu_big("    ", 253, 160, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu_big(str, 253, 160, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led2.green);
		string2frame_menu_big("    ", 253, 210, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu_big(str, 253, 210, color1, color2);
		sprintf(str, "%d", (int)menu_arr.led2.blue);
		string2frame_menu_big("    ", 253, 260, color1, color2);
		posun = posun + 9 - 12 + 3*string2frame_menu_big(str, 253, 260, color1, color2);
	}*/


	sprintf(str, "%d", L_push);
	string2frame_menu("    ", 270, 80, color1, color2);
	string2frame_menu(str, 270, 80, color1, color2);
	//sprintf(str, "%d", M_push);
	//string2frame_menu("    ", 270, 200, color1, color2);
	string2frame_menu("Confirm", 270, 200, color1, color2);
	sprintf(str, "%d", R_push);
	string2frame_menu("    ", 270, 350, color1, color2);
	string2frame_menu(str, 270, 350, color1, color2);

	sprintf(str, "%d", L_rotate);
	string2frame_menu("    ", 290, 80, color1, color2);
	string2frame_menu(str, 290, 80, color1, color2);
	//sprintf(str, "%d", M_rotate);
	//string2frame_menu("    ", 290, 200, color1, color2);
	string2frame_menu("Navigate", 290, 200, color1, color2);
	sprintf(str, "%d", R_rotate);
	string2frame_menu("    ", 290, 350, color1, color2);
	string2frame_menu(str, 290, 350, color1, color2);
}
