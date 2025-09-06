#include "main.h"
#include "stdinc.h"
#include "stm32f4xx.h"

constexpr u32 LED_PIN = 5;

void main(void)
{
    RCC->AHB1ENR |= 1 << RCC_AHB1ENR_GPIOAEN_Pos;

    // do two dummy reads after enabling the peripheral clock, as per the errata
    volatile uint32_t dummy;
    dummy = RCC->AHB1ENR;
    dummy = RCC->AHB1ENR;

    GPIOA->MODER |= 1 << GPIO_MODER_MODER5_Pos;

    while (true) {
        GPIOA->ODR ^= (1 << LED_PIN);
        for (uint32_t i = 0; i < 1'000'000; ++i)
            ;
    }
}
