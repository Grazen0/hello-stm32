#include "main.h"
#include "startup.h"
#include "stdinc.h"
#include <stddef.h>
#include <stm32f1xx.h>

register u8 *const stack_ptr __asm__("sp");

constexpr u32 LED_PIN = 13;

static volatile u32 ticks = 0;

void systick_handler(void)
{
    ++ticks;
}

void delay_ms(const u32 ms)
{
    const u32 start = ticks;
    const u32 end = start + ms;

    if (end < start) {
        while (ticks > start)
            ;
    }

    while (ticks < end)
        ;
}

void setup(void)
{
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0)
        ;

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSE;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE)
        ;

    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(0xF << ((LED_PIN - 8) * 4));
    GPIOC->CRH |= (0x2 << ((LED_PIN - 8) * 4));

    SysTick_Config(SystemCoreClock / 1000);
}

void loop(void)
{
    GPIOC->ODR ^= (1 << LED_PIN);
    delay_ms(500);
}
