#include "jadel_defs.h"
#include "jadel_surface.h"
#include "jadel_memory.h"

namespace jadel
{
    bool graphicsCreateSurface(int width, int height, Surface *target)
    {
        if (!target || width < 0 || height < 0)
        {
            return false;
        }
        target->width = width;
        target->height = height;
        //target->pixels = JADEL_MALLOC(uint32, width * height);
        target->pixels = (uint32*)jadel::memoryAllocate(width * height * sizeof(uint32));
        if (!target->pixels)
        {
            return false;
        }
        return true;
    }

    bool graphicsFreeSurface(Surface *target)
    {
        if (!target)
        {
            return false;
        }
        jadel::memoryFree(target->pixels);
        target->pixels = NULL;
        return true;
    }
}