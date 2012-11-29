#include <SDL_opengl.h>


#include "blending.h"

namespace plum
{
    namespace
    {
        int globalAlpha = 255;
    }

    int getOpacity()
    {
        return globalAlpha;
    }

    void setOpacity(int alpha)
    {
        globalAlpha = alpha;
    }
}
