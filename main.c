#include <stdio.h>

#include "ini.h"

int main(int argc, char **argv)
{
    char    *value;
    int     result;

    if (argc < 5)
    {
        printf("%s FILE SECTION KEY DEFAULT\n", argv[0]);
        return 1;
    }

    if ((result = INI_Open(argv[1])) > 0)
    {
        if (result == 1)
        {
            printf("File error\n");
        }
        else if (result == 2)
        {
            printf("No valid sections found\n");
        }
        else if (result == 3)
        {
            printf("Internal memory allocation error\n");
        }

        return result;
    }

    value = INI_GetString(argv[2], argv[3], argv[4]);
    printf("%s:\n[%s]\n%s = %s\n", argv[1], argv[2], argv[3], value);

    INI_Close();

    return 0;
}
