/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Virtual Com Port Demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

//#define USE_STM32100B_EVAL 1

#define assert_param(expr) ((void)0)

/* Includes ------------------------------------------------------------------*/
#ifdef STM32L1XX_MD
 #include "stm32l1xx.h"
#else
 #include "stm32f10x.h"
#endif /* STM32L1XX_MD */

#include "stm32f10x_gpio.h"

#include "usb_lib.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "colors.h"
#include "config.h"
#include "strip.h"
#include "strip_ll.h"
#include "sys_tick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
volatile int raibowOffset = 0;
void Rainbow(int speed)
{
	hsv hcv_color;
	hcv_color.h = 0.0;
	hcv_color.s = 1.0;
	hcv_color.v = 0.1;

	for(int i = 0; i < CFG_STRIP_LEDS; i++)
	{
		int v = (i*5) + raibowOffset;
		while(v > 360)
			v -= 360;
		hcv_color.h = v;
		StripSetLed(0, i, hsv2rgb(hcv_color));
	}
	raibowOffset += speed;
	//if(raibowOffset > 360 * 3)
	if(raibowOffset > 360)
		raibowOffset -= 360;
		// raibowOffset -= (360*3);
}

#define STAR_TAIL	4
int shootingStarStep = 0;
void ShootingStar()
{
	hsv hcv_color;
	hcv_color.h = 0.0;
	hcv_color.s = 1.0;
	hcv_color.v = 0.0;

	static uint8_t skip = 10;
	if(0==skip--)
	{
		skip = 10;

		StripClear(hsv2rgb(hcv_color));

		hcv_color.h = 0.0;
		hcv_color.s = 0.0;
		hcv_color.v = 0.5;
		for(int i = shootingStarStep; i > shootingStarStep - STAR_TAIL; i--)
		{
			hcv_color.v -= 0.05;
			StripSetLed(0, i, hsv2rgb(hcv_color));
		}
		shootingStarStep++;
		if(shootingStarStep > 100)
			shootingStarStep = 0;
	}
}

int fireworkStep = 0;
#define FW_SETOFF	(48/1)
#define FW_BLOW 	(60/1)
#define FW_LENGTH	(40/1)
int wave[FW_LENGTH] =
{20, 40, 60, 80, 100, 100, 100, 100, 100, 90,
 80, 70, 60, 50, 40,  30,  20,  10,  10,  10,
 10, 10, 10, 10, 10,  10,  10,  10,  10,  10,
 10, 9,   8, 7,   6,  5,   4,   3,   2,   1
};

void Firework()
{
	hsv hcv_color;
	hcv_color.h = 0.0;
	hcv_color.s = 1.0;
	hcv_color.v = 0.0;

	static uint8_t skip = 5;
	if(0==skip--)
	{
		skip = 5;

		StripClear(hsv2rgb(hcv_color));
		if(fireworkStep < (FW_SETOFF/4))
		{
			//shoot
			// HAL_Delay(25);
			hcv_color.h = 0.0f;
			hcv_color.s = 0.0f;
			hcv_color.v = 0.5f;
			for(int i = fireworkStep; i > fireworkStep - STAR_TAIL; i--)
			{
				hcv_color.v -= 0.8f / STAR_TAIL;
				StripSetLed(0, i, hsv2rgb(hcv_color));
				StripSetLed(0, CFG_STRIP_LEDS-i, hsv2rgb(hcv_color));
			}
		}
		// else if(fireworkStep < FW_BLOW/2)
		// {
		// 	// HAL_Delay(50);
		// 	hcv_color.h = 0.0f;
		// 	hcv_color.s = 0.1f;
		// 	hcv_color.v = 1.0f;
		// 	float v = 1.2f - (fireworkStep - FW_SETOFF) / 20.0f;

		// 	//for(int i = CFG_STRIP_LEDS-1; i >= CFG_STRIP_LEDS-STAR_TAIL*2; i--)
		// 	for(int i = CFG_STRIP_LEDS/2; i >= 0; i--)
		// 	{
		// 		v -= 0.05f;
		// 		hcv_color.v = v;
		// 		if(v > 1.0f)
		// 			v = 1.0f;
		// 		StripSetLed(0, i, hsv2rgb(hcv_color));
		// 		StripSetLed(0, CFG_STRIP_LEDS-i, hsv2rgb(hcv_color));
		// 	}
		// }
		else
		{
			hcv_color.h = 0.0;
			hcv_color.s = 1.0;
			hcv_color.v = 0.2;
			// HAL_Delay(50);
			
			//for(int i = CFG_STRIP_LEDS-1; i >= 0; i--)
			for(int i = CFG_STRIP_LEDS/2; i >= 0; i--)
			{
				int idx = (CFG_STRIP_LEDS/2)-i;//from top
				hcv_color.h = idx * 360 / (CFG_STRIP_LEDS/3);	//hcv_color.h = idx * 360 / CFG_STRIP_LEDS;	
				int waveIdx = (fireworkStep - FW_SETOFF/4) - idx;
				if(waveIdx >= FW_LENGTH)
					hcv_color.v = 0;
				else if(waveIdx < 0)
						hcv_color.v = 0;
				else 
					hcv_color.v = (float)(wave[waveIdx]) / 500;
				StripSetLed(0, i, hsv2rgb(hcv_color));
				StripSetLed(0, CFG_STRIP_LEDS-i, hsv2rgb(hcv_color));
			}
		}
		fireworkStep++;
		if(fireworkStep > 70)//if(fireworkStep > 150)
			fireworkStep = 0;
	}
}

void Flag()
{
	rgb color;
	color = rgb_create(0.3, 0, 0);

	int i = 0;
	for(i = 0; i < CFG_STRIP_LEDS / 3; i++)
	{
		StripSetLed(0, i, color);
	}

	color = rgb_create(0, 0, 0.3);
	for(; i < 3 * CFG_STRIP_LEDS / 4; i++)
	{
		StripSetLed(0, i, color);
	}

	color = rgb_create(0.3, 0.3, 0.3);
	for(; i < CFG_STRIP_LEDS; i++)
	{
		StripSetLed(0, i, color);
	}
}

int runnerStep = 0;
int runnerDir = 1;
void Runner()
{
	hsv h;
	static uint8_t skip = 25;
	if(0==skip--)
	{
		skip = 25;
		//HAL_Delay(25);
		for(int i = 0; i < CFG_STRIP_LEDS; i++)
		{
			rgb old = StripGetLed(0, i);
			h = rgb2hsv(old);
			h.v *= 0.8;
			StripSetLed(0, i, hsv2rgb(h));
		}
		h.s = 0;
		//h.s = runnerStep/CFG_STRIP_LEDS;
		h.h = 0;
		//h.h = runnerStep/CFG_STRIP_LEDS;
		h.v = 1.0;
		StripSetLed(0, runnerStep, hsv2rgb(h));
		runnerStep = runnerStep + runnerDir;
		if(runnerStep == CFG_STRIP_LEDS - 1)
		{
			runnerDir = -1;
		}
		if(runnerStep == 0)
		{
			runnerDir = 1;
		}
	}
}
/* USER CODE END 0 */

volatile  unsigned int task_load_leds1 =0;
volatile  unsigned int task_load_leds2 =0;
volatile  unsigned int task_compute 	=0;
volatile  unsigned int task_cyclogram =0;
volatile  unsigned int task_white_color = 0;

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
  uint32_t i;
  
  SystemInit();
  
  Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();

  systick_up();

  __enable_irq();
  __DSB();	

	// GPIOB->ODR |= GPIO_Pin_12;
	// delay(1);
	// GPIOB->ODR &= ~GPIO_Pin_12;

	StripInit();

  while (1)
  {
		// uint32_t dtick = tick_update();

		if(tick_update())
		{
			task_cyclogram++;

			if( task_cyclogram == 1 ) task_load_leds1 = 1;
			if( task_cyclogram == 5 ) task_load_leds2 = 1;
			if( task_cyclogram == 10) task_compute = 1;
			if( task_cyclogram >= 20) task_cyclogram = 0;

			if(task_white_color)
			{
				task_white_color--;
				task_compute = 0;
			}
		}


    if( task_load_leds1 )
    {
      task_load_leds1 = 0;
      /// start dma
			// GPIOB->ODR ^= GPIO_Pin_12;
			StripLLInit(0,0);//?
    }

		if( task_load_leds2 )
    {
      task_load_leds2 = 0;
      /// start dma
			// GPIOB->ODR ^= GPIO_Pin_12;
			StripLLInit(0,1);//?
    }

    if( task_compute )
    {
      task_compute =0;

			GPIOB->ODR |= GPIO_Pin_12;
			// Firework();

			switch((get_tick()>>16)&0x01)			
			{
				case 0:
					Rainbow(1);
				break;

				default:
					Firework();	
			}
      
			StripUpdate();
			// delay(1);
			GPIOB->ODR &= ~GPIO_Pin_12;

    }

    if(USART_Tx_ptr_out>0)
    { 
			// for(i=0;i<USART_Tx_ptr_out;i++)
			// { 
				// USART_Rx_Buffer[USART_Rx_ptr_in++]=00;
				// USART_Rx_Buffer[USART_Rx_ptr_in++] = USART_Tx_Buffer[i];
			// }

			rgb c = {0};

			switch (USART_Tx_Buffer[0])
			{
				case '1': c.r = c.g = c.b = 0.1f; break;
				case '2': c.r = c.g = c.b = 0.20f; break;
				case '3': c.r = c.g = c.b = 0.25f; break;
				case '4': c.r = c.g = c.b = 0.30f; break;
				case '5': c.r = c.g = c.b = 0.32f; break;
				case '6': c.r = c.g = c.b = 0.34f; break;
				case '7': c.r = c.g = c.b = 0.36f; break;
				case '8': c.r = c.g = c.b = 0.38f; break;
				case '9': c.r = c.g = c.b = 0.40f; break;
				default:
					c.r = c.g = c.b = 0.05f;
			}
			
			StripClear(c);

			StripUpdate();

			task_white_color = 3000; // 3 sec

			USART_Tx_ptr_out = 0;
			Handle_USBAsynchXfer();
    } 
  
  }
}

unsigned char SendData()
{
  return 1;
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
