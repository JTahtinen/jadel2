#pragma once

#include "jadel_graphics.h"

namespace jadel
{
    struct PNGChunk
    {
        uint32 length;
        uint8 type[4];
        void *data;
        uint32 CRC;
    };

    extern DECLSPEC bool loadPNG(const char *filepath, Surface *target);
}