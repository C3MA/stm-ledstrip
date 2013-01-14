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
	
	uint32_t pos = 0;
	delay_ms(200);

	
	while (1) {
		pos %= FRAMEBUFFER_SIZE;
		ws2812_framebuffer[pos]			= 0x00FF0000;
		if(pos>0) ws2812_framebuffer[pos-1] 	= 0x00400000;
		if(pos>1) ws2812_framebuffer[pos-2] 	= 0x00100000;
		delay_ms(15);
		ws2812_framebuffer[pos] = 0;
		if(pos>0) ws2812_framebuffer[pos-1] = 0;
		if(pos>1) ws2812_framebuffer[pos-2] = 0;
		pos++;
	};
	
}



