#pragma once
#include "jadel_defs.h"

namespace jadel
{
    enum Endianness
    {
        LITTLE_ENDIAN,
        BIG_ENDIAN
    };

    extern DECLSPEC Endianness endianness;

    extern DECLSPEC uint32 ALPHA_MASK;
    extern DECLSPEC uint32 RED_MASK;
    extern DECLSPEC uint32 GREEN_MASK;
    extern DECLSPEC uint32 BLUE_MASK;

    extern DECLSPEC uint32 ALPHA_SHIFT;
    extern DECLSPEC uint32 RED_SHIFT;
    extern DECLSPEC uint32 GREEN_SHIFT;
    extern DECLSPEC uint32 BLUE_SHIFT;

    void determineEndianness();

}