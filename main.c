#include "stm32f4xx_conf.h"
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_dma.h>

//#define HSE_VALUE    ((uint32_t)8000000) 


u16 PWM_Buffer[] = {
		49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,
		49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
		49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,
		20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
		49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,
		49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,
		49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,
		49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,
		49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
		20,20,20,20,20,20,20,20,49,49,49,49,49,49,49,49,20,20,20,20,20,20,20,20,
		
		00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
		00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
		00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
		00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
		00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
		00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
		00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
		00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
		00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
		00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,
	};

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

static void start_dma(void)
{
	static DMA_InitTypeDef dma_init =
	{
			.DMA_BufferSize 		= 520,
			.DMA_Channel 			= DMA_Channel_5,
			.DMA_DIR 			= DMA_DIR_MemoryToPeripheral,
			.DMA_FIFOMode 			= DMA_FIFOMode_Disable,
			.DMA_FIFOThreshold 		= DMA_FIFOThreshold_HalfFull,
			.DMA_Memory0BaseAddr 	= (uint32_t) PWM_Buffer,
			.DMA_MemoryBurst 		= DMA_MemoryBurst_Single,
			.DMA_MemoryDataSize 	= DMA_MemoryDataSize_HalfWord,
			.DMA_MemoryInc 			= DMA_MemoryInc_Enable,
			.DMA_Mode 				= DMA_Mode_Circular,
			.DMA_PeripheralBaseAddr = (uint32_t) &TIM3->CCR4,
			.DMA_PeripheralBurst 	= DMA_PeripheralBurst_Single,
			.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord,
			.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable,
			.DMA_Priority 			= DMA_Priority_Medium
	};

	DMA_Init(DMA1_Stream2, &dma_init);
	DMA_Cmd(DMA1_Stream2, ENABLE);
	TIM_DMACmd(TIM3, TIM_DMA_CC4, ENABLE);
}


int main(void)
{         	
	SystemInit();

	//InitStructures...
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	TIM_OCInitTypeDef TIM_OC_InitStructure;

	//Clock für GPIO setzen
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	//Clock für TIM4 setzen
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	
	//GPIO_PIN konfigurieren

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
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
	
	start_dma();
	
	while (1) {};
	
}
