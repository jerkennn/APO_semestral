#define _POSIX_C_SOURCE 200112L

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "font_types.h"

#include "convert_lib.h"
#include "led_effects.h"


uint32_t createRGB(int r, int g, int b)
{   
    return (((r)*0x10000) + ((g )*0x100) + (b));
}

void led_animation(int *led, double h_1, double h_2, long int period, long int startTime, int on_time, int off_time){
		
		long int start_time = 0;
		long int current_time = 0;
		
		double c1 = 2*((getMicrotime() - startTime)%(period));
		double c2 = (double)(period);
		double absMe = (1-c1/c2); 
		if(absMe<0){absMe = -absMe;}
		double coef = 1-absMe;
		double out_h = h_1 + (h_2 - h_1)*coef; 
		double *rgb_led_1 = HSV_to_RGB(out_h , 1, 1);
		uint32_t color = createRGB(rgb_led_1[0], rgb_led_1[1], rgb_led_1[2]);
		
		if(on_time == 0 || off_time == 0)
		{
			*led = color;
		}
		/*else
		{
			if(time >= on_time && on)
			{
				*led = 0;
				on = 0;
			}
			else if(time < on_time && on)
			{
				*led = color;
			}
			else if(time >= off_time && !on)
			{
				on = 1; 
			}
			
		}*/
	}
	
void led_static(int *led, int red, int green, int blue, int on_time, int off_time){
	long int start_time = 0;
	long int current_time = 0;
	uint32_t color = createRGB(red, green, blue);
	if(on_time == 0 || off_time == 0)
	{
	
		*led = color;
	}
	else
	{
		
		
	}
}

