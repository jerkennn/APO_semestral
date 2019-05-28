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
#include <sys/time.h>


#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"
#include "lcd_menu_lib.h"
#include "write2lcd_lib.h"
#include "convert_lib.h"
#include "led_effects.h"

pthread_mutex_t mtx;
unsigned char *parlcd_mem_base = NULL;
unsigned char *buttons_mem_base = NULL;
unsigned char *leds_mem_base = NULL;
 
 

typedef struct{
	volatile int rk, gk, bk, rb, gb, bb;
	struct{
		double red;
		double green;
		double blue;
		} rgb_1;
	struct{
		double red;
		double green;
		double blue;
		} rgb_2;
	GUI_set_menu menu_arr;
	bool quit;
	} data_t;


void *leds(void *d);
void *buttons(void *d);
void *display(void *d);



int main(int argc, char *argv[])
{
	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  
	buttons_mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	
	if (parlcd_mem_base == NULL)  exit(1);
	parlcd_hx8357_init(parlcd_mem_base); //only after power up

	delete_lcd(menu_arr.colourGui);
	frame2lcd();

	printf("Hello APO\n");
	data_t data = {.quit = false, .rk = 0, .gk = 0, .bk = 0, .rb = 0, .gb = 0, .bb = 0};
	data.menu_arr.currentScreen = 0;
	data.menu_arr.colourGui = 0;
	data.menu_arr.exit = 0;
	
	
	menu_arr.currentScreen = 0; 
	menu_arr.colourGui = 0;
	menu_arr.exit = 0;
	menu_arr.time1 = 0;
	menu_arr.time2 = 0;
	
	menu_arr.led1.simpleLedSetup = ' ';
	menu_arr.led2.simpleLedSetup = ' ';
	
	menu_arr.animation=0;
	
	menu_arr.led1.red = 255;
	menu_arr.led1.green = 0;
	menu_arr.led1.blue = 0;
	
	menu_arr.led2.red = 255;
	menu_arr.led2.green = 0;
	menu_arr.led2.blue = 0;
	
	
	
	data.rgb_1.red = 0;
	data.rgb_1.green = 0;
	data.rgb_1.blue = 0;
	
	data.rgb_2.red = 0;
	data.rgb_2.green = 0;
	data.rgb_2.blue = 0;

	pthread_mutex_init(&mtx, NULL);
	pthread_t threads[3];
	
	pthread_create(&threads[0], NULL, buttons, &data);
	pthread_create(&threads[1], NULL, leds, &data);
	pthread_create(&threads[2], NULL, display, &data);
	
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);

	printf("Goodbye APO\n");

	return 0;
}


void *leds(void *d){
	data_t *data = (data_t *)d;
	int *led_1 = map_phys_address(SPILED_REG_BASE_PHYS + SPILED_REG_LED_RGB1_o, SPILED_REG_SIZE, 0); 
	int *led_2 = map_phys_address(SPILED_REG_BASE_PHYS + SPILED_REG_LED_RGB2_o, SPILED_REG_SIZE, 0);
	

	bool q = false;
	//uint32_t color_1 = 0;
	//uint32_t color_2 = 0;
	//double out_h; 
	volatile double *hsv_1;
	volatile double *hsv_2;
	int animation = 0;
	long int period = 10*1000*1000;
	double h_1 = 0;
	double h_2 = 0;
	long int startTime = 0;
	
	while(!q){
		if(data->rb == 1  && animation == 1)
		{
			animation = 0;
			menu_arr.animation=0;
		}
		if(data->rb == 1 && animation != 1)
		{
			hsv_1 = RGB_to_HSV(data->rgb_1.red, data->rgb_1.green, data->rgb_1.blue);
			h_1 = hsv_1[0];
			hsv_2 = RGB_to_HSV(data->rgb_2.red, data->rgb_2.green, data->rgb_2.blue);
			h_2 = hsv_2[0];
			animation = 1;
			menu_arr.animation=1;
			startTime = getMicrotime();
			  
		}
		if(animation)
		{
			led_animation(led_1, h_1, h_2, period, startTime, 0, 0);
			led_animation(led_2, h_2, h_1, period, startTime, 0, 0);
			//*led_2 = color_1;
		}
		
		q = data->quit;
		
		}
		*led_1 = 0;
	return NULL;
}


void *buttons(void *d){
	data_t *data = (data_t *)d;
	
	volatile uint32_t rgb_knobs_value;

	bool q = false;
	
	while(!q)
	{	
			rgb_knobs_value = *(volatile uint32_t*)(buttons_mem_base + SPILED_REG_KNOBS_8BIT_o);
			
			pthread_mutex_lock(&mtx);
			
			data->bk =  rgb_knobs_value      & 0xFF; // blue knob position
			data->gk = (rgb_knobs_value>>8)  & 0xFF; // green knob position
			data->rk = (rgb_knobs_value>>16) & 0xFF; // red knob position
			
			data->bb = (rgb_knobs_value>>24) & 1;    // blue button
			data->gb = (rgb_knobs_value>>25) & 1;    // green button
			data->rb = (rgb_knobs_value>>26) & 1;    // red buttom
			
			pthread_mutex_unlock(&mtx);
			
			if(data->bb == 1)
			{
				q = true;
				data->quit = q;
			}
			q = data->quit;
		}
		
	printf("\n");
	delete_lcd(0);
	frame2lcd();
	return NULL;
	}

void *display(void *d){
	data_t *data = (data_t *)d;
	
	bool q = false;
	//double *rgb_leds;
	
	while(!q)
	{
		pthread_mutex_lock(&mtx);
		menu_arr = strip(200, 10, data->rk, data->bk, menu_arr); // !!!!!!
		
		data->rgb_1.red = menu_arr.led1.red;
		data->rgb_1.green = menu_arr.led1.green;
		data->rgb_1.blue = menu_arr.led1.blue;
		
		data->rgb_2.red = menu_arr.led2.red;
		data->rgb_2.green = menu_arr.led2.green;
		data->rgb_2.blue = menu_arr.led2.blue;
	
		down_control_panel(0, 0, 0, 0, 0, 0, menu_arr); // !!!!
		
		menu_arr = menu(data->rk, data->gk, data->bk, data->rb, data->gb, data->bb, menu_arr);
		pthread_mutex_unlock(&mtx);
		
		//printf("%d\n", menu_arr.currentScreen);
		//printf("%d\n", menu_arr.exit);
		q = (menu_arr.exit==1 ? true : false);
		data->quit = q;
		frame2lcd();
		
		
		q = q || data->quit;
		}
	return NULL;
}
