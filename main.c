#include "stm32f4xx_conf.h"
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_dma.h>
#include "delay.h"
#include "ws2812.h"

int main(void)
{  
	       	
	SystemInit();
	init_systick();
	ws2812_init();

	uint32_t pos_r = 40;
	uint32_t pos_g = 20;
	uint32_t pos_b = 0;
	delay_ms(20);
	while (1) {
		pos_r %= FRAMEBUFFER_SIZE;
		pos_g %= FRAMEBUFFER_SIZE;
		pos_b %= FRAMEBUFFER_SIZE;
		ws2812_framebuffer[pos_r].red = 255;
		if(pos_r>0) ws2812_framebuffer[pos_r-1].red = 64;
		if(pos_r>1)ws2812_framebuffer[pos_r-2].red = 10;
		
		ws2812_framebuffer[pos_g].green = 255;
		if(pos_g>0) ws2812_framebuffer[pos_g-1].green = 64;
		if(pos_g>1) ws2812_framebuffer[pos_g-2].green = 10;
		
		ws2812_framebuffer[pos_b].blue = 255;;
		if(pos_b>0) ws2812_framebuffer[pos_b-1].blue = 64;
		if(pos_b>1) ws2812_framebuffer[pos_b-2].blue = 10;

		delay_ms(20);

		ws2812_framebuffer[pos_r].red = 0;
		if(pos_r>0) ws2812_framebuffer[pos_r-1].red = 0;
		if(pos_r>1) ws2812_framebuffer[pos_r-2].red = 0;
		pos_r++;

		ws2812_framebuffer[pos_g].green = 0;
		if(pos_g>0) ws2812_framebuffer[pos_g-1].green = 0;
		if(pos_g>1) ws2812_framebuffer[pos_g-2].green = 0;
		pos_g++;

		ws2812_framebuffer[pos_b].blue = 0;
		if(pos_b>0) ws2812_framebuffer[pos_b-1].blue = 0;
		if(pos_b>1) ws2812_framebuffer[pos_b-2].blue = 0;
		pos_b++;
	};
	
}



