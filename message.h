#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdarg.h>

namespace jadel
{
    inline void message(const char *content, ...)
    {
        va_list ap;
        va_start(ap, content);
    }
}
#endif