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

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>


//========================================================================
// Parses the client API version string and extracts the version number
//========================================================================

static GLboolean parseGLVersion(int* api, int* major, int* minor, int* rev)
{
    int i, _api = GLFW_OPENGL_API, _major, _minor = 0, _rev = 0;
    const char* version;
    const char* prefixes[] =
    {
        "OpenGL ES-CM ",
        "OpenGL ES-CL ",
        "OpenGL ES ",
        NULL
    };

    version = (const char*) glGetString(GL_VERSION);
    if (!version)
    {
        _glfwSetError(GLFW_PLATFORM_ERROR, "Failed to retrieve version string");
        return GL_FALSE;
    }

    for (i = 0;  prefixes[i];  i++)
    {
        const size_t length = strlen(prefixes[i]);

        if (strncmp(version, prefixes[i], length) == 0)
        {
            version += length;
            _api = GLFW_OPENGL_ES_API;
            break;
        }
    }

    if (!sscanf(version, "%d.%d.%d", &_major, &_minor, &_rev))
    {
        _glfwSetError(GLFW_PLATFORM_ERROR, "No version found in version string");
        return GL_FALSE;
    }

    *api = _api;
    *major = _major;
    *minor = _minor;
    *rev = _rev;

    return GL_TRUE;
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

//========================================================================
// Return the available framebuffer config closest to the desired values
// This is based on the manual GLX Visual selection from 2.6
//========================================================================

const _GLFWfbconfig* _glfwChooseFBConfig(const _GLFWfbconfig* desired,
                                         const _GLFWfbconfig* alternatives,
                                         unsigned int count)
{
    unsigned int i;
    unsigned int missing, leastMissing = UINT_MAX;
    unsigned int colorDiff, leastColorDiff = UINT_MAX;
    unsigned int extraDiff, leastExtraDiff = UINT_MAX;
    const _GLFWfbconfig* current;
    const _GLFWfbconfig* closest = NULL;

    for (i = 0;  i < count;  i++)
    {
        current = alternatives + i;

        if (desired->stereo > 0 && current->stereo == 0)
        {
            // Stereo is a hard constraint
            continue;
        }

        // Count number of missing buffers
        {
            missing = 0;

            if (desired->alphaBits > 0 && current->alphaBits == 0)
                missing++;

            if (desired->depthBits > 0 && current->depthBits == 0)
                missing++;

            if (desired->stencilBits > 0 && current->stencilBits == 0)
                missing++;

            if (desired->auxBuffers > 0 && current->auxBuffers < desired->auxBuffers)
                missing += desired->auxBuffers - current->auxBuffers;

            if (desired->samples > 0 && current->samples == 0)
            {
                // Technically, several multisampling buffers could be
                // involved, but that's a lower level implementation detail and
                // not important to us here, so we count them as one
                missing++;
            }
        }

        // These polynomials make many small channel size differences matter
        // less than one large channel size difference

        // Calculate color channel size difference value
        {
            colorDiff = 0;

            if (desired->redBits > 0)
            {
                colorDiff += (desired->redBits - current->redBits) *
                             (desired->redBits - current->redBits);
            }

            if (desired->greenBits > 0)
            {
                colorDiff += (desired->greenBits - current->greenBits) *
                             (desired->greenBits - current->greenBits);
            }

            if (desired->blueBits > 0)
            {
                colorDiff += (desired->blueBits - current->blueBits) *
                             (desired->blueBits - current->blueBits);
            }
        }

        // Calculate non-color channel size difference value
        {
            extraDiff = 0;

            if (desired->alphaBits > 0)
            {
                extraDiff += (desired->alphaBits - current->alphaBits) *
                             (desired->alphaBits - current->alphaBits);
            }

            if (desired->depthBits > 0)
            {
                extraDiff += (desired->depthBits - current->depthBits) *
                             (desired->depthBits - current->depthBits);
            }

            if (desired->stencilBits > 0)
            {
                extraDiff += (desired->stencilBits - current->stencilBits) *
                             (desired->stencilBits - current->stencilBits);
            }

            if (desired->accumRedBits > 0)
            {
                extraDiff += (desired->accumRedBits - current->accumRedBits) *
                             (desired->accumRedBits - current->accumRedBits);
            }

            if (desired->accumGreenBits > 0)
            {
                extraDiff += (desired->accumGreenBits - current->accumGreenBits) *
                             (desired->accumGreenBits - current->accumGreenBits);
            }

            if (desired->accumBlueBits > 0)
            {
                extraDiff += (desired->accumBlueBits - current->accumBlueBits) *
                             (desired->accumBlueBits - current->accumBlueBits);
            }

            if (desired->accumAlphaBits > 0)
            {
                extraDiff += (desired->accumAlphaBits - current->accumAlphaBits) *
                             (desired->accumAlphaBits - current->accumAlphaBits);
            }

            if (desired->samples > 0)
            {
                extraDiff += (desired->samples - current->samples) *
                             (desired->samples - current->samples);
            }

            if (desired->sRGB)
            {
                if (!current->sRGB)
                    extraDiff++;
            }
        }

        // Figure out if the current one is better than the best one found so far
        // Least number of missing buffers is the most important heuristic,
        // then color buffer size match and lastly size match for other buffers

        if (missing < leastMissing)
            closest = current;
        else if (missing == leastMissing)
        {
            if ((colorDiff < leastColorDiff) ||
                (colorDiff == leastColorDiff && extraDiff < leastExtraDiff))
            {
                closest = current;
            }
        }

        if (current == closest)
        {
            leastMissing = missing;
            leastColorDiff = colorDiff;
            leastExtraDiff = extraDiff;
        }
    }

    return closest;
}


//========================================================================
// Checks whether the client API part of the window config is sane
// It blames glfwOpenWindow because that's the only caller
//========================================================================

GLboolean _glfwIsValidContextConfig(_GLFWwndconfig* wndconfig)
{
    if (wndconfig->clientAPI != GLFW_OPENGL_API &&
        wndconfig->clientAPI != GLFW_OPENGL_ES_API)
    {
        _glfwSetError(GLFW_INVALID_ENUM,
                      "glfwCreateWindow: Invalid client API requested");
        return GL_FALSE;
    }

    if (wndconfig->clientAPI == GLFW_OPENGL_API)
    {
        if (wndconfig->glMajor < 1 || wndconfig->glMinor < 0)
        {
            // OpenGL 1.0 is the smallest valid version
            _glfwSetError(GLFW_INVALID_VALUE,
                          "glfwCreateWindow: Invalid OpenGL version requested");
            return GL_FALSE;
        }
        if (wndconfig->glMajor == 1 && wndconfig->glMinor > 5)
        {
            // OpenGL 1.x series ended with version 1.5
            _glfwSetError(GLFW_INVALID_VALUE,
                          "glfwCreateWindow: Invalid OpenGL version requested");
            return GL_FALSE;
        }
        else if (wndconfig->glMajor == 2 && wndconfig->glMinor > 1)
        {
            // OpenGL 2.x series ended with version 2.1
            _glfwSetError(GLFW_INVALID_VALUE,
                          "glfwCreateWindow: Invalid OpenGL version requested");
            return GL_FALSE;
        }
        else if (wndconfig->glMajor == 3 && wndconfig->glMinor > 3)
        {
            // OpenGL 3.x series ended with version 3.3
            _glfwSetError(GLFW_INVALID_VALUE,
                          "glfwCreateWindow: Invalid OpenGL version requested");
            return GL_FALSE;
        }
        else
        {
            // For now, let everything else through
        }

        if (wndconfig->glProfile)
        {
            if (wndconfig->glProfile != GLFW_OPENGL_CORE_PROFILE &&
                wndconfig->glProfile != GLFW_OPENGL_COMPAT_PROFILE)
            {
                _glfwSetError(GLFW_INVALID_ENUM,
                              "glfwCreateWindow: Invalid OpenGL profile requested");
                return GL_FALSE;
            }

            if (wndconfig->glMajor < 3 ||
                (wndconfig->glMajor == 3 && wndconfig->glMinor < 2))
            {
                // Desktop OpenGL context profiles are only defined for version 3.2
                // and above

                _glfwSetError(GLFW_INVALID_VALUE,
                              "glfwCreateWindow: Context profiles only exist for "
                              "OpenGL version 3.2 and above");
                return GL_FALSE;
            }
        }

        if (wndconfig->glForward && wndconfig->glMajor < 3)
        {
            // Forward-compatible contexts are only defined for OpenGL version 3.0 and above
            _glfwSetError(GLFW_INVALID_VALUE,
                          "glfwCreateWindow: Forward compatibility only exist "
                          "for OpenGL version 3.0 and above");
            return GL_FALSE;
        }
    }
    else if (wndconfig->clientAPI == GLFW_OPENGL_ES_API)
    {
        if (wndconfig->glMajor < 1 || wndconfig->glMinor < 0)
        {
            // OpenGL ES 1.0 is the smallest valid version
            _glfwSetError(GLFW_INVALID_VALUE,
                          "glfwCreateWindow: Invalid OpenGL ES version requested");
            return GL_FALSE;
        }
        if (wndconfig->glMajor == 1 && wndconfig->glMinor > 1)
        {
            // OpenGL ES 1.x series ended with version 1.1
            _glfwSetError(GLFW_INVALID_VALUE,
                          "glfwCreateWindow: Invalid OpenGL ES version requested");
            return GL_FALSE;
        }
        else
        {
            // For now, let everything else through
        }

        if (wndconfig->glProfile)
        {
            // OpenGL ES does not support profiles
            _glfwSetError(GLFW_INVALID_VALUE,
                          "glfwCreateWindow: Context profiles are not supported "
                          "by OpenGL ES");
            return GL_FALSE;
        }

        if (wndconfig->glForward)
        {
            // OpenGL ES does not support forward-compatibility
            _glfwSetError(GLFW_INVALID_VALUE,
                          "glfwCreateWindow: Forward compatibility is not "
                          "supported by OpenGL ES");
            return GL_FALSE;
        }
    }

    if (wndconfig->glRobustness)
    {
        if (wndconfig->glRobustness != GLFW_OPENGL_NO_RESET_NOTIFICATION &&
            wndconfig->glRobustness != GLFW_OPENGL_LOSE_CONTEXT_ON_RESET)
        {
            _glfwSetError(GLFW_INVALID_VALUE,
                          "glfwCreateWindow: Invalid OpenGL robustness mode "
                          "requested");
            return GL_FALSE;
        }
    }

    return GL_TRUE;
}


//========================================================================
// Reads back context properties
// It blames glfwCreateWindow because that's the only caller
//========================================================================

GLboolean _glfwRefreshContextParams(void)
{
    _GLFWwindow* window = _glfwPlatformGetCurrentContext();

    if (!parseGLVersion(&window->clientAPI,
                        &window->glMajor,
                        &window->glMinor,
                        &window->glRevision))
    {
        return GL_FALSE;
    }

    if (window->glMajor > 2)
    {
        // OpenGL 3.0+ uses a different function for extension string retrieval
        // We cache it here instead of in glfwExtensionSupported mostly to alert
        // users as early as possible that their build may be broken

        window->GetStringi = (PFNGLGETSTRINGIPROC) glfwGetProcAddress("glGetStringi");
        if (!window->GetStringi)
        {
            _glfwSetError(GLFW_PLATFORM_ERROR,
                          "glfwCreateWindow: Entry point retrieval is broken");
            return GL_FALSE;
        }
    }

    // Read back forward-compatibility flag
    {
      window->glForward = GL_FALSE;

      if (window->clientAPI == GLFW_OPENGL_API && window->glMajor >= 3)
      {
          GLint flags;
          glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

          if (flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
              window->glForward = GL_TRUE;
          if (flags & 0)
              window->glDebug = GL_TRUE;
      }
    }

    // Read back OpenGL context profile
    {
      window->glProfile = 0;

      if (window->glMajor > 3 || (window->glMajor == 3 && window->glMinor >= 2))
      {
          GLint mask;
          glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &mask);

          if (mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
              window->glProfile = GLFW_OPENGL_COMPAT_PROFILE;
          else if (mask & GL_CONTEXT_CORE_PROFILE_BIT)
              window->glProfile = GLFW_OPENGL_CORE_PROFILE;
      }
    }

    return GL_TRUE;
}


//========================================================================
// Checks whether the current context fulfils the specified requirements
// It blames glfwCreateWindow because that's the only caller
//========================================================================

GLboolean _glfwIsValidContext(_GLFWwndconfig* wndconfig)
{
    _GLFWwindow* window = _glfwPlatformGetCurrentContext();

    if (window->glMajor < wndconfig->glMajor ||
        (window->glMajor == wndconfig->glMajor &&
         window->glMinor < wndconfig->glMinor))
    {
        // The desired OpenGL version is greater than the actual version
        // This only happens if the machine lacks {GLX|WGL}_ARB_create_context
        // /and/ the user has requested an OpenGL version greater than 1.0

        // For API consistency, we emulate the behavior of the
        // {GLX|WGL}_ARB_create_context extension and fail here

        _glfwSetError(GLFW_VERSION_UNAVAILABLE,
                      "glfwCreateWindow: The requested OpenGL version is not available");
        return GL_FALSE;
    }

    return GL_TRUE;
}


//========================================================================
// Check if a string can be found in a client API extension string
//========================================================================

int _glfwStringInExtensionString(const char* string,
                                 const GLubyte* extensions)
{
    const GLubyte* start;
    GLubyte* where;
    GLubyte* terminator;

    // It takes a bit of care to be fool-proof about parsing the
    // OpenGL extensions string. Don't be fooled by sub-strings,
    // etc.
    start = extensions;
    for (;;)
    {
        where = (GLubyte*) strstr((const char*) start, string);
        if (!where)
            return GL_FALSE;

        terminator = where + strlen(string);
        if (where == start || *(where - 1) == ' ')
        {
            if (*terminator == ' ' || *terminator == '\0')
                break;
        }

        start = terminator;
    }

    return GL_TRUE;
}


//////////////////////////////////////////////////////////////////////////
//////                        GLFW public API                       //////
//////////////////////////////////////////////////////////////////////////

//========================================================================
// Make the context associated with the specified window current
//========================================================================

GLFWAPI void glfwMakeContextCurrent(GLFWwindow handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;

    if (!_glfwInitialized)
    {
        _glfwSetError(GLFW_NOT_INITIALIZED, NULL);
        return;
    }

    if (_glfwPlatformGetCurrentContext() == window)
        return;

    _glfwPlatformMakeContextCurrent(window);
}


//========================================================================
// Returns the window whose context is current
//========================================================================

GLFWAPI GLFWwindow glfwGetCurrentContext(void)
{
    if (!_glfwInitialized)
    {
        _glfwSetError(GLFW_NOT_INITIALIZED, NULL);
        return NULL;
    }

    return _glfwPlatformGetCurrentContext();
}


//========================================================================
// Swap buffers (double-buffering)
//========================================================================

GLFWAPI void glfwSwapBuffers(GLFWwindow handle)
{
    _GLFWwindow* window = (_GLFWwindow*) handle;

    if (!_glfwInitialized)
    {
        _glfwSetError(GLFW_NOT_INITIALIZED, NULL);
        return;
    }

    _glfwPlatformSwapBuffers(window);
}


//========================================================================
// Set double buffering swap interval (0 = vsync off)
//========================================================================

GLFWAPI void glfwSwapInterval(int interval)
{
    if (!_glfwInitialized)
    {
        _glfwSetError(GLFW_NOT_INITIALIZED, NULL);
        return;
    }

    if (!_glfwPlatformGetCurrentContext())
    {
        _glfwSetError(GLFW_NO_CURRENT_CONTEXT, NULL);
        return;
    }

    _glfwPlatformSwapInterval(interval);
}


//========================================================================
// Check if a client API extension is available at runtime
//========================================================================

GLFWAPI int glfwExtensionSupported(const char* extension)
{
    const GLubyte* extensions;
    _GLFWwindow* window;

    if (!_glfwInitialized)
    {
        _glfwSetError(GLFW_NOT_INITIALIZED, NULL);
        return GL_FALSE;
    }

    window = _glfwPlatformGetCurrentContext();
    if (!window)
    {
        _glfwSetError(GLFW_NO_CURRENT_CONTEXT, NULL);
        return GL_FALSE;
    }

    if (extension == NULL || *extension == '\0')
    {
        _glfwSetError(GLFW_INVALID_VALUE, NULL);
        return GL_FALSE;
    }

    if (window->glMajor < 3)
    {
        // Check if extension is in the old style OpenGL extensions string

        extensions = glGetString(GL_EXTENSIONS);
        if (extensions != NULL)
        {
            if (_glfwStringInExtensionString(extension, extensions))
                return GL_TRUE;
        }
    }
    else
    {
        int i;
        GLint count;

        // Check if extension is in the modern OpenGL extensions string list

        glGetIntegerv(GL_NUM_EXTENSIONS, &count);

        for (i = 0;  i < count;  i++)
        {
             if (strcmp((const char*) window->GetStringi(GL_EXTENSIONS, i),
                         extension) == 0)
             {
                 return GL_TRUE;
             }
        }
    }

    // Check if extension is in the platform-specific string
    return _glfwPlatformExtensionSupported(extension);
}


//========================================================================
// Get the function pointer to a client API function
// This can be used to get access to client API extension functions
//========================================================================

GLFWAPI GLFWglproc glfwGetProcAddress(const char* procname)
{
    if (!_glfwInitialized)
    {
        _glfwSetError(GLFW_NOT_INITIALIZED, NULL);
        return NULL;
    }

    if (!_glfwPlatformGetCurrentContext())
    {
        _glfwSetError(GLFW_NO_CURRENT_CONTEXT, NULL);
        return NULL;
    }

    return _glfwPlatformGetProcAddress(procname);
}

