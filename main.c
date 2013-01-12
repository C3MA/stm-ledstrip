#include "stm32f4xx_conf.h"
#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_dma.h>

//#define HSE_VALUE    ((uint32_t)8000000) 


u16 PWM_Buffer[520];

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}


void DMA_Configuration(void)
{
DMA_DeInit(DMA1_Stream2);
DMA_InitTypeDef DMA_InitStructure ;
DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(TIM3->CCR4);
DMA_InitStructure.DMA_Memory0BaseAddr = (u32)PWM_Buffer;
DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
DMA_InitStructure.DMA_BufferSize = 520;
DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
DMA_InitStructure.DMA_Priority = DMA_Priority_High;
DMA_Init(DMA1_Stream2, &DMA_InitStructure);
// turning DMA on
DMA_Cmd(DMA1_Stream2, ENABLE);

}


int main(void)
{
	DMA_Configuration();
	for (int i=0;i<520;i++) PWM_Buffer[i]=20;
	//fill TEst_PWM
	for(int i=0;i<480;i+=24) {
		for(int j=i;j<i+8;j++) {
			PWM_Buffer[j] = 49;
		}
	}	
	
	
	
	//InitStructures...
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBase_InitStructure;
	TIM_OCInitTypeDef TIM_OC_InitStructure;
		
	SystemInit();

	//Clock für GPIO setzen
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	//Clock für TIM4 setzen
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//Clock für DMA-Controller
	RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
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
	TIM_OC_InitStructure.TIM_Pulse = 49;
	TIM_OC4Init(TIM3, &TIM_OC_InitStructure);
 	TIM_DMACmd(TIM3, TIM_DMA_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	
	while (1) {};
	
	
	/*

	TIM_OC_InitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM3, &TIM_OC_InitStructure);
	
	
	while (1) {
	
	TIM_OC_InitStructure.TIM_Pulse = 49;
	TIM_OC4Init(TIM3, &TIM_OC_InitStructure);
	Delay(12000000L);
	
	TIM_OC_InitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM3, &TIM_OC_InitStructure);
	
	Delay(12000000L);
	
	TIM_OC_InitStructure.TIM_Pulse = 20;
	TIM_OC4Init(TIM3, &TIM_OC_InitStructure);
	Delay(12000000L);
	
	TIM_OC_InitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM3, &TIM_OC_InitStructure);
	
	Delay(12000000L);
	
	
	}
*/

}
