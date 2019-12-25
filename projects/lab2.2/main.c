#include "main.h"

/* 1. Flash LEDs on extension board from 1 to 3 in an endless loop with 1 sec pause */
/* 2. Each time light only one LED */
/* 3. On user button press change LED flashing direction */

static int8_t leds_counter;
static int8_t led_direction;
   
static int8_t i;

void EXTI0_IRQHandler(void)
{
   if (EXTI_GetITStatus(EXTI_Line0) != RESET)
   {
      EXTI_ClearITPendingBit(EXTI_Line0);
      
      GPIO_ResetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
      for (int k = 0; k < 2000000; ++k);
      GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
      led_direction = -led_direction;
   }
}

void TIM2_IRQHandler(void)
{
   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
   {
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	 switch (i)
		{
		case 0: 
			{
			GPIO_ResetBits(GPIOA, GPIO_Pin_8);
			GPIO_SetBits(GPIOA, GPIO_Pin_9 | GPIO_Pin_10);
			break;
			}
		case 1: 
			{
			GPIO_ResetBits(GPIOA, GPIO_Pin_9);
			GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_10);
			break;
			}
		case 2: 
			{
			GPIO_ResetBits(GPIOA, GPIO_Pin_10);
			GPIO_SetBits(GPIOA, GPIO_Pin_9 | GPIO_Pin_8);
			break;
			}
		}
	// direction - RGB
	if (led_direction == 1)
	{
	i++;
	if (i == 3)
	    i = 0;
	}
	// direction - RBG
	else if (led_direction == -1)
	{
	i--;
	if (i == -1)
	    i = 2;
	}
   }
}

int main(void)
{
   leds_counter = 0;
   led_direction = 1;
   i = 0;
   configure_leds();
   configure_buttons();
   configure_timers();

   for (;;)
   {
   }
}