#include <stm32f4xx.h>
#include <time.h>
/*
 * @brief Simple lab -- blink leds in R->B->G order, on button press
 *        LEDS blink white once for a short period of time and then
 *        blink in a reverse order G->B->R.
 */

#define SWITCH_DELAY    ((uint32_t)2000000)

typedef enum blink_mul_e
{
  BLINK_MUL_FAST = 1,
  BLINK_MUL_LONG = 3,
} blink_mul_t;

void blink_led(GPIO_TypeDef * port, uint16_t pins, blink_mul_t multiplier);
static void setupClock(int);
static void setupClockHSI(void);

int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable peripheral clock for LEDs and buttons port */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  /* Init LEDs */
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Turn all the leds off */
  GPIO_SetBits(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);

  int8_t i = 0;
  int8_t direction = 1;
  int8_t flag = 0;

  while (1)
  {
    int state = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0);
    if (!state)
    {
      setupClock(4);
      blink_led(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10, BLINK_MUL_FAST);
    }

    int state1 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1);
    if (!state1)
    {
      setupClockHSI();
      blink_led(GPIOA, GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10, BLINK_MUL_FAST);
    }

    blink_led(GPIOA, GPIO_Pin_8 << i, BLINK_MUL_LONG);

    i = (3 + (i - direction) % 3) % 3;
  }
}

void blink_led(GPIO_TypeDef * port, uint16_t pins, blink_mul_t multiplier)
{
  uint32_t k = 0;

  GPIO_ResetBits(port, pins);
  for (k = 0; k < SWITCH_DELAY * multiplier; ++k);
  GPIO_SetBits(port, pins);
}

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

static void setupClock(int PLL_N)
{
  /* Try to enable the HSE oscillator */
  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
      RCC_HSEConfig(RCC_HSE_ON);
      if (SUCCESS != RCC_WaitForHSEStartUp())
      {
        /* IF HSE fails - do nothing */
        goto hse_fail;
      }
  }

  /* Switch to HSE instead of PLL and wait until it switches */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);
  while (RCC_GetSYSCLKSource() != 0x04);

  /* Disable PLL */
  RCC_PLLCmd(DISABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET);
  
  /* Configure PLL */
  RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, PLL_N, 7);
  RCC_PLLCmd(ENABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != SET);
  
  /* Switch to PLL instead of HSE */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  while (RCC_GetSYSCLKSource() != 0x08);

hse_fail:
  return;
}

static void setupClockHSI(void)
{

  /* Switch to HSI and wait until it switches */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
  while (RCC_GetSYSCLKSource() != 0x00);

    /* Disable PLL */
  RCC_PLLCmd(DISABLE);
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET);

}

