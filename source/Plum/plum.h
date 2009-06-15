#ifndef PLUM_H
#define PLUM_H
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <exception>
#include <algorithm>

// Mmm... broccoli pie.
const double PLUM_PI = 3.14159265358;

#if defined(WIN32) || defined(_WIN32)
#define PLUM_WIN32
#endif

#ifdef PLUM_WIN32
#   define PLUM_CALLBACK __stdcall
#else
#   define PLUM_CALLBACK
#endif

#define PLUM_ABS(a) (((a) < 0) ? -(a) : (a))
#define PLUM_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define PLUM_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define PLUM_SWAP(a, b)	{ \
					(a) = (a) - (b); \
					(b) = (a) + (b); \
					(a) = (b) - (a); \
				}

namespace Plum
{
	inline int stringToInteger(const std::string& s)
	{
		return atoi(s.c_str());
	}

	inline bool isStringNumeric(const std::string& s)
	{
		for (unsigned int i = 0; i < s.length(); i++)
		{
			if(!isdigit(s[i]))
			{
				return false;
			}
		}
		return true;
	}

	inline std::string integerToString(const int x)
	{
		std::ostringstream o;
		if (!(o << x)) return "";
		return o.str();
	}
}

#ifdef PLUM_WIN32
	#include <windows.h>
	// zlib's for compression stuff!
	#include "../zlib/zlib.h"
	// corona's for image loading!
	#include "../corona/corona.h"
	// fmod's for audio!
	#include "../fmod_header/fmod.hpp"
	#include "../fmod_header/fmod_errors.h"
	// SDL's for video and input stuff!
	#include "../sdl_header/SDL.h"
	#include "../sdl_header/SDL_opengl.h"
	#include "../sdl_header/SDL_syswm.h"
	// Lua!
	extern "C"
	{
		#include "../lua/lua.h"
		#include "../lua/lualib.h"
		#include "../lua/lauxlib.h"
	}
#else
	// zlib's for compression stuff!
	#include <zlib.h>
	// corona's for image loading!
	#include <corona.h>
	// fmod's for audio!
	#include <fmod.hpp>
	#include <fmod_errors.h>
	// SDL's for video and input stuff!
	#include <SDL.h>
	#include <SDL_opengl.h>
#endif

#include "color.h"
#include "blending.h"
#include "image.h"
#include "texture.h"
#include "font.h"
#include "video.h"
#include "input.h"
#include "timer.h"
#include "audio.h"
#include "formats.h"
#include "animation.h"
#include "sprite.h"
#include "engine.h"

#endif
