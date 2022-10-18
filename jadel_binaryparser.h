#pragma once

#include "jadel_defs.h"
namespace jadel
{
    struct DECLSPEC BinaryParser
    {
        const void *data;
        const uint8 *pointer;
        bool flipByteOrder;

        BinaryParser(const void *data, bool flipByteOrder);
        BinaryParser(const void *data);

        int8 getINT8();
        int16 getINT16();
        int32 getINT32();
        int64 getINT64();

        uint8 getUINT8();
        uint16 getUINT16();
        uint32 getUINT32();
        uint64 getUINT64();

        void advance(int numBytes);

        void reset();
    };
}