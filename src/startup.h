#ifndef STARTUP_H
#define STARTUP_H

#include "stdinc.h"

extern const u32 _etext, _sdata, _edata, _sidata, _sbss, _ebss, _estack;

constexpr u32 ISR_VECTOR_SIZE_WORDS = 114;

extern void reset_handler(void);

extern void hard_fault_handler(void);

extern void bus_fault_handler(void);

extern void usage_fault_handler(void);

extern void svcall_handler(void);

extern void debug_monitor_handler(void);

extern void pendsv_handler(void);

extern void systick_handler(void);

#endif
