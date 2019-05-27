#ifndef _LCD_MENU_LIB_H_
#define _LCD_MENU_LIB_H_
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <sys/time.h>

#define FRAME_W 480
#define FRAME_H 320

#ifdef __cplusplusplus
extern "C"
#endif

typedef struct led_set
{
    double red;
    double green;
    double blue;
    double period;
    char simpleLedSetup;
}led_settings;

typedef struct posuvnik_set
{
    double h;
    double s;
    double v;
}posuvnik_settings;

typedef struct GUI_set
{
    unsigned long long time1;
    unsigned long long time2;

    int currentScreen;
    int colourGui;
    int exit;
    int animation1;
    int animation2;
    
    led_settings led1;
    led_settings led2;
    
    posuvnik_settings posuvnik_up;
    posuvnik_settings posuvnik_down;
}GUI_set_menu;

GUI_set_menu menu_arr;

// int btn1=0;
int btn2;
// int btn3=0;

int posuvnik1;
int posuvnik2;
double posuvnik1_shift;
double posuvnik2_shift;

double *rgb1;
double *rgb2;
uint16_t hex1;
uint16_t hex2;

unsigned char *parlcd_mem_base;

uint16_t frame[FRAME_H][FRAME_W];

/* GUI_set: obrazovka GUI; barva gui; exit */
GUI_set_menu menu(int rotate1, int rotate2, int rotate3, int button1, int button2, int button3, GUI_set_menu menu_arr);
void down_control_panel(int L_rotate, int L_push, int M_rotate, int M_push,int R_rotate, int R_push, GUI_set_menu menu_arr);

//double* strip(int yrow, int xcolumn, int posuvnik1, int posuvnik2, int convert_lcd);
GUI_set_menu strip(int yrow, int xcolumn, double posuvnik1_vstup, double posuvnik2_vstup, GUI_set_menu menu_arr);

#ifdef __cplusplus

#endif

#endif
