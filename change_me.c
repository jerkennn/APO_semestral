
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

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <termios.h>


#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "lcd_menu_lib.h"

pthread_mutex_t mtx;
unsigned char *parlcd_mem_base = NULL;
unsigned char *buttons_mem_base = NULL;
unsigned char *leds_mem_base = NULL;
 
 
uint32_t createRGB(int r, int g, int b)
{   
    return (((r)*0x10000) + ((g )*0x100) + (b));
}

typedef struct{
	volatile int rk, gk, bk, rb, gb, bb;
	//volatile int r_step, g_step, b_step;
	//volatile int pos;
	struct{
		int red;
		int green;
		int blue;
		} rgb;
	
	bool quit;
	} data_t;

/*
void call_termios(int reset) {
   static struct termios tio, tioOld;
   tcgetattr(STDIN_FILENO, &tio);
   if (reset) {
      tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
   } else {
      tioOld = tio;
      cfmakeraw(&tio);
      tcsetattr(STDIN_FILENO, TCSANOW, &tio);
   }
}*/
void *leds(void *d);
void *buttons(void *d);

int main(int argc, char *argv[])
{
	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  
	buttons_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	
	
  
	if (parlcd_mem_base == NULL)  exit(1);
	parlcd_hx8357_init(parlcd_mem_base); //only after power up
	//double *arr = 


	printf("Hello APO\n");
	data_t data = {.quit = false, .rk = 0, .gk = 0, .bk = 0, .rb = 0, .gb = 0, .bb = 0};
	data.rgb.red = 0;
	data.rgb.green = 0;
	data.rgb.blue = 0;

	pthread_mutex_init(&mtx, NULL);
	pthread_t threads[2];
	
	pthread_create(&threads[0], NULL, buttons, &data);
	pthread_create(&threads[1], NULL, leds, &data);
	
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);

	printf("Goodbye APO\n");

	return 0;
}

void *leds(void *d){
	data_t *data = (data_t *)d;
	int *led_1 = map_phys_address(SPILED_REG_BASE_PHYS + SPILED_REG_LED_RGB1_o, SPILED_REG_SIZE, 0); 
	
	//unsigned char *mem_base;
	//mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	//if (mem_base == NULL) exit(1);
	
	bool q = false;
	uint32_t color_1;
	//uint32_t color_2; 
	//double *my_rgb = HSV_to_RGB((((double)data->rk/255)*360), 1, 1);
	while(!q)
		{
		//printf("%lf %lf %lf\n", my_rgb[0], my_rgb[1], my_rgb[2]);
		color_1 = createRGB(data->rgb.red, data->rgb.green, data->rgb.blue); 
		//printf("%d %d %d \n", data->rgb.red, data->rgb.green, data->rgb.blue);
		//color_2 = color_1 ;
		
		//*(volatile uint32_t*)(leds_mem_base + SPILED_REG_LED_RGB1_o) = color_1;
		*led_1 = color_1;
		//*(volatile uint32_t*)(leds_mem_base + SPILED_REG_LED_RGB2_o) = color_2;
		q = data->quit;
		}
	return NULL;
	}
void *buttons(void *d){
	data_t *data = (data_t *)d;
	
	//bumem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
//	if (mem_base == NULL) exit(1);
	
	volatile uint32_t rgb_knobs_value;

	bool q = false;
			
	int rk, gk, bk;
	rk = 0;
	gk = 0;
	bk = 0;	
	double *rgb_leds;
	while(!q)
	{	
			rgb_leds = strip(200, 10, data->rk, data->bk); // !!!!!!
			data->rgb.red = rgb_leds[0];
			data->rgb.green = rgb_leds[1];
			data->rgb.blue = rgb_leds[2];
			down_controll_panel(0, 0, 0, 0, 0, 0, rgb_leds); // !!!!!

			menu(data->rk, data->gk, data->bk, data->rb, data->gb, data->bb);
			frame2lcd();
			rgb_knobs_value = *(volatile uint32_t*)(buttons_mem_base + SPILED_REG_KNOBS_8BIT_o);
			pthread_mutex_lock(&mtx);
			
			bk =  rgb_knobs_value      & 0xFF; // blue knob position
			gk = (rgb_knobs_value>>8)  & 0xFF; // green knob position
			rk = (rgb_knobs_value>>16) & 0xFF; // red knob position
			
			/*data->b_step = -data->bk +bk;
			data->g_step = -data->gk + gk;
			data->r_step = -data->rk + rk;
			data->pos += data->b_step;*/
			
			data->bk = bk;
			data->gk = gk;
			data->rk = rk;
			
			data->rb = (rgb_knobs_value>>24) & 1;    // blue button
			data->gb = (rgb_knobs_value>>25) & 1;    // green button
			data->bb = (rgb_knobs_value>>26) & 1;    // red buttom
			pthread_mutex_unlock(&mtx);
			

			//printf("%d %d %d\n ", data->rk, data->gk, data->bk);
			//printf("%d %d %d\n", data->b_step, data->g_step, data->b_step);
			char c = getchar();
			if(c == 'q')
			{
				q = true;
				}
			if(data->bb == 1 && data->rb == 1 && data->gb == 1)
			{
				q = true;
			}
			data->quit = q;
		}
	printf("\n");
	delete_lcd();
	frame2lcd();
	return NULL;
	}


