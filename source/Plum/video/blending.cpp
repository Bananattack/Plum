#include "../plum.h"

namespace Plum
{
	int _globalAlpha = 255;
	BlendMode _blendMode = BlendPreserve;

	void (PLUM_CALLBACK *glBlendEquationEXT)(int);

	void PLUM_CALLBACK NoBlendExtension(int a)
	{
	}

	BlendMode getBlendMode()
	{
		printf("Getting mode (_bM = %d)", _blendMode);
		return _blendMode;
	}

	void setBlendMode(BlendMode mode)
	{
		_blendMode = mode;
		printf("Setting mode (_bM = %d, m = %d)", _blendMode, mode);
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
