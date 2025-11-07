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

static constexpr u32 SCL = 0;
static constexpr u32 SDA = 1;

static inline void init_clock(void)
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
    SysTick_Config(SystemCoreClock / 100'000);
}

typedef enum : u8 {
    PinMode_Output,
    PinMode_Input,
} PinMode;

static inline void pin_mode(const u32 pin, const PinMode mode)
{
    GPIOA->CRL &= ~(0xF << (4 * pin));

    if (mode == PinMode_Output)
        GPIOA->CRL |= 0x6 << (4 * pin);
    else
        GPIOA->CRL |= 0x4 << (4 * pin);
}

static inline u8 pin_read(const u32 pin)
{
    pin_mode(pin, PinMode_Input);
    return (GPIOA->IDR >> pin) & 1;
}

void pin_release(const u32 pin)
{
    pin_mode(pin, PinMode_Input);
}

void pin_drive_low(const u32 pin)
{
    pin_mode(pin, PinMode_Output);
    GPIOA->ODR &= ~(1 << pin);
}

void i2c_write(const u8 byte)
{
    for (size_t i = 0; i < 8; ++i) {
        const u8 bit = (byte >> (7 - i)) & 1;

        if (bit)
            pin_release(SDA);
        else
            pin_drive_low(SDA);

        delay_ms(2);

        pin_release(SCL);
        delay_ms(2);
        pin_drive_low(SCL);
        delay_ms(2);
    }

    // Read ACK
    pin_release(SDA);
    pin_release(SCL);
    delay_ms(2);

    const u8 ack = pin_read(SDA);
    if (ack != 0)
        printf("did not get an ack\n");

    pin_drive_low(SCL);
    delay_ms(2);
}

static constexpr u8 RW_WRITE = 0;
static constexpr u8 RW_READ = 1;

void i2c_write_addr(const u8 byte, const u8 rw)
{
    i2c_write((byte << 1) | rw);
}

u8 i2c_read(const bool ack)
{
    u8 result = 0;

    for (size_t i = 0; i < 8; ++i) {
        pin_release(SCL);
        delay_ms(2);

        const u8 bit = pin_read(SDA);
        result |= bit << (7 - i);

        pin_drive_low(SCL);
        delay_ms(2);
    }

    // Write (N)ACK
    if (ack) {
        pin_drive_low(SDA);
        delay_ms(2);
    }

    pin_release(SCL);
    delay_ms(2);
    pin_drive_low(SCL);
    delay_ms(2);

    if (ack) {
        pin_release(SDA);
        delay_ms(2);
    }

    return result;
}

static void i2c_start(void)
{
    pin_release(SDA);
    pin_release(SCL);

    pin_drive_low(SDA);
    delay_ms(2);
    pin_drive_low(SCL);
    delay_ms(2);
}

static void i2c_stop(void)
{
    pin_drive_low(SDA);
    pin_release(SCL);
    delay_ms(2);
    pin_release(SDA);
    delay_ms(2);
}

int main(void)
{
    init_clock();

    // Enable port A GPIO clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    while (true) {
        i2c_start();
        i2c_write_addr(0x52, RW_WRITE);
        i2c_write(0x00);
        i2c_stop();

        i2c_start();
        i2c_write_addr(0x52, RW_READ);

        for (size_t i = 0; i < 6; ++i) {
            const u8 byte = i2c_read(i != 5);

            if (i < 4)
                continue;

            for (size_t i = 0; i < 8; ++i)
                printf("%i", (byte >> (7 - i)) & 1);

            printf(" ");
        }

        i2c_stop();
        printf("\n");
    }

    return 0;
}
