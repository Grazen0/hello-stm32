#include "main.h"
#include "startup.h"
#include "stdinc.h"
#include "stm32f103xb.h"
#include "system_stm32f1xx.h"
#include <stddef.h>
#include <stdio.h>
#include <stm32f1xx.h>

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

constexpr u32 LED_PIN = 13;

static void init_clock(void)
{
    // Turn on HSE
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0)
        ;

    // Select HSE as system clock
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSE;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE)
        ;

    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);
}

int main(void)
{
    init_clock();

    // Enable port C GPIO clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(0b11 << (4 * (LED_PIN - 8)));
    GPIOC->CRH |= (0x2 << (4 * (LED_PIN - 8)));

    printf("Hello, world!\n");

    while (true) {
        GPIOC->ODR ^= (1 << LED_PIN);
        delay_ms(500);
    }

    return 0;
}
