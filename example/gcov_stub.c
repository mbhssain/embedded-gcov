/* Stub for gcov functions when building without coverage */

void __gcov_dump(void)
{
    /* No-op when coverage is disabled */
}

void __gcov_reset(void)
{
    /* No-op when coverage is disabled */
}

void __gcov_init(void *info)
{
    /* No-op when coverage is disabled */
    (void)info;
}


