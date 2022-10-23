#include "jadel_endian.h"

namespace jadel
{
    uint32 ALPHA_MASK;
    uint32 RED_MASK;
    uint32 GREEN_MASK;
    uint32 BLUE_MASK;

    uint32 ALPHA_SHIFT;
    uint32 RED_SHIFT;
    uint32 GREEN_SHIFT;
    uint32 BLUE_SHIFT;

    Endianness endianness;
    
    void determineEndianness()
    {
        uint32 x = 0x76543210;
        char *c = (char *)&x;

        if (*c == 0x10)
        {
            endianness = LITTLE_ENDIAN;

            ALPHA_MASK  = 0xff000000;
            RED_MASK    = 0x00ff0000;
            GREEN_MASK  = 0x0000ff00;
            BLUE_MASK   = 0x000000ff;

            ALPHA_SHIFT = 24;
            RED_SHIFT   = 16;
            GREEN_SHIFT = 8;
            BLUE_SHIFT  = 0;
        }
        else
        {
            endianness = BIG_ENDIAN;

            ALPHA_MASK  = 0x000000ff;
            RED_MASK    = 0x0000ff00;
            GREEN_MASK  = 0x00ff0000;
            BLUE_MASK   = 0xff000000;

            ALPHA_SHIFT = 0;
            RED_SHIFT   = 8;
            GREEN_SHIFT = 16;
            BLUE_SHIFT  = 24;
        }

    }
}