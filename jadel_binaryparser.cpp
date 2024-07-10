#include "jadel_binaryparser.h"
#include "jadel_util.h"

namespace jadel
{
    BinaryParser::BinaryParser(const void *data, bool flipByteOrder)
        : data(data), pointer((uint8 *)data), flipByteOrder(flipByteOrder)
    {
    }

    BinaryParser::BinaryParser(const void *data)
        : BinaryParser(data, false)
    {
    }

    int8 BinaryParser::getINT8()
    {
        int8 result = *(int8 *)pointer;
        ++pointer;
        return result;
    }

    int16 BinaryParser::getINT16()
    {
        int16 result = *(int16 *)pointer;
        if (flipByteOrder)
            flipBytes(&result, 2);
        pointer += 2;
        return result;
    }

    int32 BinaryParser::getINT32()
    {
        int32 result = *(int32 *)pointer;
        if (flipByteOrder)
            flipBytes(&result, 4);
        pointer += 4;
        return result;
    }

    int64 BinaryParser::getINT64()
    {
        int64 result = *(int64 *)pointer;
        if (flipByteOrder)
            flipBytes(&result, 8);
        pointer += 8;
        return result;
    }

    uint8 BinaryParser::getUINT8()
    {
        uint8 result = *(uint8 *)pointer;
        ++pointer;
        return result;
    }

    uint16 BinaryParser::getUINT16()
    {
        uint16 result = *(uint16 *)pointer;
        if (flipByteOrder)
            flipBytes(&result, 2);
        pointer += 2;
        return result;
    }

    uint32 BinaryParser::getUINT32()
    {
        uint32 result = *(uint32 *)pointer;
        if (flipByteOrder)
            flipBytes(&result, 4);
        pointer += 4;
        return result;
    }

    uint64 BinaryParser::getUINT64()
    {
        uint64 result = *(uint64 *)pointer;
        if (flipByteOrder)
            flipBytes(&result, 8);
        pointer += 8;
        return result;
    }

    void BinaryParser::advance(int numBytes)
    {
        pointer += numBytes;
    }

    void BinaryParser::reset()
    {
        pointer = (uint8 *)data;
    }
}