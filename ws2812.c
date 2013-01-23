#include "ws2812.h"
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_dma.h>


static uint16_t PWM_Buffer[PWM_BUFFER_SIZE];
uint32_t frame_pos = 0;
int incomplete_return = 0; 

void Update_Buffer(uint16_t* buffer);

static void start_dma(void)
{
	static DMA_InitTypeDef dma_init =
	{
			.DMA_BufferSize 	= PWM_BUFFER_SIZE,
			.DMA_Channel 		= DMA_Channel_5,
			.DMA_DIR 		= DMA_DIR_MemoryToPeripheral,
			.DMA_FIFOMode 		= DMA_FIFOMode_Disable,
			.DMA_FIFOThreshold 	= DMA_FIFOThreshold_HalfFull,
			.DMA_Memory0BaseAddr 	= (uint32_t) PWM_Buffer,
			.DMA_MemoryBurst 	= DMA_MemoryBurst_Single,
			.DMA_MemoryDataSize 	= DMA_MemoryDataSize_HalfWord,
			.DMA_MemoryInc 		= DMA_MemoryInc_Enable,
			.DMA_Mode 		= DMA_Mode_Circular,
			.DMA_PeripheralBaseAddr = (uint32_t) &TIM3->CCR4,
			.DMA_PeripheralBurst 	= DMA_PeripheralBurst_Single,
			.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord,
			.DMA_PeripheralInc 	= DMA_PeripheralInc_Disable,
			.DMA_Priority 		= DMA_Priority_Medium
	};

	DMA_Init(DMA1_Stream2, &dma_init);
	DMA_Cmd(DMA1_Stream2, ENABLE);
	TIM_DMACmd(TIM3, TIM_DMA_CC4, ENABLE);
}

static void init_buffers(void)
{
	for(int i = 0; i < PWM_BUFFER_SIZE; i++)
	{
		PWM_Buffer[i] = 0;
	}
	for(int i = 0; i < FRAMEBUFFER_SIZE; i++)
	{
		ws2812_framebuffer[i].red = 0;
		ws2812_framebuffer[i].green = 0;
		ws2812_framebuffer[i].blue = 0;
	}
}

void ws2812_init() {
	init_buffers();

	//InitStructures...
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	TIM_OCInitTypeDef TIM_OC_InitStructure;
	NVIC_InitTypeDef nvic_init;

	//Clock für GPIO setzen
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	//Clock für TIM4 setzen
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//GPIO_PIN konfigurieren

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//GPIO Alternate function verbinden
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
	
	TIM_TimeBase_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBase_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBase_InitStructure.TIM_Period = 104;
	TIM_TimeBase_InitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBase_InitStructure);
 
	TIM_OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_InitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OC_InitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	TIM_OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC_InitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_InitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OC_InitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM3, &TIM_OC_InitStructure);

	TIM_CtrlPWMOutputs(TIM3, ENABLE);

	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	TIM_CCxCmd(TIM3, TIM_Channel_4, TIM_CCx_Enable);
	TIM_Cmd(TIM3, ENABLE);

	// DMA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	TIM_DMACmd(TIM3, TIM_DMA_CC4, ENABLE);
	DMA_ITConfig(DMA1_Stream2, DMA_IT_HT, ENABLE);
	DMA_ITConfig(DMA1_Stream2, DMA_IT_TC, ENABLE);

	start_dma();

	// NVIC for DMA
	nvic_init.NVIC_IRQChannel = DMA1_Stream2_IRQn;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 4;
	nvic_init.NVIC_IRQChannelSubPriority = 0;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_init);	
}

// writes the pwm values of one byte into the array which will be used by the dma
static inline void color2pwm(uint16_t ** const dest, const uint8_t color) {
	uint8_t mask = 0x80;

	do {
		if (color & mask) {
			**dest = 49;
		} else {
			**dest = 20;
		}
		*dest += 1;
		mask >>= 1;
	} while (mask != 0);
}

void Update_Buffer(uint16_t* buffer)
{
	struct led *framebufferp;
	uint32_t i, j;
	uint16_t * bufp;

	for (i = 0; i < (PWM_BUFFER_SIZE/2) / 24; i++) {
		if (incomplete_return)
		{
			incomplete_return = 0;
			for(j = 0; j < 24; j++)
			{
				buffer[i * 24 + j] = 0;
			}
			
		}
		else
		{
			if (frame_pos  == FRAMEBUFFER_SIZE)
			{
				incomplete_return = 1;
				frame_pos = 0;
				
				for(j = 0; j < 24; j++)
				{
					buffer[i * 24 + j] = 0;
				}
			}
			else
			{
				framebufferp = &ws2812_framebuffer[frame_pos++];
				bufp =  buffer + (i * 24);

				// edit here to change order of colors in "ws2812_framebuffer" (0x00RRGGBB, 0x00GGBBRR, etc)
				// the chip needs G R B
				color2pwm( &bufp, framebufferp->green); // green
				color2pwm( &bufp, framebufferp->red); // red
				color2pwm( &bufp, framebufferp->blue); // blue
			}
		}
	}
}

void DMA1_Stream2_IRQHandler(void)
{
	// Half-Transfer completed
	if (DMA_GetITStatus(DMA1_Stream2, DMA_IT_HTIF2))
	{
		DMA_ClearITPendingBit(DMA1_Stream2, DMA_IT_HTIF2);
		Update_Buffer(PWM_Buffer);
	}

	// Transfer completed
	if (DMA_GetITStatus(DMA1_Stream2, DMA_IT_TCIF2))
	{
		DMA_ClearITPendingBit(DMA1_Stream2, DMA_IT_TCIF2);
		Update_Buffer(PWM_Buffer + (PWM_BUFFER_SIZE / 2));
	}

}
