#include "startup.h"
#include "main.h"
#include "stdinc.h"

static void default_handler(void)
{
    while (true)
        ;
}

void nmi_handler(void) __attribute__((weak, alias("default_handler")));
void hard_fault_handler(void) __attribute__((weak, alias("default_handler")));
void bus_fault_handler(void) __attribute__((weak, alias("default_handler")));
void usage_fault_handler(void) __attribute__((weak, alias("default_handler")));
void svcall_handler(void) __attribute__((weak, alias("default_handler")));
void debug_monitor_handler(void)
    __attribute__((weak, alias("default_handler")));
void pendsv_handler(void) __attribute__((weak, alias("default_handler")));
void systick_handler(void) __attribute__((weak, alias("default_handler")));

const u32 isr_vector[ISR_VECTOR_SIZE_WORDS]
    __attribute__((section(".isr_vector"))) = {
        STACK_POINTER_INIT_ADDRESS,
        (u32)reset_handler,
        (u32)nmi_handler,
        (u32)hard_fault_handler,
        (u32)bus_fault_handler,
        (u32)usage_fault_handler,
        0,
        0,
        0,
        0,
        0,
        (u32)svcall_handler,
        (u32)debug_monitor_handler,
        0,
        (u32)pendsv_handler,
        (u32)systick_handler,
};

void reset_handler(void)
{
    const u32 DATA_SIZE = (u32)&_edata - (u32)&_sdata;
    const u32 BSS_SIZE = (u32)&_ebss - (u32)&_sbss;

    // Copy .data from FLASH to SRAM
    const u8 *const flash_data = (u8 *)&_etext;
    u8 *const sram_data = (u8 *)&_sdata;

    for (u32 i = 0; i < DATA_SIZE; ++i)
        sram_data[i] = flash_data[i];

    // Zero-fill .bss section in SRAM
    u8 *const bss = (u8 *)&_sbss;

    for (u32 i = 0; i < BSS_SIZE; ++i)
        bss[i] = 0;

    main();
}
