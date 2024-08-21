#include "jadel_file.h"
#include <memory.h>
#include <string.h>

namespace jadel
{
    static bool isReadPossible(const BinaryFile *file, size_t numBytesToRead)
    {
        size_t remainingBytes = file->numBytesWritten - file->numBytesRead();
        bool result = remainingBytes >= numBytesToRead;
        return result;
    }

    static bool isWritePossible(const BinaryFile *file, size_t numBytesToWrite)
    {
        size_t availableBytes = file->fileBufferSize - file->numBytesWritten;
        bool result = availableBytes >= numBytesToWrite;
        return result;
    }

    BinaryFile::BinaryFile(size_t size)
    {
        init(size);
    }

    BinaryFile::BinaryFile(const char *filepath)
    {
        init(filepath);
    }

    size_t BinaryFile::numBytesRead() const
    {
        size_t result = readPointer - this->data;
        return result;
    }

    void BinaryFile::close()
    {
        jadel::memoryFree(this->data);
        this->data = NULL;
        this->readPointer = NULL;
        this->writePointer = NULL;
    }

    bool BinaryFile::init(void *data, size_t size, uint32 initFlag)
    {
        if (!data)
        {
            this->data = NULL;
            this->fileBufferSize = 0;
            this->numBytesWritten = 0;
            this->writePointer = NULL;
            this->readPointer = NULL;
            return false;
        }
        this->data = data;
        this->fileBufferSize = size;
        if (initFlag == BINARY_FILE_INIT_EMPTY)
        {
            this->numBytesWritten = 0;
        }
        else
        {
            this->numBytesWritten = this->fileBufferSize;
        }
        setWritePointerOffset(0);
        setReadPointerOffset(0);
        return true;
    }

    bool BinaryFile::init(size_t size)
    {
        void *data = (jadel::memoryAllocate(size));
        return init(data, size, BINARY_FILE_INIT_EMPTY);
    }

    bool BinaryFile::init(const char *filepath)
    {
        size_t expectedFileSize = getBinaryFileSize(filepath);
        if (expectedFileSize == 0)
            return false;
        void *data = jadel::memoryAllocate(expectedFileSize);
        size_t fileSize;
        if (!readBinaryFile(filepath, data, expectedFileSize, &fileSize))
        {
            return false;
        }
        if (expectedFileSize != fileSize)
        {
            jadel::memoryFree(data);
            return false;
        }
        return init(data, fileSize, BINARY_FILE_INIT_FILLED);
    }

    bool BinaryFile::setWritePointerOffset(size_t offset)
    {
        if (offset > this->fileBufferSize)
        {
            return false;
        }
        this->writePointer = (uint8 *)this->data + offset;
        return true;
    }

    bool BinaryFile::setReadPointerOffset(size_t offset)
    {
        if (offset > this->fileBufferSize)
        {
            return false;
        }
        this->readPointer = (uint8 *)this->data + offset;
        return true;
    }

    bool BinaryFile::advanceWritePointer(size_t numBytes)
    {
        size_t dataOffset = (this->writePointer + numBytes) - this->data;
        if (dataOffset > this->fileBufferSize)
        {
            return false;
        }
        this->writePointer += numBytes;
        return true;
    }

    bool BinaryFile::advenceReadPointer(size_t numBytes)
    {
        size_t dataOffset = (this->readPointer + numBytes) - this->data;
        if (dataOffset > this->fileBufferSize)
        {
            return false;
        }
        this->readPointer += numBytes;
        return true;
    }

    bool BinaryFile::writeNBytes(size_t numBytes, void *val)
    {
        if (!val || !isWritePossible(this, numBytes))
        {
            jadel::message("Binary File: Write failed!\n");
            return false;
        }
        memmove(this->writePointer, val, numBytes);
        this->numBytesWritten += numBytes;
        this->writePointer = (uint8 *)this->data + numBytesWritten;
        return true;
    }

    bool BinaryFile::writeInt(int val)
    {
        return writeNBytes(sizeof(int), &val);
    }

    bool BinaryFile::writeUint32(uint32 val)
    {
        return writeNBytes(sizeof(uint32), &val);
    }

    bool BinaryFile::writeUint64(uint64 val)
    {
        return writeNBytes(sizeof(uint64), &val);
    }

    bool BinaryFile::writeBool(bool val)
    {
        return writeNBytes(sizeof(bool), &val);
    }

    bool BinaryFile::writeFloat(float val)
    {
        return writeNBytes(sizeof(float), &val);
    }

    bool BinaryFile::writeDouble(double val)
    {
        return writeNBytes(sizeof(double), &val);
    }

    bool BinaryFile::writeChar(char val)
    {
        return writeNBytes(sizeof(char), &val);
    }

    bool BinaryFile::writeString(const char *val, size_t maxStringLength, size_t bufferSize)
    {
        size_t stringLength = JADEL_CLAMP(maxStringLength, 0, bufferSize);      
        bool result = writeNBytes(stringLength * sizeof(char), (void *)val);
        if (!result) 
        {
            return false;
        }
        for (size_t i = 0; i < bufferSize - stringLength; ++i)
        {
            result = writeChar((char)0);
            if (!result)
            {
                return false;
            }
        }
        return result;
    }

    bool BinaryFile::writeString(const char *val, size_t bufferSize)
    {
        size_t len = strnlen(val, bufferSize - 1);
        return writeString(val, len, bufferSize);
    }

    bool BinaryFile::readNBytes(void *dst, size_t numBytes)
    {
        if (!dst || !isReadPossible(this, numBytes))
        {
            jadel::message("Binary File: read failed!\n");
            return false;
        }
        memmove(dst, this->readPointer, numBytes);
        this->readPointer += numBytes;
        return true;
    }

    bool BinaryFile::readInt(int *dst)
    {
        return readNBytes(dst, sizeof(int));
    }

    bool BinaryFile::readUint32(uint32 *dst)
    {
        return readNBytes(dst, sizeof(uint32));
    }

    bool BinaryFile::readUint64(uint64 *dst)
    {
        return readNBytes(dst, sizeof(uint64));
    }

    bool BinaryFile::readBool(bool *dst)
    {
        return readNBytes(dst, sizeof(bool));
    }

    bool BinaryFile::readFloat(float *dst)
    {
        return readNBytes(dst, sizeof(float));
    }

    bool BinaryFile::readDouble(double *dst)
    {
        return readNBytes(dst, sizeof(double));
    }

    bool BinaryFile::readChar(char *dst)
    {
        return readNBytes(dst, sizeof(char));
    }

    bool BinaryFile::readString(char *dst, size_t expectedStringLengthIncTerminator, size_t bufferSize)
    {
        size_t stringLength = JADEL_CLAMP(expectedStringLengthIncTerminator, 0, bufferSize);
        size_t i = 0;
        for (; i < stringLength; ++i)
        {
            if (!readChar(dst + i))
            {
                return false;
            }
        }
        for (; i < bufferSize; ++i)
        {
            dst[i] = 0;
        }
        return true;
    }

    bool BinaryFile::writeToFile(const char *filepath)
    {
        if (!this->data)
        {
            return false;
        }

        bool result = writeBinaryFile(filepath, this->data, numBytesWritten);
        return result;
    }
}