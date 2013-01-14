#ifndef WS2812_H
#define WS2812_H

#include <stm32f4xx.h>

#define PWM_BUFFER_SIZE 192
#define FRAMEBUFFER_SIZE 240

uint32_t ws2812_framebuffer[FRAMEBUFFER_SIZE];

void ws2812_init(void);

#endif
