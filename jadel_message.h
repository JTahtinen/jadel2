#pragma once
#include <stdarg.h>
#include <stdio.h>
#include "jadel_update.h"

static char stringBuffer[50];

namespace jadel
{
    inline void message(const char *content, ...)
    {
        va_list ap; /* points to each unnamed arg in turn */
        const char *p, *sval;
        int ival;
        double dval;
        va_start(ap, content); /* make ap point to 1st unnamed arg */
        memset(stringBuffer, 0, 50);
        for (p = content; *p; p++)
        {
            if (*p != '%')
            {
                writeToConsole(p, 1);
                continue;
            }
            switch (*++p)
            {
            case 'd':
                ival = va_arg(ap, int);
                sprintf(stringBuffer, "%d", ival);
                writeToConsole(stringBuffer, strlen(stringBuffer));
                break;
            case 'f':
                dval = va_arg(ap, double);
                sprintf(stringBuffer, "%f", dval);
                writeToConsole(stringBuffer, strlen(stringBuffer));
                break;
            case 's':
                for (sval = va_arg(ap, char *); *sval; sval++)
                    writeToConsole(sval, 1);
                break;
            default:
                writeToConsole(p, 1);
                break;
            }
        }
        va_end(ap);

    }
}