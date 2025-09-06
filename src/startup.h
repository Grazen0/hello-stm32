#ifndef STARTUP_H
#define STARTUP_H

#include "stdinc.h"

constexpr u32 SRAM_START = 0x20000000U;
constexpr u32 SRAM_SIZE = 32U * 1024U;
constexpr u32 SRAM_END = SRAM_START + SRAM_SIZE;
constexpr u32 STACK_POINTER_INIT_ADDRESS = SRAM_END;
constexpr u32 ISR_VECTOR_SIZE_WORDS = 114;

extern const u32 _etext, _sdata, _edata, _sbss, _ebss;

void reset_handler(void);

void hard_fault_handler(void);

void bus_fault_handler(void);

void usage_fault_handler(void);

void svcall_handler(void);

void debug_monitor_handler(void);

void pendsv_handler(void);

void systick_handler(void);

#endif
