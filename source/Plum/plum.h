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

#if defined(WIN32) || defined(_WIN32)
#	define PLUM_WIN32
#endif

#ifdef PLUM_WIN32
#   define PLUM_CALLBACK __stdcall
#else
#   define PLUM_CALLBACK
#endif

#ifdef PLUM_WIN32
#	include <windows.h>
#endif

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
#include <SDL_syswm.h>

extern "C"
{
	// Lua!
	#include <lua/lua.h>
	#include <lua/lualib.h>
	#include <lua/lauxlib.h>

	// zziplib's for zip files!
	#include <zzip/lib.h>
}

#include "common.h"
#include "pit.h"

#include "video/color.h"
#include "video/blending.h"
#include "video/image.h"
#include "video/texture.h"
#include "video/video.h"

#include "audio/audio.h"

#include "font.h"
#include "input.h"
#include "timer.h"
#include "animation.h"
#include "sprite.h"
#include "config.h"
#include "engine.h"
#include "script/script.h"

#endif
