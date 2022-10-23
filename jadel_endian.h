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

    extern uint32 ALPHA_MASK;
    extern uint32 RED_MASK;
    extern uint32 GREEN_MASK;
    extern uint32 BLUE_MASK;

    extern uint32 ALPHA_SHIFT;
    extern uint32 RED_SHIFT;
    extern uint32 GREEN_SHIFT;
    extern uint32 BLUE_SHIFT;

    void determineEndianness();

}