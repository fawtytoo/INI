
// INI_Open(filename)
// returns 0 on success
//    "    1 on file error
//    "    2 if no sections found
//    "    3 on internal memory error
int INI_Open(char *);

// INI_Close
void INI_Close(void);

// INI_GetString(section, key, default)
// returns key value on success
//    "    default on failure
char *INI_GetString(char *, char *, char *);

// INI_GetInteger(section, key, default)
// returns key value on success
//    "    default on failure
int INI_GetInteger(char *, char *, int);

// INI_GetBoolean(section, key, default)
// boolean values in the INI file are keywords: False/True (case insensitive)
// boolean values for default and return value are: 0=false, 1=true
// returns key value(0 or 1) on success
//    "    default on failure
int INI_GetBoolean(char *, char *, int);
