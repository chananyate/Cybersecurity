

char *StrStr(const char *string, const char *substring)
{
    const char *runner = NULL;

    const char *tmp = NULL;

    runner = string;

    tmp = substring;

    while (*string != '\0')
    {
        substring = tmp;
        runner = string;

        while (*runner != '\0' && *runner == *substring)
        {
            ++runner;
            ++substring;
        }

    if (*substring == '\0')
        return (char *)string;

     ++string;

    }

    return NULL;
}
