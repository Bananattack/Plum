#pragma once

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
#elif defined(__linux__)
#	define PLUM_LINUX
#endif

#ifdef PLUM_WIN32
#   define PLUM_CALLBACK __stdcall
#else
#   define PLUM_CALLBACK
#endif

#ifdef PLUM_WIN32
#	include <windows.h>
#	include <delayimp.h>
#	ifdef _DEBUG
#		define CRTDBG_MAP_ALLOC
#		include <stdlib.h>
#		include <crtdbg.h>
#	endif
#endif

// zlib's for compression stuff!
#include <zlib.h>
// corona's for image loading!
#include <corona.h>
// audiere's for audio!
#include <audiere.h>
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

#include "common/common.h"
#include "common/types.h"
#include "common/path.h"
#include "common/file.h"
#include "common/directory.h"
#include "common/config.h"
#include "common/base64.h"
#include "common/sha1.h"
#include "common/compression.h"

#include "video/color.h"
#include "video/blending.h"
#include "video/transform.h"
#include "video/canvas.h"
#include "video/image.h"
#include "video/video.h"

#include "audio/audio_audiere.h"

#include "common/animation.h"

#include "engine/input.h"
#include "engine/timer.h"
#include "engine/sprite.h" // REMOVEME later.
#include "engine/font.h"
#include "engine/tileset.h" // REMOVEME later.
#include "engine/spritesheet.h"
#include "engine/camera.h"
#include "engine/tilemap.h"

#include "script/script.h"
#include "engine/engine.h"
