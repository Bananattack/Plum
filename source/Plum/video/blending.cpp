#include <SDL_opengl.h>


#include "blending.h"

namespace plum
{
    namespace
    {
        int globalAlpha = 255;
        BlendMode blendMode = BlendPreserve;
    }

    BlendMode getBlendMode()
    {
        return blendMode;
    }

    void setBlendMode(BlendMode mode)
    {
        blendMode = mode;
    }

    int getOpacity()
    {
        return globalAlpha;
    }

    void setOpacity(int alpha)
    {
        globalAlpha = alpha;
    }

    void useHardwareBlender(BlendMode mode)
    {
        switch(mode)
        {
            case BlendOpaque:
                glDisable(GL_BLEND);
                break;
            case BlendMerge:
            case BlendPreserve:
            default:
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                break;
            case BlendAdd:
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                break;
            case BlendSubtract:
                glDisable(GL_BLEND);
                break;
        }
    }

    void useHardwareColor(int r, int g, int b, int a) 
    {
        return glColor4ub(r, g, b, a * globalAlpha / 255);
    }
}
