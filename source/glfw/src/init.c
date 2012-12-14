//========================================================================
// GLFW - An OpenGL library
// Platform:    Any
// API version: 3.0
// WWW:         http://www.glfw.org/
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2010 Camilla Berglund <elmindreda@elmindreda.org>
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

#include "internal.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>


//------------------------------------------------------------------------
// Flag indicating whether GLFW has been successfully initialized
//------------------------------------------------------------------------
GLboolean _glfwInitialized = GL_FALSE;


//------------------------------------------------------------------------
// All shared and API-specific global data protected by _glfwInitialized
// This should only be touched after a call to glfwInit that has not been
// followed by a call to glfwTerminate
//------------------------------------------------------------------------
_GLFWlibrary _glfwLibrary;


//------------------------------------------------------------------------
// The current GLFW error code
// This is outside of _glfwLibrary so it can be initialized and usable
// before glfwInit is called, which lets that function report errors
// TODO: Make this thread-local
//------------------------------------------------------------------------
static int _glfwError = GLFW_NO_ERROR;


//------------------------------------------------------------------------
// The current error callback
// This is outside of _glfwLibrary so it can be initialized and usable
// before glfwInit is called, which lets that function report errors
//------------------------------------------------------------------------
static GLFWerrorfun _glfwErrorCallback = NULL;


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

//========================================================================
// Sets the current error value
//========================================================================

void _glfwSetError(int error, const char* format, ...)
{
    if (_glfwErrorCallback)
    {
        char buffer[16384];
        const char* description;

        if (format)
        {
            int count;
            va_list vl;

            va_start(vl, format);
            count = vsnprintf(buffer, sizeof(buffer), format, vl);
            va_end(vl);

            if (count < 0)
                buffer[sizeof(buffer) - 1] = '\0';

            description = buffer;
        }
        else
            description = glfwErrorString(error);

        _glfwErrorCallback(error, description);
    }
    else
        _glfwError = error;
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW public API                       //////
//////////////////////////////////////////////////////////////////////////

//========================================================================
// Initialize various GLFW state
//========================================================================

GLFWAPI int glfwInit(void)
{
    if (_glfwInitialized)
        return GL_TRUE;

    memset(&_glfwLibrary, 0, sizeof(_glfwLibrary));

    if (!_glfwPlatformInit())
    {
        _glfwPlatformTerminate();
        return GL_FALSE;
    }

    _glfwInitialized = GL_TRUE;

    // Not all window hints have zero as their default value
    glfwDefaultWindowHints();

    return GL_TRUE;
}


//========================================================================
// Close window and shut down library
//========================================================================

GLFWAPI void glfwTerminate(void)
{
    if (!_glfwInitialized)
        return;

    // Close all remaining windows
    while (_glfwLibrary.windowListHead)
        glfwDestroyWindow(_glfwLibrary.windowListHead);

    if (!_glfwPlatformTerminate())
        return;

    if (_glfwLibrary.modes)
        free(_glfwLibrary.modes);

    _glfwInitialized = GL_FALSE;
}


//========================================================================
// Get GLFW version
// This function may be called without GLFW having been initialized
//========================================================================

GLFWAPI void glfwGetVersion(int* major, int* minor, int* rev)
{
    if (major != NULL)
        *major = GLFW_VERSION_MAJOR;

    if (minor != NULL)
        *minor = GLFW_VERSION_MINOR;

    if (rev != NULL)
        *rev = GLFW_VERSION_REVISION;
}


//========================================================================
// Get the GLFW version string
// This function may be called without GLFW having been initialized
//========================================================================

GLFWAPI const char* glfwGetVersionString(void)
{
    return _glfwPlatformGetVersionString();
}


//========================================================================
// Returns the current error value
// This function may be called without GLFW having been initialized
//========================================================================

GLFWAPI int glfwGetError(void)
{
    int error = _glfwError;
    _glfwError = GLFW_NO_ERROR;
    return error;
}


//========================================================================
// Returns a string representation of the specified error value
// This function may be called without GLFW having been initialized
//========================================================================

GLFWAPI const char* glfwErrorString(int error)
{
    switch (error)
    {
        case GLFW_NO_ERROR:
            return "No error";
        case GLFW_NOT_INITIALIZED:
            return "The GLFW library is not initialized";
        case GLFW_NO_CURRENT_CONTEXT:
            return "There is no current context";
        case GLFW_INVALID_ENUM:
            return "Invalid argument for enum parameter";
        case GLFW_INVALID_VALUE:
            return "Invalid value for parameter";
        case GLFW_OUT_OF_MEMORY:
            return "Out of memory";
        case GLFW_API_UNAVAILABLE:
            return "The requested client API is unavailable";
        case GLFW_VERSION_UNAVAILABLE:
            return "The requested client API version is unavailable";
        case GLFW_PLATFORM_ERROR:
            return "A platform-specific error occurred";
        case GLFW_FORMAT_UNAVAILABLE:
            return "The requested format is unavailable";
    }

    return "ERROR: UNKNOWN ERROR TOKEN PASSED TO glfwErrorString";
}


//========================================================================
// Sets the callback function for GLFW errors
// This function may be called without GLFW having been initialized
//========================================================================

GLFWAPI void glfwSetErrorCallback(GLFWerrorfun cbfun)
{
    _glfwErrorCallback = cbfun;
}

