#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

typedef struct stat     STATUS;

typedef struct key_s
{
    char            *name;
    char            *value;
    struct key_s    *next;
}
KEY;

typedef struct sect_s
{
    char            *name;
    KEY             *key;
    struct sect_s   *next;
}
SECTION;

char        *iniBuffer = NULL;
SECTION     *iniSection = NULL;

int IsSpace(const char test)
{
    if (test == ' ' || test == '\t')
    {
        return 0;
    }

    return 1;
}

char *SkipLeadingSpace(char *pos)
{
    while (IsSpace(*pos) == 0)
    {
        pos++;
    }

    return pos;
}

char *RemoveTrailingSpace(char *end)
{
    while (IsSpace(*end) == 0)
    {
        end--;
    }

    *(end + 1) = 0;

    return end;
}

int IsValidName(char *name)
{
    if (strchr(ALPHABET, *name) == NULL)
    {
        return 1;
    }

    for (name++; *name; name++)
    {
        if (strchr(ALPHABET"0123456789_", *name) == NULL)
        {
            return 1;
        }
    }

    return 0;
}

void INI_Close()
{
    SECTION     *section;
    KEY         *key;

    while (iniSection != NULL)
    {
        section = iniSection;
        iniSection = section->next;
        while (section->key != NULL)
        {
            key = section->key;
            section->key = key->next;
            free(key);
        }

        free(section);
    }

    if (iniBuffer != NULL)
    {
        free(iniBuffer);
    }
}

int INI_Open(char *iniFile)
{
    STATUS      status;
    FILE        *file;
    int         length;
    char        *lineStart, *lineEnd, *iniEnd, *posTemp;
    SECTION     *section;
    KEY         *key;

    if (stat(iniFile, &status) == -1)
    {
        return 1;
    }

    if ((file = fopen(iniFile, "rb")) == NULL)
    {
        return 1;
    }

    if ((iniBuffer = malloc(status.st_size + 1)) != NULL)
    {
        length = fread(iniBuffer, 1, status.st_size, file);
    }
    fclose(file);

    if (length < status.st_size)
    {
        return 1;
    }

    if (iniBuffer == NULL)
    {
        return 3;
    }

    iniEnd = iniBuffer + status.st_size;
    *iniEnd = '\n'; // need to guarantee line ending at end of document

    iniSection = NULL;

    for (lineStart = iniBuffer; lineStart < iniEnd; lineStart = lineEnd + 1)
    {
        lineEnd = strchr(lineStart, '\n');
        *lineEnd = 0;

        lineStart = SkipLeadingSpace(lineStart);
        posTemp = RemoveTrailingSpace(lineEnd - 1);

        // sections are minimum 3 characters, i.e. [s]
        // key pairs are minimum 3 characters, i.e. n=v
        if ((length = strlen(lineStart)) < 3)
        {
            continue;
        }

        if (*lineStart == '[' && *posTemp == ']')
        {
            *posTemp = 0; // delete ]
            if (IsValidName(lineStart + 1) == 0)
            {
                if ((section = malloc(sizeof(SECTION))) == NULL)
                {
                    INI_Close();
                    return 3;
                }
                section->name = lineStart + 1;
                section->key = NULL;
                section->next = iniSection;
                iniSection = section;
            }
        }
        else if (iniSection != NULL && (posTemp = strchr(lineStart, '=')) != NULL)
        {
            if (posTemp > lineStart && posTemp < lineEnd - 1)
            {
                RemoveTrailingSpace(posTemp - 1);
                posTemp = SkipLeadingSpace(posTemp + 1);
                if (IsValidName(lineStart) == 0 && strlen(posTemp) > 0)
                {
                    if ((key = malloc(sizeof(KEY))) == NULL)
                    {
                        INI_Close();
                        return 3;
                    }
                    key->name = lineStart;
                    key->value = posTemp;
                    key->next = iniSection->key;
                    iniSection->key = key;
                }
            }
        }
    }

    if (iniSection == NULL)
    {
        INI_Close();
        return 2;
    }

    return 0;
}

char *GetValue(char *section, char *key)
{
    SECTION *section_p = iniSection;
    KEY     *key_p;

    while (section_p != NULL)
    {
        if (strcasecmp(section_p->name, section) == 0 && section_p->key != NULL)
        {
            key_p = section_p->key;
            while (key_p != NULL)
            {
                if (strcasecmp(key_p->name, key) == 0)
                {
                    return key_p->value;
                }
                key_p = key_p->next;
            }

            return NULL;
        }
        section_p = section_p->next;
    }

    return NULL;
}

char *INI_GetString(char *section, char *key, char *this)
{
    char    *value;

    if ((value = GetValue(section, key)) == NULL)
    {
        return this;
    }

    return value;
}

int INI_GetInteger(char *section, char *key, int this)
{
    char    *value, n[12];
    int     number;

    if ((value = GetValue(section, key)) == NULL)
    {
        return this;
    }

    number = atoi(value);
    sprintf(n, "%i", number);

    if (strcmp(value, n) == 0)
    {
        return number;
    }

    return this;
}

int INI_GetBoolean(char *section, char *key, int this)
{
    char    *value;

    if ((value = GetValue(section, key)) == NULL)
    {
        return this;
    }

    if (strcasecmp(value, "False") == 0)
    {
        return 0;
    }

    if (strcasecmp(value, "True") == 0)
    {
        return 1;
    }

    return this;
}
