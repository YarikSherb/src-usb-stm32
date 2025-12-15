#include "strip_ll.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

TIM_TimeBaseInitTypeDef htim2;
TIM_TimeBaseInitTypeDef htim6;

static void Dma_Init(unsigned char chan);
static void TIM2_Init(unsigned char chan);
//static void TIM6_Init(void);

static void UpdateByte(volatile uint16_t *ptr, uint8_t value);

static volatile int enabled;
/**
 * Длительности импульсов для каждого разряда
 * Дополнительный разряд в конце - для обнуления линии (импульс нулевой длительности оставит линию в состоянии сброса)
 * Инициализация тут только для того, чтобы массив ушел в сегмент data, который доступен для DMA (секция RAM_D1, см. скрипт линкера)
 */
volatile uint16_t bits1[TOTAL_BITS+1] = {0,1};

void reset_bits(void)
{
	for(int i = 0; i < TOTAL_BITS; i++)
	{
		bits1[i] = T_ZERO;//T_ZERO;
	}
	bits1[TOTAL_BITS] = 0;//Добавить импульс нулевой длительности для сброса линии
}

void StripLLInit(unsigned char reset_b, unsigned char chan)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	//Инициализация массива
	//if(reset_b!=0)	reset_bits();

	enabled = 0;

	TIM_DMACmd(TIM2,TIM_DMA_Update, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	TIM2->CNT = 0;

	//GPIOB->ODR |= ((uint16_t)0x1000);
	delay(1);
	//GPIOB->ODR &= ~((uint16_t)0x1000);
	
	TIM2_Init(chan);
	Dma_Init(chan);
	
	TIM_DMACmd(TIM2,TIM_DMA_Update, ENABLE);
}

void StripLLSet(int channel, rgb *data, int cnt)
{
	for(int i = 0; i < cnt; i++)
	{
		uint8_t v;
		v = data[i].r * 255;
		UpdateByte(bits1 + i*24 + 0, v);
		v = data[i].g * 255;
		UpdateByte(bits1 + i*24 + 8, v);
		v = data[i].b * 255;
		v = (v & 0xFC) + 0;
		UpdateByte(bits1 + i*24 + 16, v);
	}
	bits1[TOTAL_BITS] = 0;
}

void UpdateByte(volatile uint16_t *ptr, uint8_t value)
{
	for(int i = 0; i < 8; i++)
	{
		if(value & (1 << (7-i)))
		{
			ptr[i] = T_ONE;
		}
		else
		{
			ptr[i] = T_ZERO;
		}
	}
}
/*
void StripLLEnable(int enable)
{
	enabled = enable;
	if(enable)
	{
		bits1[TOTAL_BITS] = 0;
		//HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_3, (uint32_t*)bits1, TOTAL_BITS+1);
		TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);
	}
}*/

static void TIM2_Init(unsigned char chan)
{
  TIM_OCInitTypeDef sConfigOC = {0};

	TIM_Cmd(TIM2, DISABLE);

  htim2.TIM_Prescaler = T_PRESCALER;
  htim2.TIM_CounterMode = TIM_CounterMode_Up;
  htim2.TIM_Period = T_PULSE;
  htim2.TIM_ClockDivision = TIM_CKD_DIV1;
  htim2.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &htim2);
  
  sConfigOC.TIM_OCMode = TIM_OCMode_PWM1;
  sConfigOC.TIM_Pulse = 0;
  sConfigOC.TIM_OCPolarity = TIM_OCPolarity_High;
  sConfigOC.TIM_OCNPolarity = TIM_OCNPolarity_High;
  sConfigOC.TIM_OCIdleState = TIM_OCIdleState_Reset;
  sConfigOC.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	sConfigOC.TIM_OutputState = TIM_OutputState_Enable;
	sConfigOC.TIM_OutputNState = TIM_OutputNState_Disable;

	if(chan==0)
  	TIM_OC3Init(TIM2,&sConfigOC);
	else
  	TIM_OC4Init(TIM2,&sConfigOC);

	TIM_Cmd(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM2,ENABLE);

	if(chan==0)
  	TIM_DMAConfig(TIM2, TIM_DMABase_CCR3, TIM_DMABurstLength_1Transfer);
	else
		TIM_DMAConfig(TIM2, TIM_DMABase_CCR4, TIM_DMABurstLength_1Transfer);//TIM_DMABase_CCR3
	//TIM_DMACmd(TIM2,TIM_DMA_Update, ENABLE);
}

void Dma_Init(unsigned char chan)
{
	static DMA_InitTypeDef  hdma_tim;
	DMA_StructInit(&hdma_tim);

	DMA_Cmd(DMA1_Channel2,DISABLE);

	hdma_tim.DMA_BufferSize = (TOTAL_BITS+1);
	hdma_tim.DMA_DIR = DMA_DIR_PeripheralDST;
	hdma_tim.DMA_MemoryInc = DMA_MemoryInc_Enable;
	hdma_tim.DMA_MemoryBaseAddr = &bits1[0];
	hdma_tim.DMA_Priority = DMA_Priority_High;
	hdma_tim.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	hdma_tim.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

	if(chan==0)
		hdma_tim.DMA_PeripheralBaseAddr = (uint32_t*) &(TIM2->CCR3);
	else	
		hdma_tim.DMA_PeripheralBaseAddr = (uint32_t*) &(TIM2->CCR4);

	DMA_Init(DMA1_Channel2, &hdma_tim);

	// hdma_tim.DMA_PeripheralBaseAddr = &(TIM2->CCR4);
	// DMA_Init(DMA1_Channel2, &hdma_tim);

	// DMA_ClearFlag(DMA1_FLAG_GL1);
	// DMA_ClearFlag(DMA1_FLAG_GL2);

	/*##-2- Configure the NVIC for DMA #########################################*/
	/* NVIC configuration for DMA transfer complete interrupt */
	// NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	// NVIC_EnableIRQ(DMA1_Stream0_IRQn);

	// DMA_Cmd(DMA1_Channel1,ENABLE);
	DMA_Cmd(DMA1_Channel2,ENABLE);
}
