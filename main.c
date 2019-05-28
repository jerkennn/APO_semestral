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
  
#define PORT 8090 

typedef struct{
	double red;
	double green;
	double blue;
	int led1_static;
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
	menu_arr.led1.periodSet.periodAnime = 1;
	
	
	menu_arr.led2.red = 255;
	menu_arr.led2.green = 0;
	menu_arr.led2.blue = 0;
	menu_arr.led2.periodSet.periodOFF = 0;
	menu_arr.led2.periodSet.periodON = 0;
	menu_arr.led2.periodSet.periodAnime = 1;
	

	
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
	pthread_t threads[4];
	
	pthread_create(&threads[0], NULL, buttons_thread, &data);
	pthread_create(&threads[1], NULL, leds_thread, &data);
	pthread_create(&threads[2], NULL, display_thread, &data);
	pthread_create(&threads[3], NULL, server_thread, &data);
	
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_join(threads[2], NULL);
	pthread_join(threads[3], NULL);


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
	//long int period = 8*1000*1000;
	double h_1 = 0;
	double h_2 = 0;
	short staticLight1 = 0;
	short staticLight2 = 0;
	long int startTime = 0;
	int first = 1;
	
	while(!q){
		/*if(data->rb == 1  && menu_arr.animation == 1)
		{
			animation = 0;
			menu_arr.animation=0;
		}*/
		if(menu_arr.animation != prev_animation) first = 1;
		if(menu_arr.animation == 1 && first == 1)
		{
			hsv_1 = RGB_to_HSV(data->rgb_1.red, data->rgb_1.green, data->rgb_1.blue);
			h_1 = hsv_1[0];
			hsv_2 = RGB_to_HSV(data->rgb_2.red, data->rgb_2.green, data->rgb_2.blue);
			h_2 = hsv_2[0];
			//animation = 1; 	
			menu_arr.animation=1;
			startTime = getMicrotime();
			first = 0;
		}
		if(menu_arr.animation)
		{
			if(menu_arr.led1.periodSet.periodAnime==0) menu_arr.led1.periodSet.periodAnime=1;
			if(menu_arr.led2.periodSet.periodAnime==0) menu_arr.led2.periodSet.periodAnime=1;
			led1_animation(led_1, h_1, h_2, 1000*((int)menu_arr.led1.periodSet.periodAnime), startTime, 1000*((int)menu_arr.led1.periodSet.periodON), 1000*((int)menu_arr.led1.periodSet.periodOFF));
			led2_animation(led_2, h_2, h_1, 1000*((int)menu_arr.led2.periodSet.periodAnime), startTime, 1000*((int)menu_arr.led2.periodSet.periodON), 1000*((int)menu_arr.led2.periodSet.periodOFF));
			//*led_2 = color_1;
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
		
		menu_arr = getPeriod(data->rk, data->gk, data->bk, menu_arr);
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
void *server_thread(void *d){

	data_t *data = (data_t *)d;
	int sockfd; 
    char buffer[sizeof(udp_data)]; 
	//char *hello = "Hello from server"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
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
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int len;
    int n;
    
    bool q = false;
    
    udp_data *input_data = malloc(sizeof(udp_data));
    
    while(!q){
	    n = recvfrom(sockfd, (char *)buffer, STRUCT_SIZE, MSG_WAITALL, (struct sockaddr *) &cliaddr, &len);
	    memcpy(input_data, buffer, sizeof(udp_data));
	    menu_arr.led1.red = input_data->red;
	    menu_arr.led1.green = input_data->green; 
	    menu_arr.led1.blue = input_data->blue; 
	    menu_arr.led1.staticLight =  input_data->led1_static;
	    //buffer[n] = '\0'; 
	    //printf("Client : %s\n", buffer);
	    q = data->quit;
    }
    free(input_data);
	return NULL;
}

/*
void *client_thread(void *d)
{
	data_t *data = (data_t *)d;
	int sockfd; 
    char buffer[sizeof(udp_data)]; 

    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
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
    	memcpy(buffer, output_data, sizeof(udp_data));
	    sendto(sockfd, (char *)buffer, STRUCT_SIZE, MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
	    printf("Hello message sent.\n");
	    q = data->quit;
    }
  	free(output_data);
    close(sockfd); 
    return NULL;
}*/
