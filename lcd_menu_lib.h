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

//#define MIN(a, b) (a <= b ? a : b)
//#define MAX(a, b) (a >= b ? a : b)

//static int btn1=0;
int btn2;
//static int btn3=0;

long time1;
long time2;

unsigned char *parlcd_mem_base;

uint16_t frame[FRAME_H][FRAME_W];

void frame2lcd();

int string2frame_menu(char *s, int yrow, int xcolumn, uint16_t forecolor, uint16_t backcolor);
void menu(int rotate1, int rotate2, int rotate3, int button1, int button2, int button3);

//double* HSV_to_RGB(double h, double s, double v);
//double* RGB_to_HSV(double r, double g, double b);

//uint16_t RGB_to_hex(double r, double g, double b);

double* strip(int yrow, int xcolumn, int posuvnik1, int posuvnik2);
void down_controll_panel(int L_rotate, int L_push, int M_rotate, int M_push,int R_rotate, int R_push, double*leds);
void delete_lcd();

//long getMicrotime();

#ifdef __cplusplus

#endif

#endif
