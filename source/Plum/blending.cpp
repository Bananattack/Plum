#include "plum.h"

namespace Plum
{
	int _globalAlpha = 255;
	extern BlendMode _blendMode = BlendPreserve;

	void (PLUM_CALLBACK *glBlendEquationEXT)(int);

	void PLUM_CALLBACK NoBlendExtension(int a)
	{
	}
}
