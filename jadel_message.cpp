#include "jadel_message.h"
#include <stdarg.h>
#include <stdio.h>
#include "jadel_update.h"

#define JADEL_MESSAGE_STRING_BUFFER_SIZE (50)
static char jadelMessageStringBuffer[JADEL_MESSAGE_STRING_BUFFER_SIZE];

namespace jadel
{
    void message(const char *content, ...)
    {
        va_list ap; /* points to each unnamed arg in turn */
        const char *p, *sval;
        int ival;
        double dval;
        va_start(ap, content); /* make ap point to 1st unnamed arg */
        memset(jadelMessageStringBuffer, 0, JADEL_MESSAGE_STRING_BUFFER_SIZE);
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
                snprintf(jadelMessageStringBuffer, JADEL_MESSAGE_STRING_BUFFER_SIZE, "%d", ival);
                writeToConsole(jadelMessageStringBuffer, strlen(jadelMessageStringBuffer));
                break;
            case 'f':
                dval = va_arg(ap, double);
                snprintf(jadelMessageStringBuffer, JADEL_MESSAGE_STRING_BUFFER_SIZE, "%f", dval);
                writeToConsole(jadelMessageStringBuffer, strlen(jadelMessageStringBuffer));
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