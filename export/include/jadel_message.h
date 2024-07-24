#pragma once
#include "jadel_defs.h"

namespace jadel
{
    DECLSPEC void message(const char *content, ...);
}

#ifdef DEBUG
#define JADEL_DEBUGMSG(content, ...) jadel::message(content, __VA_ARGS__)
#else
#define JADEL_DEBUGMSG(content, ...)
#endif