size_t StrSpn(const char *str1, const char *str2);
{
    const char *runner = NULL;

    const char *tmp = NULL;

    runner = str1;

    tmp = str2;

    size_t count = 0;

    while (*str1 != '\0')
    {
        str2 = tmp;
        runner = str1;

        while (*runner != '\0' && *runner == *str2)
        {
            ++count;
            ++runner;
            ++substring;
        }

    if (*str2 == '\0')
        return count;

     ++str1;

    }

    return 1;
}
