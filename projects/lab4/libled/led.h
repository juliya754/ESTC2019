#ifndef _LED_H_
#define _LED_H_

#include <stm32f4xx.h>
#include <stdint.h>

typedef struct
{
	uint32_t* current;
	uint32_t red;
	uint32_t green;
	uint32_t blue;
} Led;

#define PWM_WIDTH (1000000)
#define PWM_FREQUENCY_HZ (50)
/*
void TIM2_IRQHandler(void);

void configure_leds(void);
void configure_timers(void);
void configure_interrupts(void);
void configure_pwm(void);
void led_set_color(Led* led);
void start_timers(void);
*/
void TIM2_IRQHandler(void);

void configure_leds(void);
void configure_timers(void);
void configure_interrupts(void);
void configure_pwm(void);
void set_led_color(Led led);
void start_timers(void);
#endif