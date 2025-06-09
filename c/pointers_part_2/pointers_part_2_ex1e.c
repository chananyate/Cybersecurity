char *StrChr(const char *str, int c)
{
    while (*str != '\0')
    {
        if (*str == (char)c)
            return str; 

        ++str;
    }

    if (c == '\0')
        return str;

    return NULL;
}
