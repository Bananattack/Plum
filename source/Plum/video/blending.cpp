#include "../plum.h"

namespace plum
{
    int _globalAlpha = 255;
    BlendMode _blendMode = BlendPreserve;

    void (PLUM_CALLBACK *glBlendEquationEXT)(int);

    void PLUM_CALLBACK NoBlendExtension(int a)
    {
    }

    BlendMode getBlendMode()
    {
        return _blendMode;
    }

    void setBlendMode(BlendMode mode)
    {
        _blendMode = mode;
    }

    int getOpacity()
    {
        return _globalAlpha;
    }

    void setOpacity(int alpha)
    {
        _globalAlpha = alpha;
    }
}
