#include "syscalls.h"
#include <errno.h>
#include <stddef.h>
#include <stm32f1xx.h>
#include <sys/stat.h>

#undef errno
extern int errno;

void _exit(void)
{
    while (true)
        ;
}

int _close(const int file)
{
    return -1;
}

int fork(void)
{
    errno = EAGAIN;
    return -1;
}

int _fstat(const int file, struct stat *const st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(const int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int open(const char *name, int flags, int mode)
{
    return -1;
}

int _read(const int file, char *const ptr, const int len)
{
    struct {
        int fd;
        const char *buf;
        int len;
    } args = {
        .fd = file,
        .buf = ptr,
        .len = len,
    };

    __asm__ volatile("mov r0, %[op]\n"
                     "mov r1, %[args]\n"
                     "bkpt 0xAB\n"
                     :
                     : [op] "r"(0x06), [args] "r"(&args)
                     : "r0", "r1", "memory");

    return len;
}

void *_sbrk(const int incr)
{
    extern char __bss_end__;

    static char *heap_end = &__bss_end__;

    char *stack_ptr_char = (char *)__get_MSP();

    if (heap_end + incr > stack_ptr_char) {
        // Heap and stack collision
        while (true)
            ;
    }

    char *const prev_heap_end = heap_end;
    heap_end += incr;
    return prev_heap_end;
}

int _write(const int file, const char *const ptr, const int len)
{
    struct {
        int fd;
        const char *buf;
        int len;
    } args = {
        .fd = file,
        .buf = ptr,
        .len = len,
    };

    __asm__ volatile("mov r0, %[op]\n"
                     "mov r1, %[args]\n"
                     "bkpt 0xAB\n"
                     :
                     : [op] "r"(0x05), [args] "r"(&args)
                     : "r0", "r1", "memory");

    return len;
}
