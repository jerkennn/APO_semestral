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

typedef struct period_st
{
    double periodON;
    double periodOFF;
    double periodAnime;
}period_setting;

typedef struct led_set
{
    double red;
    double green;
    double blue;
    period_setting periodSet;
    char simpleLedSetup;
    short staticLight;
}led_settings;


typedef struct GUI_set
{
    int currentScreen;
    int colourGui;
    int exit;
    led_settings led1;
    led_settings led2;
    long time1;
    long time2;
    int animation;
    int size;
    int periodStrip_prev1;
    int periodStrip_prev2;
    int periodStrip_prev3;
}GUI_set_menu;

GUI_set_menu menu_arr;

// int btn1=0;
int btn2;
// int btn3=0;

double *rgb1;
double *rgb2;
uint16_t hex1;
uint16_t hex2;

double *led1_hsv;
double *led2_hsv;


unsigned char *parlcd_mem_base;

uint16_t frame[FRAME_H][FRAME_W];

/* GUI_set: obrazovka GUI; barva gui; exit */
GUI_set_menu menu(int rotate1, int rotate2, int rotate3, int button1, int button2, int button3, GUI_set_menu menu_arr);

GUI_set_menu strip(int yrow, int xcolumn, int posuvnik1, int posuvnik2, GUI_set_menu menu_arr);
GUI_set_menu getPeriod(int posuvnik1, int posuvnik2, int posuvnik3, GUI_set_menu menu_arr);

void down_control_panel(int L_rotate, int L_push, int M_rotate, int M_push,int R_rotate, int R_push, GUI_set_menu menu_arr);

#ifdef __cplusplus

#endif

#endif
