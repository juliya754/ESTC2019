#include <stm32f4xx.h>

#define SWITCH_DELAY    ((uint32_t)2000000)

static
int configure_leds(void)
{
   GPIO_InitTypeDef leds_init_structure;
   /* Enable clocking for LEDS */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   leds_init_structure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
   leds_init_structure.GPIO_Mode = GPIO_Mode_OUT;
   leds_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
   leds_init_structure.GPIO_OType = GPIO_OType_PP;
   GPIO_Init(GPIOA, &leds_init_structure);
   GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);

   return 0;
}

static
int configure_buttons(void)
{
   GPIO_InitTypeDef buttons_init_structure;
   /* Enable clocking for Buttons */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
   buttons_init_structure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
   buttons_init_structure.GPIO_Mode  = GPIO_Mode_IN;
   buttons_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
   buttons_init_structure.GPIO_OType = GPIO_OType_PP;
   buttons_init_structure.GPIO_PuPd  = GPIO_PuPd_UP;
   GPIO_Init(GPIOE, &buttons_init_structure);
   return 0;
}

static
int configure_timers(void)
{
   /* Timer  */
   TIM_TimeBaseInitTypeDef timer_init_structure;
   /* Initialize peripheral clock */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
   /* Initialize timer */
   timer_init_structure.TIM_Prescaler     = 84;  /* Scale value to microseconds */
   timer_init_structure.TIM_CounterMode   = TIM_CounterMode_Up;
   timer_init_structure.TIM_Period        = 1000000;   /* Gives us a second interval */
   timer_init_structure.TIM_ClockDivision = TIM_CKD_DIV1; /* Tell timer to divide clocks */
   timer_init_structure.TIM_RepetitionCounter = 0;
   TIM_TimeBaseInit(TIM2, &timer_init_structure);
   /* Start timer */
   TIM_Cmd(TIM2, ENABLE);

   return 0;
}


void blink_led(GPIO_TypeDef * port, uint16_t pins, uint16_t multiplier)
{
  uint32_t k = 0;

  GPIO_ResetBits(port, pins);
  for (k = 0; k < SWITCH_DELAY * multiplier; ++k);
  GPIO_SetBits(port, pins);
}

int main(void)
{
   uint32_t timer_value = 0;

   int8_t   led_counter   = 0;
   int8_t   led_direction = 1;
   int8_t i = 0;
   uint8_t  button_state          = Bit_RESET;
   uint8_t  previous_button_state = Bit_RESET;

   configure_leds();
   configure_buttons();
   configure_timers();

   for (;;)
   {
      timer_value = TIM_GetCounter(TIM2);
      // each second lights one color in direction of cases
      if (timer_value == 0)
      {
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

      // Handle button press
      button_state = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
      // Trigger event on button release - change direction & highlight by white
      if (previous_button_state == Bit_SET && button_state == Bit_RESET)
      {
         led_direction = -led_direction;
	 blink_led(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10, 1);
      }
      previous_button_state = button_state;

   }
}
