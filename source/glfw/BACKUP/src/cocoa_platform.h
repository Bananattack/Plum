//========================================================================
// GLFW - An OpenGL library
// Platform:    Cocoa
// API Version: 3.0
// WWW:         http://www.glfw.org/
//------------------------------------------------------------------------
// Copyright (c) 2009-2010 Camilla Berglund <elmindreda@elmindreda.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#ifndef _cocoa_platform_h_
#define _cocoa_platform_h_


#include <stdint.h>

#if defined(__OBJC__)
#import <Cocoa/Cocoa.h>
#else
#include <ApplicationServices/ApplicationServices.h>
typedef void* id;
#endif

#if defined(_GLFW_NSGL)
 #include "nsgl_platform.h"
#else
 #error "No supported context creation API selected"
#endif

#define _GLFW_PLATFORM_WINDOW_STATE         _GLFWwindowNS  NS
#define _GLFW_PLATFORM_LIBRARY_WINDOW_STATE _GLFWlibraryNS NS


//========================================================================
// GLFW platform specific types
//========================================================================

//------------------------------------------------------------------------
// Pointer length integer
//------------------------------------------------------------------------
typedef intptr_t GLFWintptr;


//------------------------------------------------------------------------
// Platform-specific window structure
//------------------------------------------------------------------------
typedef struct _GLFWwindowNS
{
    id           object;
    id	         delegate;
    id           view;
    unsigned int modifierFlags;
} _GLFWwindowNS;


//------------------------------------------------------------------------
// Platform-specific library global data for Cocoa
//------------------------------------------------------------------------
typedef struct _GLFWlibraryNS
{
    struct {
        double base;
        double resolution;
    } timer;

    CGDisplayModeRef desktopMode;
    CGEventSourceRef eventSource;
    id               delegate;
    id               autoreleasePool;

    char*            clipboardString;
} _GLFWlibraryNS;


//========================================================================
// Prototypes for platform specific internal functions
//========================================================================

// Time
void _glfwInitTimer(void);

// Joystick input
void _glfwInitJoysticks(void);
void _glfwTerminateJoysticks(void);

// Fullscreen
GLboolean _glfwSetVideoMode(int* width, int* height, int* bpp, int* refreshRate);
void _glfwRestoreVideoMode(void);

// OpenGL support
int _glfwInitOpenGL(void);
void _glfwTerminateOpenGL(void);

#endif // _cocoa_platform_h_
