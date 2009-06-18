#ifndef PLUM_BLENDMODE_H
#define PLUM_BLENDMODE_H

#ifndef GL_FUNC_REVERSE_SUBTRACT_EXT
#	define GL_FUNC_REVERSE_SUBTRACT_EXT 0x800B
#endif
#ifndef GL_FUNC_ADD_EXT
#	define GL_FUNC_ADD_EXT 0x8006
#endif

namespace Plum
{
	enum BlendMode
	{
		BlendUnspecified,	// Used internally to allow optional arguments on blitters.
		BlendOpaque,		// Direct copy to destination
		BlendMerge,		// Combines two images, altering alpha as needed.
		BlendPreserve,	// Combines two images, preserving alpha of the destination.
		BlendAdd,		// Adds the RGB value of the source to the dest
		BlendSubtract	// Subtracts the RGB value of the source from the dest
	};

	struct SoftOpaqueBlender
	{
		inline Color operator() (Color source, Color dest) const
		{
			return source;
		}
	};

	struct SoftMergeBlender
	{
		inline Color operator() (Color source, Color dest) const
		{
				Color result;
				int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;
				int finalAlpha = sourceAlpha + ((255 - sourceAlpha) * dest[AlphaChannel]) / 255;
				sourceAlpha = (finalAlpha == 0) ? 0 : sourceAlpha * 255 / finalAlpha;

				result[RedChannel] = (source[RedChannel] * sourceAlpha + dest[RedChannel] * (255 - sourceAlpha)) / 255;
				result[GreenChannel] = (source[GreenChannel] * sourceAlpha + dest[GreenChannel] * (255 - sourceAlpha)) / 255;
				result[BlueChannel] = (source[BlueChannel] * sourceAlpha + dest[BlueChannel] * (255 - sourceAlpha)) / 255;
				result[AlphaChannel] = finalAlpha;

				return result;
		}
	};

	struct SoftPreserveBlender
	{
		inline Color operator() (Color source, Color dest) const
		{
				Color result;
				int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;

				result[RedChannel] = (sourceAlpha * (source[RedChannel] - dest[RedChannel])) / 255 + dest[RedChannel];
				result[GreenChannel] = (sourceAlpha * (source[GreenChannel] - dest[GreenChannel])) / 255 + dest[GreenChannel];
				result[BlueChannel] = (sourceAlpha * (source[BlueChannel] - dest[GreenChannel])) / 255 + dest[BlueChannel];
				result[AlphaChannel] = dest[AlphaChannel];
				return result;
		}
	};

	struct SoftAddBlender
	{
		inline Color operator() (Color source, Color dest) const
		{
				Color result;
				int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;

				result[RedChannel] = PLUM_MIN((sourceAlpha * source[RedChannel]) / 255 + dest[RedChannel], 255);
				result[GreenChannel] = PLUM_MIN((sourceAlpha * source[GreenChannel]) / 255 + dest[GreenChannel], 255);
				result[BlueChannel] = PLUM_MIN((sourceAlpha * source[BlueChannel]) / 255 + dest[BlueChannel], 255);
				result[AlphaChannel] = dest[AlphaChannel];
				return result;
		}
	};

	struct SoftSubtractBlender
	{
		inline Color operator() (Color source, Color dest) const
		{
				Color result;
				int sourceAlpha = source[AlphaChannel] * getOpacity() / 255;

				result[RedChannel] = PLUM_MAX((sourceAlpha * -source[RedChannel]) / 255 + dest[RedChannel], 0);
				result[GreenChannel] = PLUM_MAX((sourceAlpha * -source[GreenChannel]) / 255 + dest[GreenChannel], 0);
				result[BlueChannel] = PLUM_MAX((sourceAlpha * -source[BlueChannel]) / 255 + dest[BlueChannel], 0);
				result[AlphaChannel] = dest[AlphaChannel];
				return result;
		}
	};

	extern int _globalAlpha;
	extern BlendMode _blendMode;

	extern void (PLUM_CALLBACK *glBlendEquationEXT)(int);
	void PLUM_CALLBACK NoBlendExtension(int);

	BlendMode getBlendMode();
	void setBlendMode(BlendMode mode);
	int getOpacity();
	void setOpacity(int alpha);

	inline void useHardwareBlender(BlendMode mode)
	{
		glBlendEquationEXT(GL_FUNC_ADD_EXT);
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
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glBlendEquationEXT(GL_FUNC_REVERSE_SUBTRACT_EXT);
				break;
		}
	}

	inline void useHardwareColor(int r, int g, int b, int a) 
	{
		return glColor4ub(r, g, b, a * _globalAlpha / 255);
	}
}

#endif
