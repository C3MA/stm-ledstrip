#ifndef WS2812_H
#define WS2812_H

#include <stm32f4xx.h>

//enter number of LEDs here!
#define FRAMEBUFFER_SIZE 240



//Size of Ring-Buffer
#define PWM_BUFFER_SIZE 192


struct led {
	unsigned int red : 8;
	unsigned int green : 8;
	unsigned int blue : 8;
	};
	

struct led ws2812_framebuffer[FRAMEBUFFER_SIZE];


void ws2812_init(void);

#endif
