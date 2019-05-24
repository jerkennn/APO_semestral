#ifndef _LCDFRAME_H_
#define _LCDFRAME_H_
#include <stdint.h>

#define FRAME_W 480
#define FRAME_H 320

#ifdef __cplusplusplus
extern "C" {
#endif

unsigned char *parlcd_mem_base;

uint16_t frame[FRAME_H][FRAME_W];

void frame2lcd();
int char2frame(char c, int yrow, int xcolumn, uint16_t forecolor, uint16_t backcolor);

int string2frame(char *s, int yrow, int xcolumn, uint16_t forecolor, uint16_t backcolor);

#ifdef __cplusplus
}

#endif

#endif
