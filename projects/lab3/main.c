#include "main.h"

#include <math.h>

uint32_t time;
int brightness = 10;
int color = 0;

void EXTI0_IRQHandler(void)
{
   if (EXTI_GetITStatus(EXTI_Line0) != RESET)
   {
      EXTI_ClearITPendingBit(EXTI_Line0);
      if (brightness == 10)
		brightness = 50;
      else brightness = 10;
   }
}

void EXTI1_IRQHandler(void)
{
   if (EXTI_GetITStatus(EXTI_Line1) != RESET)
   {
	EXTI_ClearITPendingBit(EXTI_Line1);
	if (color == 0 || color == 1)
		{color += 1;}
	else {color = 0;}
   }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	if (color == 0) {
			TIM_SetCompare1(TIM1, brightness);
			TIM_SetCompare2(TIM1, 0);
			TIM_SetCompare3(TIM1, 0);
			}
	if (color == 1) {
			TIM_SetCompare2(TIM1, brightness);
			TIM_SetCompare1(TIM1, 0);
			TIM_SetCompare3(TIM1, 0);
			}
	if (color == 2) {
			TIM_SetCompare3(TIM1, brightness);
			TIM_SetCompare1(TIM1, 0);
			TIM_SetCompare2(TIM1, 0);
			}

    }
}

int main(void)
{
    time = 0;
    configure_timers();
    configure_button0();
    configure_button1();
    configure_interrupts();
    configure_pwm();

    configure_leds();
    start_timers();
    

    for (;;);
}