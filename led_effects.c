#define _POSIX_C_SOURCE 200112L

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

#include "convert_lib.h"
#include "led_effects.h"

int first_1_a = 1;
int on_1_a = 1;

int first_1 = 1;
int on_1 = 1;

int first_2_a = 1;
int on_2_a = 1;

int first_2 = 1;
int on_2 = 1;


uint32_t createRGB(int r, int g, int b)
{   
    return (((r)*0x10000) + ((g )*0x100) + (b));
}

void led1_animation(int *led, double h_1, double h_2, long int period, long int startTime, int on_time, int off_time){
		
		static long int start_time;
		if(period==0) period=1;
		
		double c1 = 2*((getMicrotime() - startTime)%(period));
		double c2 = (double)(period);
		double absVal = (1-c1/c2); 
		if(absVal<0){absVal = -absVal;}
		double coef = 1-absVal;
		double out_h = h_1 + (h_2 - h_1)*coef; 
		double *rgb_led_1 = HSV_to_RGB(out_h , 1, 1);
		uint32_t color = createRGB(rgb_led_1[0], rgb_led_1[1], rgb_led_1[2]);
		
		if(on_time == 0 || off_time == 0)
		{
			*led = color;
		}
		else
		{
			if(first_1_a)
			{
				start_time = getMicrotime();
				*led = color;
				first_1_a = 0;
			}
			if(on_1_a &&(getMicrotime()-start_time >= on_time))
			{
				*led = 0;
				on_1_a = 0;
			}
			else if(on_1_a && (getMicrotime() - start_time < on_time))
			{
				*led = color;
			}
			else if(!on_1_a && (getMicrotime() - start_time >= on_time + off_time))
			{
				on_1_a = 1;
				start_time = getMicrotime();
			}
			
		}
	}
	
void led1_static(int *led, int red, int green, int blue, int on_time, int off_time){ //led1_static(led1, r, g ,b, 0, 0)
	static long int start_time;
	uint32_t color = createRGB(red, green, blue);
	if(on_time == 0 || off_time == 0)
	{
	
		*led = color;
	}
	else
	{
		if(first_1)
		{
			start_time = getMicrotime();
			*led = color;
			first_1 = 0;
		}
		if(on_1 &&(getMicrotime()-start_time >= on_time))
		{
			*led = 0;
			on_1 = 0;
		}
		else if(on_1 && (getMicrotime() - start_time < on_time))
		{
			*led = color;
		}
		else if(!on_1 && (getMicrotime() - start_time >= on_time + off_time))
		{
			on_1 = 1;
			start_time = getMicrotime();
		}
		
	}
}

void led2_animation(int *led, double h_1, double h_2, long int period, long int startTime, int on_time, int off_time){
		
		static long int start_time;
		if(period==0) period=1;
		double c1 = 2*((getMicrotime() - startTime)%(period));
		double c2 = (double)(period);
		double absVal = (1-c1/c2); 
		if(absVal<0){absVal = -absVal;}
		double coef = 1-absVal;
		double out_h = h_1 + (h_2 - h_1)*coef; 
		double *rgb_led_1 = HSV_to_RGB(out_h , 1, 1);
		uint32_t color = createRGB(rgb_led_1[0], rgb_led_1[1], rgb_led_1[2]);
		
		if(on_time == 0 || off_time == 0)
		{
			*led = color;
		}
		else
		{
			if(first_2_a)
			{
				start_time = getMicrotime();
				*led = color;
				first_2_a = 0;
			}
			if(on_2_a &&(getMicrotime()-start_time >= on_time))
			{
				*led = 0;
				on_2_a = 0;
			}
			else if(on_2_a && (getMicrotime() - start_time < on_time))
			{
				*led = color;
			}
			else if(!on_2_a && (getMicrotime() - start_time >= on_time + off_time))
			{
				on_2_a = 1;
				start_time = getMicrotime();
			}
			
		}
	}
	
void led2_static(int *led, int red, int green, int blue, int on_time, int off_time){
	static long int start_time = 0;
	uint32_t color = createRGB(red, green, blue);
	if(on_time == 0 || off_time == 0)
	{
	
		*led = color;
	}
	else
	{
		if(first_2)
		{
			start_time = getMicrotime();
			*led = color;
			first_2 = 0;
		}
		if(on_2 &&(getMicrotime()-start_time >= on_time))
		{
			*led = 0;
			on_2 = 0;
		}
		else if(on_2 && (getMicrotime() - start_time < on_time))
		{
			*led = color;
		}
		else if(!on_2 && (getMicrotime() - start_time >= on_time + off_time))
		{
			on_2 = 1;
			start_time = getMicrotime();
		}
		
	}
}

