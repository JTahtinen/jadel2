#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "jadel_png.h"
#include "jadel_util.h"

namespace jadel
{
    void imageLoadInit()
    {
        stbi_set_flip_vertically_on_load(true);
    }

    bool loadPNG(const char *filename, jadel::Surface *target)
    {
        int width;
        int height;
        int channels;
        target->pixels = stbi_load(filename, &width, &height, &channels, 0);
        if (!target->pixels)
            return false;
        for (int i = 0; i < width * height; ++i)
        {
            uint8 *pixel = (uint8 *)target->pixels + (channels * i);
            jadel::flipBytes(pixel, 3);
        }
        target->width = width;
        target->height = height;
        return true;
    }
}
/*#include <stdlib.h>
#include "jadel_png.h"
#include "jadel_util.h"
#include "jadel_file.h"
#include "jadel_endian.h"
#include "jadel_defs.h"
#include "jadel_array.h"
#include "jadel_graphics.h"
namespace jadel
{
    enum COLOR_TYPE
    {
        COLOR_TYPE_GRAYSCALE = 0,
        COLOR_TYPE_RGB = 2,
        COLOR_TYPE_PALETTE = 3,
        COLOR_TYPE_GRAYSCALE_ALPHA = 4,
        COLOR_TYPE_RGB_ALPHA = 6
    };

    enum FILTER_TYPE
    {
        FILTER_METHOD_NONE = 0,
        FILTER_METHOD_SUB,
        FILTER_METHOD_UP,
        FILTER_METHOD_AVERAGE,
        FILTER_METHOD_PAETH
    };

    struct IHDR
    {
        uint32 width;
        uint32 height;
        uint8 bitDepth;
        uint8 colorType;
        uint8 compressionMethod;
        uint8 filterMethod;
        uint8 interlanceMethod;
    };

    struct PaletteEntry
    {
        uint8 r;
        uint8 g;
        uint8 b;
    };

    bool processChunks(Array<PNGChunk> &chunks, Surface *target)
    {
        IHDR header;
        Array<PaletteEntry> paletteEntries(256);
        bool PLTEmandatory = false;
        bool PLTEpresent = false;
        if (!compareHexToString(chunks[0].type, "IHDR", false))
        {
            printf("ERROR: First PNG chunk not IHDR!\n");
            return false;
        }
        for (size_t i = 0; i < chunks.size; ++i)
        {
            PNGChunk *chunk = &chunks[i];
            if (compareHexToString(chunk->type, "IHDR", false))
            {
                if (i != 0)
                {
                    printf("ERROR: IHDR at PNG chunk index %zd\n", i);
                    return false;
                }
                printf("Handle IHDR\n");

                BinaryParser parser(chunk->data, true);
                header.width = parser.getINT32();
                header.height = parser.getINT32();
                header.bitDepth = parser.getUINT8();
                header.colorType = parser.getUINT8();
                header.compressionMethod = parser.getUINT8();
                header.filterMethod = parser.getUINT8();
                header.interlanceMethod = parser.getUINT8();

                if (header.colorType == COLOR_TYPE_PALETTE)
                    PLTEmandatory = true;
            }
            else if (compareHexToString(chunk->type, "PLTE", false))
            {
                PLTEpresent = true;
                if (chunk->length % 3 != 0)
                {
                    printf("ERROR: PLTE length invalid! Must be divisible by 3!\n");
                    return false;
                }
                if (header.colorType == COLOR_TYPE_GRAYSCALE || header.colorType == COLOR_TYPE_GRAYSCALE_ALPHA)
                {
                    printf("ERROR: PLTE present with invalid color type\n");
                    return false;
                }
                BinaryParser parser(chunk->data, true);
                for (int i = 0; i < chunk->length / 3; ++i)
                {
                    PaletteEntry entry;
                    entry.r = parser.getUINT8();
                    entry.g = parser.getUINT8();
                    entry.b = parser.getUINT8();
                    paletteEntries.push(entry);
                }
                printf("Handle PLTE\n");
            }
            else if (compareHexToString(chunk->type, "IDAT", false))
            {
                target->pixels = malloc(header.width * header.height * sizeof(uint32));
                target->width = header.width;
                target->height = header.height;
                switch (header.colorType)
                {
                case COLOR_TYPE_GRAYSCALE:
                    printf("Color type grayscale\n");
                    break;
                case COLOR_TYPE_RGB:
                    printf("Color type RGB\n");
                    break;
                case COLOR_TYPE_PALETTE:
                {
                    printf("Color type palette\n");
                    BinaryParser parser(chunk->data);
                    uint32 *targetPixels = (uint32 *)target->pixels;
                    for (int y = 0; y < target->height; ++y)
                    {
                        for (int x = 0; i < target->width; ++x)
                        {
                            uint8 paletteIndex = parser.getUINT8();
                            PaletteEntry &entry = paletteEntries[paletteIndex];
                            uint32 pixel = graphicsCreateColor(255, entry.r, entry.g, entry.b);
                            targetPixels[x + y * target->width] = pixel;
                        }
                    }
                    break;
                }
                case COLOR_TYPE_GRAYSCALE_ALPHA:
                    printf("Color type grayscale alpha\n");
                    break;
                case COLOR_TYPE_RGB_ALPHA:
                {
                    printf("Color type RGBA\n");
                    BinaryParser parser(chunk->data, true);
                    uint32 *targetPixels = (uint32 *)target->pixels;
                    switch (header.filterMethod)
                    {
                    case FILTER_METHOD_NONE:
                        printf("FILTER_METHOD_NONE\n");
                        for (int y = 0; y < target->height; ++y)
                        {
                            for (int x = 0; x < target->width; ++x)
                            {
                                uint32 pixel = parser.getUINT32();
                                targetPixels[x + y * target->width] = pixel;
                            }
                        }
                        break;
                    case FILTER_METHOD_SUB:
                        printf("FILTER_METHOD_SUB\n");
                        break;
                    case FILTER_METHOD_UP:
                        printf("FILTER_METHOD_UP\n");
                        break;
                    case FILTER_METHOD_AVERAGE:
                        printf("FILTER_METHOD_AVERAGE\n");
                        break;
                    case FILTER_METHOD_PAETH:
                        printf("FILTER_METHOD_PAETH\n");
                        break;
                    default:
                        printf("ERROR: INVALID FILTER METHOD\n");
                        return false;
                    }
                    break;
                }
                default:
                    printf("ERROR: Invalid color type!\n");
                    return false;
                }
                printf("Handle IDAT\n");
            }
            else if (compareHexToString(chunk->type, "IEND", false))
            {
                printf("Handle IEND\n");
                if (i < chunks.size - 1)
                {
                    printf("Error: IEND not the last chunk\n");
                }
            }
            else if (compareHexToString(chunk->type, "sRGB", false))
            {
                printf("Handle sRGB\n");
            }
            else if (compareHexToString(chunk->type, "cHRM", false))
            {
                printf("Handle cHRM\n");
            }
            else if (compareHexToString(chunk->type, "gAMA", false))
            {
                printf("Handle gAMA\n");
            }
            else if (compareHexToString(chunk->type, "sBit", false))
            {
                printf("Handle sBit\n");
            }
            else if (compareHexToString(chunk->type, "bKGD", false))
            {
                printf("Handle bKGD\n");
            }
            else if (compareHexToString(chunk->type, "hIST", false))
            {
                printf("Handle hIST\n");
            }
            else if (compareHexToString(chunk->type, "tRNS", false))
            {
                printf("Handle tRNS\n");
            }
            else if (compareHexToString(chunk->type, "pHYs", false))
            {
                printf("Handle pHYs\n");
            }
            else if (compareHexToString(chunk->type, "tIME", false))
            {
                printf("Handle tIME\n");
            }
            else if (compareHexToString(chunk->type, "tEXt", false))
            {
                printf("Handle tEXt\n");
            }
            else if (compareHexToString(chunk->type, "zTXt", false))
            {
                printf("Handle zTXt\n");
            }
            else
            {
                printf("Invalid PNG chunk type: ");
                printHexAsString(chunk->type, 4, false);
                printf("\n");
                // return false;
            }
        }
        if (PLTEmandatory && !PLTEpresent)
        {
            printf("ERROR: No PLTE present even though mandatory\n");
            return false;
        }
        return true;
    }

    bool loadPNG(const char *filepath, Surface *target)
    {
        size_t fileSize = getBinaryFileSize(filepath);
        if (fileSize == 0)
            return false;
        uint64 signature;
        readBinaryFileBytes(filepath, 0, 8, &signature);
        uint64 PNGSignature =
            ((uint64)137 << 56 | (uint64)80 << 48 | (uint64)78 << 40 | (uint64)71 << 32 | (uint64)13 << 24 | (uint64)10 << 16 | (uint64)26 << 8 | (uint64)10);
        if (endianness == LITTLE_ENDIAN)
        {
            flipBytes(&signature, 8);
        }
        if (signature != PNGSignature)
        {
            return false;
        }
        void *pngBuffer = malloc(fileSize - 8);
        if (!readBinaryFileBytes(filepath, 8, fileSize - 8, pngBuffer))
        {
            free(pngBuffer);
            return false;
        }
        // PNGChunk chunks[20];
        Array<PNGChunk> chunks(20);

        // int chunkIndex = 0;
        PNGChunk currentChunk;
        BinaryParser parser(pngBuffer);
        if (endianness == LITTLE_ENDIAN)
        {
            parser.flipByteOrder = true; // PNG file format is big-endian.
        }
        uint8 *currentType;
        size_t currentChunkOffset = 0;
        do
        {
            currentChunk.length = parser.getUINT32();
            for (int i = 0; i < 4; ++i)
                currentChunk.type[i] = parser.getUINT8();
            if (currentChunk.length > 0)
            {
                currentChunk.data = malloc(currentChunk.length);
                memcpy(currentChunk.data, (uint8 *)pngBuffer + currentChunkOffset + 8, currentChunk.length);
            }
            parser.advance(currentChunk.length);
            currentChunk.CRC = parser.getUINT32();
            currentType = currentChunk.type;

            currentChunkOffset += 16 + currentChunk.length;
            chunks.push(currentChunk);

            printHexAsString((uint8 *)&currentType[0], 4, false);
        } while (!compareHexToString((uint8 *)&currentType[0], "IEND", false));

        printf("Chunks loaded: %zd\n", chunks.size);
        processChunks(chunks, target);
        return true;
    }
}*/