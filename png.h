#ifndef PNG_H
#define PNG_H
#include "graphics.h"

namespace jadel
{
    struct PNGChunk
    {
        uint32 length;
        uint8 type[4];
        void *data;
        uint32 CRC;
    };

    extern DLLIMPORT bool loadPNG(const char *filepath, Surface *target);
}
#endif