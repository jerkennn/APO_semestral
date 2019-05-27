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

void led_animation(int *led, double h_1, double h_2, long int period, long int startTime){
		double c1 = 2*((getMicrotime() - startTime)%(period));
		double c2 = (double)(period);
		double absMe = (1-c1/c2); 
		if(absMe<0){absMe = -absMe;}
		double coef = 1-absMe;
		double out_h = h_1 + (h_2 - h_1)*coef; 
		double *rgb_led_1 = HSV_to_RGB(out_h , 1, 1);
		uint32_t color_1 = createRGB(rgb_led_1[0], rgb_led_1[1], rgb_led_1[2]);
		*led = color_1;
	}
