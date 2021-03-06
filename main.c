/*******************************************************************
  KOREK program

  (C) Copyright 2019 by Vojtech Kozel and Martin Rektoris
      e-mail:   kozelvo1@fel.cvut.cz; rektomar@fel.cvut.cz
      license:  MIT license

*******************************************************************/

#define _POSIX_C_SOURCE 200112L
#define _DEFAULT_SOURCE
#define _BSD_SOURCE

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


#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT 9876 

typedef struct{
	double led1_red;
	double led1_green;
	double led1_blue;
	int led1_static;
	int led1_animation;
	int animation1_period;
	int led1_on;
	int led1_off;
	
	double led2_red;
	double led2_green;
	double led2_blue;
	int led2_static;
	int led2_animation;
	int animation2_period;
	int led2_on;
	int led2_off;
	
} udp_data;

#define STRUCT_SIZE (sizeof(udp_data)+8)

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
	int ethernet_mode;  /// -1 for slave, 0 for turned off, 1 for master
	bool quit;
	} data_t;


void *leds_thread(void *d);
void *buttons_thread(void *d);
void *display_thread(void *d);
void *server_thread(void *d);
void *client_thread(void *d);



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
	data.ethernet_mode = 0;
	menu_arr.ethernet_mode = 0;
	
	
	menu_arr.currentScreen = 0; 
	menu_arr.colourGui = 0;
	menu_arr.exit = 0;
	menu_arr.time1 = 0;
	menu_arr.time2 = 0;
	
	menu_arr.led1.simpleLedSetup = ' ';
	menu_arr.led2.simpleLedSetup = ' ';
	
	menu_arr.animation=0;
	menu_arr.led1.staticLight=0;
	
	menu_arr.led1.red = 255;
	menu_arr.led1.green = 0;
	menu_arr.led1.blue = 0;
	menu_arr.led1.periodSet.periodOFF = 0;
	menu_arr.led1.periodSet.periodON = 0;
	menu_arr.led1.periodSet.periodAnime = 1200;
	
	
	menu_arr.led2.red = 255;
	menu_arr.led2.green = 0;
	menu_arr.led2.blue = 0;
	menu_arr.led2.periodSet.periodOFF = 0;
	menu_arr.led2.periodSet.periodON = 0;
	menu_arr.led2.periodSet.periodAnime = 1200;
	

	
	data.rgb_1.red = 0;
	data.rgb_1.green = 0;
	data.rgb_1.blue = 0;
	
	data.rgb_2.red = 0;
	data.rgb_2.green = 0;
	data.rgb_2.blue = 0;

	menu_arr.periodStrip_prev1 = 0;
	menu_arr.periodStrip_prev2 = 0;


	delete_lcd(0);
		
	string2frame_menu_big("KOREK", 100, 180, 0xFFFF, 0);
	string2frame_menu_big("Welcome", 140, 160, 0xFFFF, 0);
	
	frame2lcd();
	sleep(2);

	pthread_mutex_init(&mtx, NULL);
	pthread_t threads[5];
	
	pthread_create(&threads[0], NULL, buttons_thread, &data);
	pthread_create(&threads[1], NULL, leds_thread, &data);
	pthread_create(&threads[2], NULL, display_thread, &data);
	pthread_create(&threads[3], NULL, server_thread, &data);
	pthread_create(&threads[4], NULL, client_thread, &data);
	
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
	pthread_cancel(threads[3]);
	pthread_join(threads[4], NULL);


	delete_lcd(0);
	string2frame_menu_big("KOREK", 100, 180, 0xFFFF, 0);
	string2frame_menu_big("Goodbye", 140, 160, 0xFFFF, 0);
	frame2lcd();
	sleep(1);
	delete_lcd(0);
	frame2lcd();
	
	printf("Goodbye APO\n");

	return 0;
}


void *leds_thread(void *d){
	data_t *data = (data_t *)d;
	int *led_1 = map_phys_address(SPILED_REG_BASE_PHYS + SPILED_REG_LED_RGB1_o, SPILED_REG_SIZE, 0); 
	int *led_2 = map_phys_address(SPILED_REG_BASE_PHYS + SPILED_REG_LED_RGB2_o, SPILED_REG_SIZE, 0);
	

	bool q = false;
	//uint32_t color_1 = 0;
	//uint32_t color_2 = 0;
	//double out_h; 
	volatile double *hsv_1;
	volatile double *hsv_2;
	int prev_animation = 1;
	double h_1 = 0;
	double h_2 = 0;
	short staticLight1 = 0;
	short staticLight2 = 0;
	long int startTime = 0;
	int first = 1;
	
	while(!q){
		if(menu_arr.animation != prev_animation) first = 1;
		if(menu_arr.animation == 1 && first == 1)
		{
			hsv_1 = RGB_to_HSV(data->rgb_1.red, data->rgb_1.green, data->rgb_1.blue);
			h_1 = hsv_1[0];
			hsv_2 = RGB_to_HSV(data->rgb_2.red, data->rgb_2.green, data->rgb_2.blue);
			h_2 = hsv_2[0];	
			menu_arr.animation=1;
			startTime = getMicrotime();
			first = 0;
		}
		if(menu_arr.animation)
		{
			led1_animation(led_1, h_1, h_2, 1000*((int)menu_arr.led1.periodSet.periodAnime), startTime, 1000*((int)menu_arr.led1.periodSet.periodON), 1000*((int)menu_arr.led1.periodSet.periodOFF));
			led2_animation(led_2, h_2, h_1, 1000*((int)menu_arr.led2.periodSet.periodAnime), startTime, 1000*((int)menu_arr.led2.periodSet.periodON), 1000*((int)menu_arr.led2.periodSet.periodOFF));
		}
		else if(!menu_arr.led1.staticLight && !staticLight1 && !menu_arr.led2.staticLight && !staticLight2)
		{
			*led_1 = 0;
			*led_2 = 0;
		}
		

		if(!menu_arr.animation && menu_arr.led1.staticLight)
		{
			staticLight1=1;
			led1_static(led_1, menu_arr.led1.red, menu_arr.led1.green , menu_arr.led1.blue, 1000*((int)menu_arr.led1.periodSet.periodON), 1000*((int)menu_arr.led1.periodSet.periodOFF));
		}
		if(!menu_arr.animation && menu_arr.led2.staticLight)
		{
			staticLight2=1;
			led2_static(led_2, menu_arr.led2.red, menu_arr.led2.green , menu_arr.led2.blue, 1000*((int)menu_arr.led2.periodSet.periodON), 1000*((int)menu_arr.led2.periodSet.periodOFF));
		}
		else if(!menu_arr.led1.staticLight && staticLight1)
		{
			*led_1=0;
			staticLight1=0;
		}
		else if(!menu_arr.led2.staticLight && staticLight2)
		{
			*led_2=0;
			staticLight2=0;
		}
		
		
		prev_animation = menu_arr.animation;
		q = data->quit;
		
	}
		*led_1 = 0;
		*led_2 = 0;
	return NULL;
}


void *buttons_thread(void *d){
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

void *display_thread(void *d){
	data_t *data = (data_t *)d;
	
	bool q = false;
	
	while(!q)
	{
		pthread_mutex_lock(&mtx);
		menu_arr = strip(200, 10, data->rk, data->bk, menu_arr);
		
		
		data->rgb_1.red = menu_arr.led1.red;
		data->rgb_1.green = menu_arr.led1.green;
		data->rgb_1.blue = menu_arr.led1.blue;
		
		data->rgb_2.red = menu_arr.led2.red;
		data->rgb_2.green = menu_arr.led2.green;
		data->rgb_2.blue = menu_arr.led2.blue;
	
		down_control_panel(0, 0, 0, 0, 0, 0, menu_arr);
		
		menu_arr = menu(data->rk, data->gk, data->bk, data->rb, data->gb, data->bb, menu_arr);
		data->ethernet_mode = menu_arr.ethernet_mode;
		
		menu_arr = getPeriod(data->rk, data->gk, data->bk, menu_arr);
		pthread_mutex_unlock(&mtx);
		
		q = (menu_arr.exit==1 ? true : false);
		data->quit = q;
		frame2lcd();
		
		
		q = q || data->quit;
		}
	return NULL;
}

/*
Pri tvorbe sitove komunikace jsme se inspirovali na strance predmetu: https://cw.fel.cvut.cz/wiki/courses/b35apo/tutorials/10/start
a na strance o computer science: https://www.geeksforgeeks.org/udp-server-client-implementation-c/?fbclid=IwAR0fvmbe-kqIjUn9ES4w1IdBAsyzb7Cq6uubefnFyVjpGvGOgecSjjuOEY8
*/

void *server_thread(void *d){

	data_t *data = (data_t *)d;
	int sockfd; 
    char buffer[sizeof(udp_data)]; 
    struct sockaddr_in servaddr, cliaddr; 
      
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr));

    int broadcast = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(1);
    }
      
    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 

    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int len = STRUCT_SIZE;
    int n = STRUCT_SIZE;
    
    bool q = false;
    
    udp_data *input_data = malloc(sizeof(udp_data));
    
    while(!q){
    	
    	if(data->ethernet_mode == 0 || data->ethernet_mode == 1)
    	{
    		q = data->quit;
    		continue;
    	}
    	
	    n = recvfrom(sockfd, (char *)buffer, STRUCT_SIZE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
	    if(n < 0)
		{
			perror("Receive from ");
			}
	    
	    memcpy(input_data, buffer, sizeof(udp_data));
	    
	    //pthread_mutex_lock(&mtx);
	    menu_arr.led1.red = input_data->led1_red;
	    menu_arr.led1.green = input_data->led1_green; 
	    menu_arr.led1.blue = input_data->led1_blue; 
	    menu_arr.led1.staticLight =  input_data->led1_static;
	    menu_arr.animation = input_data->led1_animation;
	    menu_arr.led1.periodSet.periodON = input_data->led1_on;
    	menu_arr.led1.periodSet.periodOFF = input_data->led1_off;
    	menu_arr.led1.periodSet.periodAnime = input_data->animation1_period;
	    
	    
	    menu_arr.led2.red = input_data->led2_red;
	    menu_arr.led2.green = input_data->led2_green; 
	    menu_arr.led2.blue = input_data->led2_blue; 
	    menu_arr.led2.staticLight = input_data->led2_static;
		menu_arr.led2.periodSet.periodON = input_data->led2_on;
    	menu_arr.led2.periodSet.periodOFF = input_data->led2_off;
    	menu_arr.led2.periodSet.periodAnime = input_data->animation2_period;
	    
	    //pthread_mutex_unlock(&mtx);
	    q = data->quit;
    }
    free(input_data);
	return NULL;
}


void *client_thread(void *d)
{
	data_t *data = (data_t *)d;
	int sockfd; 
    char buffer[sizeof(udp_data)]; 

    struct sockaddr_in     servaddr; 
  
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr));

    int broadcast = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
        perror("setsockopt (SO_BROADCAST)");
        exit(1);
    }
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_BROADCAST; 
      
    //int n, len; 
    
    bool q = false;
    udp_data *output_data = malloc(sizeof(udp_data));
    
    while(!q){
    	if(data->ethernet_mode == 0 || data->ethernet_mode == -1)
    	{
    		q = data->quit;
    		continue;
    	}
    	output_data->led1_red =  menu_arr.led1.red;
    	output_data->led1_green =  menu_arr.led1.green;
    	output_data->led1_blue =  menu_arr.led1.blue;
    	output_data->led1_static = menu_arr.led1.staticLight;
    	output_data->led1_animation = menu_arr.animation;
    	output_data->led1_on = menu_arr.led1.periodSet.periodON;
    	output_data->led1_off = menu_arr.led1.periodSet.periodOFF;
    	output_data->animation1_period = menu_arr.led1.periodSet.periodAnime;
    	//output_data->led1_animation = menu_arr.animation;
    	
    	output_data->led2_red = menu_arr.led2.red;
    	output_data->led2_green = menu_arr.led2.green;
    	output_data->led2_blue = menu_arr.led2.blue;
    	output_data->led2_static = menu_arr.led2.staticLight;
    	output_data->led2_animation = menu_arr.animation;
    	output_data->led2_on = menu_arr.led2.periodSet.periodON;
    	output_data->led2_off = menu_arr.led2.periodSet.periodOFF;
    	output_data->animation2_period = menu_arr.led2.periodSet.periodAnime;
    	//	output_data->led2_animation
    	
    	memcpy(buffer, output_data, sizeof(udp_data));
	    sendto(sockfd, (char *)buffer, STRUCT_SIZE, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	    q = data->quit;
    }
  	free(output_data);
    close(sockfd); 
    return NULL;
}
