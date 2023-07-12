#include "jadel_file.h"
#include <memory.h>

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
        size_t result = pointerToLastReadByte - this->data;
        return result;
    }

    void BinaryFile::close()
    {
        jadel::memoryFree(this->data);
        this->data = NULL;
        this->pointerToLastReadByte = NULL;
        this->pointerToLastWrittenByte = NULL;
    }

    bool BinaryFile::init(void *data, size_t size, uint32 initFlag)
    {
        if (!data)
        {
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
        void *data = (jadel::memoryReserve(size));
        return init(data, size, BINARY_FILE_INIT_EMPTY);
    }

    bool BinaryFile::init(const char *filepath)
    {
        size_t expectedFileSize = getBinaryFileSize(filepath);
        if (expectedFileSize == 0)
            return false;
        void *data = jadel::memoryReserve(expectedFileSize);
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
        this->pointerToLastWrittenByte = (uint8 *)this->data + offset;
        return true;
    }

    bool BinaryFile::setReadPointerOffset(size_t offset)
    {
        if (offset > this->fileBufferSize)
        {
            return false;
        }
        this->pointerToLastReadByte = (uint8 *)this->data + offset;
        return true;
    }

    bool BinaryFile::writeNBytes(size_t numBytes, void *val)
    {
        if (!val || !isWritePossible(this, numBytes))
        {
            return false;
        }
        memmove(this->pointerToLastWrittenByte, val, numBytes);
        this->numBytesWritten += numBytes;
        this->pointerToLastWrittenByte = (uint8*)this->data + numBytesWritten;
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

    bool BinaryFile::readNBytes(void *dst, size_t numBytes)
    {
        if (!dst || !isReadPossible(this, numBytes))
        {
            return false;
        }
        memmove(dst, this->pointerToLastReadByte, numBytes);
        this->pointerToLastReadByte += numBytes;
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