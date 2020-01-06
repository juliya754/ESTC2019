#include "main.h"

#include <math.h>

uint32_t time;

uint16_t brightness_max;

uint32_t brightness0;  // red
uint32_t brightness1;  // green
uint32_t brightness2;  // blue

uint32_t pwm_direction0;
uint32_t pwm_direction1;
uint32_t pwm_direction2;

uint32_t color;

void EXTI0_IRQHandler(void)
{
   if (EXTI_GetITStatus(EXTI_Line0) != RESET)
   {
      EXTI_ClearITPendingBit(EXTI_Line0);
	switch (color){
	case 0:
		{
		brightness0 += 20 * pwm_direction0;
		if (brightness0 >= 100)
			{
			pwm_direction0 = -pwm_direction0;
			brightness0 = 100;
			}
		else if (brightness0 <= 0)
			{
			pwm_direction0 = -pwm_direction0;
			brightness0 = 0;
			}
		break;
		}
	case 1:
		{
		brightness1 += 20 * pwm_direction1;
		if (brightness1 >= 100)
			{
			pwm_direction1 = -pwm_direction1;
			brightness1 = 100;
			}
		else if (brightness1 <= 0)
			{
			pwm_direction1 = -pwm_direction1;
			brightness1 = 0;
			}
		break;
		}
	case 2:
		{
		brightness2 += 20 * pwm_direction2;
		if (brightness2 >= 100)
			{
			pwm_direction2 = -pwm_direction2;
			brightness2 = 100;
			}
		else if (brightness2 <= 0)
			{
			pwm_direction2 = -pwm_direction2;
			brightness2 = 0;
			}
		break;
		}
	}
   }
}

void EXTI1_IRQHandler(void)
{
   if (EXTI_GetITStatus(EXTI_Line1) != RESET)
   {
	EXTI_ClearITPendingBit(EXTI_Line1);
	color = (color + 1) % 3;
   }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_SetCompare1(TIM1, (100000 / PWM_FREQUENCY_HZ - 1) * brightness0 / 100); // red
	TIM_SetCompare2(TIM1, (100000 / PWM_FREQUENCY_HZ - 1) * brightness1 / 100); // green
	TIM_SetCompare3(TIM1, (100000 / PWM_FREQUENCY_HZ - 1) * brightness2 / 100); // blue
    }
}

int main(void)
{
	brightness_max = (uint16_t)(100000 / PWM_FREQUENCY_HZ - 1);

	brightness0 = 0;  // red
	brightness1 = 0;  // green
	brightness2 = 0;  // blue

	pwm_direction0 = 1;
	pwm_direction1 = 1;
	pwm_direction2 = 1;

	color = 0;

	configure_timers();
	configure_button0();
	configure_button1();
	configure_interrupts();
	configure_pwm();

	configure_leds();
	start_timers();
    

    for (;;);
}