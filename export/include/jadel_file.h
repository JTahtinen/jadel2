#pragma once

#include <stdio.h>
#include "jadel_binaryparser.h"
#include "jadel_math.h"
#include "jadel_endian.h"
#include "jadel_graphics.h"
#include "jadel_memory.h"
#include "jadel_message.h"
#include "jadel_defs.h"

#include <stdlib.h>

namespace jadel
{
    enum
    {
        BINARY_FILE_INIT_EMPTY,
        BINARY_FILE_INIT_FILLED
    };

    struct DECLSPEC BinaryFile
    {
        void *data;
        size_t fileBufferSize;
        size_t numBytesWritten;
        uint8 *writePointer;
        uint8 *readPointer;

        BinaryFile(size_t size);
        BinaryFile(const char *filepath);
        BinaryFile() = default;

        size_t numBytesRead() const;

        void close();
        bool init(void *data, size_t size, uint32 initFlag);
        bool init(size_t size);
        bool init(const char *filepath);
        bool setWritePointerOffset(size_t offset);
        bool setReadPointerOffset(size_t offset);
        bool advanceWritePointer(size_t numBytes);
        bool advenceReadPointer(size_t numBytes);

        bool writeNBytes(size_t numBytes, void *src);
        bool writeInt(int val);
        bool writeUint32(uint32 val);
        bool writeUint64(uint64 val);
        bool writeBool(bool val);
        bool writeFloat(float val);
        bool writeDouble(double val);
        bool writeChar(char val);
        
        // Pads the end of the buffer with zeros 
        bool writeString(const char* val, size_t maxStringLength, size_t bufferSize);
        bool writeString(const char* val, size_t bufferSize);
        bool readNBytes(void *dst, size_t numBytes);
        bool readInt(int *dst);
        bool readUint32(uint32 *dst);
        bool readUint64(uint64 *dst);
        bool readBool(bool *dst);
        bool readFloat(float *dst);
        bool readDouble(double *dst);
        bool readChar(char* dst);
        bool readString(char* dst, size_t expectedStringLengthIncTerminator, size_t bufferSize);


        bool writeToFile(const char *filepath);
    };

    inline size_t getFileSize(FILE *fp)
    {
        fseek(fp, 0, SEEK_END);
        size_t result = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        return result;
    }

    inline size_t getBinaryFileSize(const char *filepath)
    {
        FILE *fp = fopen(filepath, "rb");
        if (!fp)
            return 0;
        size_t result = getFileSize(fp);
        return result;
    }

    inline size_t getTextFileSize(const char *filepath)
    {
        FILE *fp = fopen(filepath, "r");

        if (!fp)
            return 0;
        size_t result = getFileSize(fp);
        return result;
    }

    inline bool readTextFile(const char *filepath, char *target, int bufferSize, size_t *numCharacters)
    {
        FILE *fp = fopen(filepath, "r");
        if (!fp)
            return false;

        *numCharacters = 0;
        while (*numCharacters < bufferSize)
        {
            char c = fgetc(fp);
            if (c == EOF)
                break;
            target[(*numCharacters)++] = c;
        }
        fclose(fp);
        return true;
    }

    inline bool readTextFileAndReserveMemory(const char *filepath, char **target, size_t *numCharacters)
    {
        FILE *fp = fopen(filepath, "r");
        if (!fp)
            return false;
        size_t fileSize = getFileSize(fp) + 1; // Null terminator
        char *data = (char *)jadel::memoryAllocate(fileSize);
        if (!data)
            return false;
        bool result = readTextFile(filepath, data, fileSize, numCharacters);
        *target = data;
        return result;
    }

    inline bool readBinaryFile(const char *filepath, void *buffer, size_t bufferSize, size_t *numBytesRead)
    {
        FILE *fp = fopen(filepath, "rb");
        if (!fp)
            return false;

        *numBytesRead = 0;

        fseek(fp, 0, SEEK_END);
        size_t fileSize = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        if (bufferSize < fileSize)
        {
            jadel::message("Buffer size too low: %zd bytes, file size is %zd bytes\n", bufferSize, fileSize);
            return false;
        }
        fread(buffer, fileSize, 1, fp);
        *numBytesRead = fileSize;
        return true;
    }

    inline bool readBinaryFileBytes(const char *filepath, size_t offset, size_t numBytes, void *buffer)
    {
        FILE *fp = fopen(filepath, "rb");
        if (!fp)
            return false;

        fseek(fp, offset, SEEK_SET);

        fread(buffer, numBytes, 1, fp);
        fclose(fp);
        return true;
    }

    inline bool writeBinaryFile(const char *filepath, const void *data, size_t sizeInBytes)
    {
        FILE *fp = fopen(filepath, "wb");
        if (!fp)
            return false;

        fwrite(data, sizeInBytes, 1, fp);

        fclose(fp);
        return true;
    }

    struct BMPHeader
    {
        uint16 fileType;
        uint32 fileSize;
        uint32 offset;
    };

    inline bool loadBMP(const char *filepath, jadel::Surface *target)
    {
        size_t fileSize = getBinaryFileSize(filepath);
        if (fileSize == 0)
            return false;
        BMPHeader header;
        void *headerBuffer = malloc(26);
        readBinaryFileBytes(filepath, 0, 26, headerBuffer);
        jadel::BinaryParser parser(headerBuffer);
        if (endianness == BIG_ENDIAN)
        {
            parser.flipByteOrder = true; // BMP file format is little-endian.
        }
        header.fileType = parser.getUINT16();
        const char *ft = (const char *)&header.fileType;
        if (ft[0] != 'B' || ft[1] != 'M')
            return false;
        header.fileSize = parser.getUINT32();
        parser.advance(4);
        header.offset = parser.getUINT32();
        parser.advance(4);
        int width = parser.getINT32();
        int height = parser.getINT32();
        void *buffer = malloc(width * height * 4);
        int numBytes;
        if (!readBinaryFileBytes(filepath, header.offset, width * height * 4, buffer))
            return false;

        target->width = width;
        target->height = height;
        target->pixels = buffer;

        free(headerBuffer);
        return true;
    }

}