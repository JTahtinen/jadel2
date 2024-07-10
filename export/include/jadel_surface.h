#pragma once

namespace jadel
{
    struct Surface
    {
        int width;
        int height;
        void *pixels;
    };

    DECLSPEC bool graphicsCreateSurface(int width, int height, Surface *target);
    DECLSPEC bool graphicsFreeSurface(Surface *target);
}