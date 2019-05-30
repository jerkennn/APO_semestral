#define _POSIX_C_SOURCE 200112L

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

#include "lcd_menu_lib.h"
#include "convert_lib.h"
#include "write2lcd_lib.h"


unsigned char *parlcd_mem_base;

uint16_t frame[FRAME_H][FRAME_W];

/*
 * frame2lcd() Function was taken from practical exercise APO (by Ing. Susta, Ph.D.)
*/
void frame2lcd()
{
	parlcd_write_cmd(parlcd_mem_base,0x2c); // to 0,0
	
    volatile uint32_t * ptr = (volatile uint32_t *) frame;
    volatile uint32_t * dest = (volatile uint32_t *)(parlcd_mem_base + PARLCD_REG_DATA_o);
    
    int i;
    for(i=0; i<(320*480)/2; i++) *dest = *ptr++;
}

/*
 * string2frame_menu() The function was compiled on the basis of knowledge gained from practical exercise APO (by Ing. Susta, Ph.D.)
*/
int string2frame_menu(char *s, int yrow, int xcolumn, uint16_t forecolor, uint16_t backcolor)
{
	int w = 0;
	int p = 0;
	int strlen_ = strlen(s);
	for(int c_i=0; c_i < strlen_; c_i++)
	{
		int c=s[c_i];
		int cix = c-' ';
		
		const uint16_t * ptr = font_winFreeSystem14x16.bits+cix*16;
		
		int x, y;
		xcolumn = xcolumn + w;
		w = font_winFreeSystem14x16.width[cix]; 
		for(y=0; y<16; y++)
		{
			uint16_t mask = *ptr++;
			
			for(x=0; x<w+4; x++)
			{
				frame[yrow + y][xcolumn + x] = (mask & 0x8000) ? forecolor : backcolor;
				mask <<= 1;
			}
		}
		w+=4;
		p+=4;
	}
	return p;
}

int string2frame_menu_big(char *s, int yrow, int xcolumn, uint16_t forecolor, uint16_t backcolor)
{
	int w = 0;
	int p = 0;
	int strlen_ = strlen(s);
	for(int c_i=0; c_i < strlen_; c_i++)
	{
		int c=s[c_i];
		int cix = c-' ';
		
		const uint16_t * ptr = font_winFreeSystem14x16.bits+cix*16;
		
		int x, y;
		xcolumn = xcolumn + w;
		w = font_winFreeSystem14x16.width[cix]*2; 
		for(y=0; y<32; y+=2)
		{
			uint16_t mask = *ptr++;
			
			for(x=0; x<w+4; x+=2)
			{
				frame[yrow + y][xcolumn + x] = (mask & 0x8000) ? forecolor : backcolor;
				frame[yrow + y][xcolumn + x +1] = (mask & 0x8000) ? forecolor : backcolor;
				frame[yrow + y+1][xcolumn + x] = (mask & 0x8000) ? forecolor : backcolor;
				frame[yrow + y+1][xcolumn + x +1] = (mask & 0x8000) ? forecolor : backcolor;
				mask <<= 1;
			}
		}
		w+=4;
		p+=4;
	}
	return p;
}

void delete_lcd(int contrast_lcd)
{
    int color=0;
	if(contrast_lcd==0) color=0x0000;
	else if(contrast_lcd==1) color=0xFFFF;

	for(int i = 0; i<320; i++)
	{
		for(int j=0; j<480; j++) frame[i][j]=color;
	}
}

