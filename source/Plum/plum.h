#pragma once

#if defined(WIN32) || defined(_WIN32)
#    define PLUM_WIN32
#elif defined(__linux__)
#    define PLUM_LINUX
#endif

#ifdef PLUM_WIN32
#define PLUM_CALLBACK __stdcall
#else
#define PLUM_CALLBACK
#endif

#include "common/common.h"
