#define _POSIX_C_SOURCE 200112L

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

#include "lcd_menu_lib.h"
#include "convert_lib.h"
#include "write2lcd_lib.h"

GUI_set_menu menu(int rotate1, int rotate2, int rotate3, int button1, int button2, int button3, GUI_set_menu menu_arr)
{
	btn2 = 24;
	int color1=0;
	int color2=0;
	if(menu_arr.colourGui==1) {color1=0x0000; color2=0xFFFF;}
	else if(menu_arr.colourGui==0) {color1=0xFFFF; color2=0x0000;}

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
			if(menu_arr.currentScreen==6) btn2=24;
			break;
		case 15 ... 20:
			btn2 = 4*24;
			if(menu_arr.currentScreen==5) btn2=24;
			else if(menu_arr.currentScreen==6) btn2=2*24;
			break;
		case 21 ... 25:
			btn2 = 5*24;
			if(menu_arr.currentScreen==5) btn2=2*24;
			else if(menu_arr.currentScreen==6) btn2=24;
			break;
		case 26 ... 30:
			btn2 = 6*24;
			if(menu_arr.currentScreen==0) btn2=1*24;
			else if(menu_arr.currentScreen==5) btn2=3*24;
			else if(menu_arr.currentScreen==6) btn2=2*24;
			break;
		default:
			break;
	}
	
	for(int i=0; i<170; i++)
	{
		for(int j = 0; j<320; j++) frame[i][j] = color2;
	}

	//int posun=0;
	string2frame_menu("******************** KOREK ********************", 0, 0, color1, color2);

	for(int i=1;i<=6;i++) string2frame_menu("   ", i*24, 10, color1, color2);
	string2frame_menu(">>", btn2, 10, color1, color2);
	
	if(menu_arr.currentScreen==0) // start menu
	{
		string2frame_menu("Change LEDs", 24, 40, color1, color2);
		string2frame_menu("Special effects", 48, 40, color1, color2);
		string2frame_menu("Ethernet settings", 72, 40, color1, color2);
		string2frame_menu("KOREK settings", 96, 40, color1, color2);
		string2frame_menu("Exit KOREK", 120, 40, color1, color2);
		if(button2==1) {
		time2 = getMicrotime();
		if(time2>=time1+300000) {
			menu_arr.currentScreen=btn2/24;
			btn2=24;
			time2=0; time1 = getMicrotime();
			}
		}
	}
	else if(menu_arr.currentScreen==1) // led 1 menu
	{
		string2frame_menu("Change hue (H)", 24, 40, color1, color2);
		string2frame_menu("Change saturation (S)", 48, 40, color1, color2);
		string2frame_menu("Change value (V)", 72, 40, color1, color2);
		string2frame_menu("Change period", 96, 40, color1, color2);
		string2frame_menu("Set color shift", 120, 40, color1, color2);
		string2frame_menu("Back", 144, 40, color1, color2);
		if(button2==1) {
		time2 = getMicrotime();
		if(time2>=time1+300000) {
			if(btn2==144) menu_arr.currentScreen=0;
			btn2=24;
			time2=0; time1 = getMicrotime();
			}
		}
	}
	else if(menu_arr.currentScreen==2) //both leds menu
	{
		string2frame_menu(" -- ", 24, 40, color1, color2);
		string2frame_menu(" -- ", 48, 40, color1, color2);
		string2frame_menu(" -- ", 72, 40, color1, color2);
		string2frame_menu(" -- ", 96, 40, color1, color2);
		string2frame_menu(" -- ", 120, 40, color1, color2);
		string2frame_menu("Back", 144, 40, color1, color2);
		if(button2==1) {
		time2 = getMicrotime();
		if(time2>=time1+300000) {
			if(btn2==144) menu_arr.currentScreen=0;
			btn2=24;
			time2=0; time1 = getMicrotime();
			}
		}
	}
	else if(menu_arr.currentScreen==3) //ethernet
	{
		string2frame_menu(" -- ", 24, 40, color1, color2);
		string2frame_menu(" -- ", 48, 40, color1, color2);
		string2frame_menu(" -- ", 72, 40, color1, color2);
		string2frame_menu(" -- ", 96, 40, color1, color2);
		string2frame_menu(" -- ", 120, 40, color1, color2);
		string2frame_menu("Back", 144, 40, color1, color2);
		if(button2==1) {
		time2 = getMicrotime();
		if(time2>=time1+300000) {
			if(btn2==144) menu_arr.currentScreen=0;
			btn2=24;
			time2=0; time1 = getMicrotime();
			}
		}
	}
	else if(menu_arr.currentScreen==4) // KOREK settings
	{
		string2frame_menu("Invert color of GUI", 24, 40, color1, color2);
		string2frame_menu(" -- ", 48, 40, color1, color2);
		string2frame_menu("Back", 72, 40, color1, color2);
		if(button2==1) {
		time2 = getMicrotime();
		if(time2>=time1+300000) {
			switch(btn2)
			{
				case 24:
					menu_arr.colourGui=!menu_arr.colourGui; 
					delete_lcd(menu_arr.colourGui);
					if(menu_arr.colourGui==1) {color1=0x0000; color2=0xFFFF;}
					else if(menu_arr.colourGui==0) {color1=0xFFFF; color2=0x0000;}
					string2frame_menu("Invert color of GUI", 24, 40, color1, color2);
					string2frame_menu(" -- ", 48, 40, color1, color2);
					string2frame_menu("Back", 72, 40, color1, color2);
					break;
				case 72:
					menu_arr.currentScreen=0; break;
				default:
					break;
			}
			btn2=24;
			time2=0; time1 = getMicrotime();
			}
		}
	}
	else if(menu_arr.currentScreen==5) // Exit
	{
		string2frame_menu(" Back ", 24, 40, color1, color2);
		string2frame_menu(" Confirm exit ", 48, 40, color1, color2);
		if(button2==1) {
		time2 = getMicrotime();
		if(time2>=time1+300000) {
			switch(btn2)
			{
				case 24:
					menu_arr.currentScreen=0; break;
				case 48:
					menu_arr.exit=1; break;
				default:
					break;
			}
			btn2=24;
			time2=0; time1 = getMicrotime();
			}
		}
	}
	return menu_arr;
}



GUI_set_menu strip(int yrow, int xcolumn, int posuvnik1, int posuvnik2, GUI_set_menu menu_arr)
{
	posuvnik1 = (int)(((double)posuvnik1/255)*460);
	posuvnik2 = (int)(((double)posuvnik2/255)*460);
	int x,y;
	double *rgb = calloc(6, sizeof(double));
	uint16_t hex;
	double *led1_hsv = RGB_to_HSV(menu_arr.led1.red, menu_arr.led1.green, menu_arr.led1.blue);
	double *led2_hsv = RGB_to_HSV(menu_arr.led2.red, menu_arr.led2.green, menu_arr.led2.blue);

	for(y=0; y<15; y++)
	{
		for(x=0; x<460; x++)
		{
			if(menu_arr.led1.simpleLedSetup=='h') rgb = HSV_to_RGB(((double)x/460)*360, led1_hsv[1], led1_hsv[2]);
			else if(menu_arr.led1.simpleLedSetup=='s') rgb = HSV_to_RGB(led1_hsv[0], ((double)x/460) , led1_hsv[2]);
			else if(menu_arr.led1.simpleLedSetup=='v') rgb = HSV_to_RGB(led1_hsv[0], led1_hsv[1], ((double)x/460) );
			hex = RGB_to_hex(rgb[0], rgb[1], rgb[2]);
			if((int)(((double)x/460)*360) >= posuvnik1-1 && (int)(((double)x/460)*360) <= posuvnik1+1) frame[yrow + y][xcolumn + x] = 0;
			else frame[yrow + y][xcolumn + x] = hex;
			if((int)(((double)x/460)*360)==posuvnik1) {
				menu_arr.led1.red = rgb[0];
				menu_arr.led1.green = rgb[1];
				menu_arr.led1.blue = rgb[2];
			} 
		}	
	}

	for(y=0; y<15; y++)
	{
		for(x=0; x<460; x++)
		{
			if(menu_arr.led2.simpleLedSetup=='h') rgb = HSV_to_RGB(((double)x/460)*360, led2_hsv[1], led2_hsv[2]);
			else if(menu_arr.led2.simpleLedSetup=='s') rgb = HSV_to_RGB(led2_hsv[0], ((double)x/460) , led2_hsv[2]);
			else if(menu_arr.led2.simpleLedSetup=='v') rgb = HSV_to_RGB(led2_hsv[0], led2_hsv[1], ((double)x/460) );
			hex = RGB_to_hex(rgb[0], rgb[1], rgb[2]);
			if((int)(((double)x/460)*360) >= posuvnik2-1 && (int)(((double)x/460)*360) <= posuvnik2+1) frame[yrow+16 + y][xcolumn + x] = 0;
			else frame[yrow+16 + y][xcolumn + x] = hex;
			if((int)(((double)x/460)*360)==posuvnik2) {
				menu_arr.led2.red = rgb[0];
				menu_arr.led2.green = rgb[1];
				menu_arr.led2.blue = rgb[2];
			} 
		}	
	}
	free(rgb);
	return menu_arr;
}

void down_control_panel(int L_rotate, int L_push, int M_rotate, int M_push,int R_rotate, int R_push, GUI_set_menu menu_arr)
{
	int color1=0;
	int color2=0;
	if(menu_arr.colourGui==1) {color1=0x0000; color2=0xFFFF;}
	else if(menu_arr.colourGui==0) {color1=0xFFFF; color2=0x0000;}

	int posun=0;
	char str[255];
	for(int i=0; i<480; i++) {frame[234][i] = color1; frame[235][i] = color1; frame[236][i] = color1;}
	
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

