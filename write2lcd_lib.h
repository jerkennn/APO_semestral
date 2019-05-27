#ifndef _WRITE2LCD_LIB_H_
#define _WRITE2LCD_LIB_H_
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

unsigned char *parlcd_mem_base;

uint16_t frame[FRAME_H][FRAME_W];

int string2frame_menu(char *s, int yrow, int xcolumn, uint16_t forecolor, uint16_t backcolor);
int string2frame_menu_big(char *s, int yrow, int xcolumn, uint16_t forecolor, uint16_t backcolor);

void frame2lcd();
void delete_lcd(int convert_lcd);

#ifdef __cplusplus

#endif

#endif
