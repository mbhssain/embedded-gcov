/* Minimal syscall stubs for embedded targets using newlib-nano */
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#undef errno
extern int errno;

/* Environment variables (not used in embedded) */
char *__env[1] = { 0 };
char **environ = __env;

/* Stub implementations for newlib syscalls */

void _exit(int status)
{
    /* Hang here - in real embedded system, might reset or enter low power */
    (void)status;
    while(1);
}

int _close(int file)
{
    (void)file;
    return -1;
}

int _fstat(int file, struct stat *st)
{
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

int _write(int file, char *ptr, int len)
{
    (void)file;
    /* In real embedded system, this would write to UART */
    /* For now, just pretend we wrote everything */
    (void)ptr;
    return len;
}

caddr_t _sbrk(int incr)
{
    /* Simple heap management stub */
    extern char end asm("end");  /* Defined by linker */
    static char *heap_end = 0;
    char *prev_heap_end;

    if (heap_end == 0)
        heap_end = &end;

    prev_heap_end = heap_end;
    heap_end += incr;
    
    return (caddr_t)prev_heap_end;
}

int _getpid(void)
{
    return 1;
}

int _kill(int pid, int sig)
{
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

