#include <stm32f4xx.h>
#include <led.h>

uint16_t brightness_max;

uint32_t pwm_direction0;
uint32_t pwm_direction1;
uint32_t pwm_direction2;

uint32_t color;

Led led = {0, 0, 0};

void EXTI0_IRQHandler(void)
{
   if (EXTI_GetITStatus(EXTI_Line0) != RESET)
   {
      EXTI_ClearITPendingBit(EXTI_Line0);
	switch (color){
	case 0:
		{
		led.red += pwm_direction0;
		if (led.red >= brightness_max)
			pwm_direction0 = -pwm_direction0;
		else if (led.red <= 0)
			pwm_direction0 = -pwm_direction0;
		break;
		}
	case 1:
		{
		led.green += pwm_direction1;
		if (led.green >= brightness_max)
			pwm_direction1 = -pwm_direction1;
		else if (led.green <= 0)
			pwm_direction1 = -pwm_direction1;
		break;
		}
	case 2:
		{

		led.blue += pwm_direction2;
		if (led.blue >= brightness_max)
			pwm_direction2 = -pwm_direction2;
		else if (led.blue <= 0)
			pwm_direction2 = -pwm_direction2;
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
        set_led_color(led);
    }
}

int main(void)
{
	brightness_max = 25;

	pwm_direction0 = 1;
	pwm_direction1 = 1;
	pwm_direction2 = 1;

	color = 0;
        led.red=0;
        led.green=0;
        led.blue=0;
	configure_timers();
	configure_button0();
	configure_button1();
	configure_interrupts();
	configure_pwm();

	configure_leds();
	start_timers();
    

    for (;;);
}