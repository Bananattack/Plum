//========================================================================
// GLFW - An OpenGL library
// Platform:    X11
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

#include <limits.h>
#include <stdlib.h>
#include <string.h>


//------------------------------------------------------------------------
// Display resolution
//------------------------------------------------------------------------

typedef struct
{
    int width;
    int height;
} _GLFWvidsize;


//========================================================================
// List available resolutions
//========================================================================

static _GLFWvidsize* getResolutions(int* found)
{
    int i, j;
    _GLFWvidsize* result = NULL;

    *found = 0;

    // Build array of available resolutions

    if (_glfwLibrary.X11.RandR.available)
    {
#if defined(_GLFW_HAS_XRANDR)
        XRRScreenConfiguration* sc;
        XRRScreenSize* sizes;

        sc = XRRGetScreenInfo(_glfwLibrary.X11.display, _glfwLibrary.X11.root);
        sizes = XRRConfigSizes(sc, found);

        result = (_GLFWvidsize*) malloc(sizeof(_GLFWvidsize) * *found);

        for (i = 0;  i < *found;  i++)
        {
            result[i].width  = sizes[i].width;
            result[i].height = sizes[i].height;
        }

        XRRFreeScreenConfigInfo(sc);
#endif /*_GLFW_HAS_XRANDR*/
    }
    else if (_glfwLibrary.X11.VidMode.available)
    {
#if defined(_GLFW_HAS_XF86VIDMODE)
        XF86VidModeModeInfo** modes;
        int modeCount;

        XF86VidModeGetAllModeLines(_glfwLibrary.X11.display,
                                   _glfwLibrary.X11.screen,
                                   &modeCount, &modes);

        result = (_GLFWvidsize*) malloc(sizeof(_GLFWvidsize) * modeCount);

        for (i = 0;  i < modeCount;  i++)
        {
            _GLFWvidsize size;
            size.width  = modes[i]->hdisplay;
            size.height = modes[i]->vdisplay;

            for (j = 0;  j < *found;  j++)
            {
                if (memcmp(result + j, &size, sizeof(_GLFWvidsize)) == 0)
                    break;
            }

            if (j < *found)
            {
                // This size is a duplicate, so skip it
                continue;
            }

            result[*found] = size;
            (*found)++;
        }

        XFree(modes);
#endif /*_GLFW_HAS_XF86VIDMODE*/
    }

    if (result == NULL)
    {
        *found = 1;
        result = (_GLFWvidsize*) malloc(sizeof(_GLFWvidsize));

        result[0].width = DisplayWidth(_glfwLibrary.X11.display,
                                       _glfwLibrary.X11.screen);
        result[0].height = DisplayHeight(_glfwLibrary.X11.display,
                                         _glfwLibrary.X11.screen);
    }

    return result;
}



//////////////////////////////////////////////////////////////////////////
//////                       GLFW internal API                      //////
//////////////////////////////////////////////////////////////////////////

//========================================================================
// Finds the video mode closest in size to the specified desired size
//========================================================================

int _glfwGetClosestVideoMode(int* width, int* height, int* rate)
{
    int i, match, bestmatch;

    if (_glfwLibrary.X11.RandR.available)
    {
#if defined(_GLFW_HAS_XRANDR)
        int sizecount, bestsize;
        int ratecount, bestrate;
        short* ratelist;
        XRRScreenConfiguration* sc;
        XRRScreenSize* sizelist;

        sc = XRRGetScreenInfo(_glfwLibrary.X11.display, _glfwLibrary.X11.root);

        sizelist = XRRConfigSizes(sc, &sizecount);

        // Find the best matching mode
        bestsize  = -1;
        bestmatch = INT_MAX;
        for (i = 0;  i < sizecount;  i++)
        {
            match = (*width - sizelist[i].width) *
                    (*width - sizelist[i].width) +
                    (*height - sizelist[i].height) *
                    (*height - sizelist[i].height);
            if (match < bestmatch)
            {
                bestmatch = match;
                bestsize  = i;
            }
        }

        if (bestsize != -1)
        {
            // Report width & height of best matching mode
            *width = sizelist[bestsize].width;
            *height = sizelist[bestsize].height;

            if (*rate > 0)
            {
                ratelist = XRRConfigRates(sc, bestsize, &ratecount);

                bestrate = -1;
                bestmatch = INT_MAX;
                for (i = 0;  i < ratecount;  i++)
                {
                    match = abs(ratelist[i] - *rate);
                    if (match < bestmatch)
                    {
                        bestmatch = match;
                        bestrate = ratelist[i];
                    }
                }

                if (bestrate != -1)
                    *rate = bestrate;
            }
        }

        XRRFreeScreenConfigInfo(sc);

        if (bestsize != -1)
            return bestsize;
#endif /*_GLFW_HAS_XRANDR*/
    }
    else if (_glfwLibrary.X11.VidMode.available)
    {
#if defined(_GLFW_HAS_XF86VIDMODE)
        XF86VidModeModeInfo** modelist;
        int bestmode, modecount;

        // Get a list of all available display modes
        XF86VidModeGetAllModeLines(_glfwLibrary.X11.display,
                                   _glfwLibrary.X11.screen,
                                   &modecount, &modelist);

        // Find the best matching mode
        bestmode  = -1;
        bestmatch = INT_MAX;
        for (i = 0;  i < modecount;  i++)
        {
            match = (*width - modelist[i]->hdisplay) *
                    (*width - modelist[i]->hdisplay) +
                    (*height - modelist[i]->vdisplay) *
                    (*height - modelist[i]->vdisplay);
            if (match < bestmatch)
            {
                bestmatch = match;
                bestmode  = i;
            }
        }

        if (bestmode != -1)
        {
            // Report width & height of best matching mode
            *width = modelist[bestmode]->hdisplay;
            *height = modelist[bestmode]->vdisplay;
        }

        XFree(modelist);

        if (bestmode != -1)
            return bestmode;
#endif /*_GLFW_HAS_XF86VIDMODE*/
    }

    // Default: Simply use the screen resolution
    *width = DisplayWidth(_glfwLibrary.X11.display, _glfwLibrary.X11.screen);
    *height = DisplayHeight(_glfwLibrary.X11.display, _glfwLibrary.X11.screen);

    return 0;
}


//========================================================================
// Change the current video mode
//========================================================================

void _glfwSetVideoModeMODE(int mode, int rate)
{
    if (_glfwLibrary.X11.RandR.available)
    {
#if defined(_GLFW_HAS_XRANDR)
        XRRScreenConfiguration* sc;
        Window root;

        root = _glfwLibrary.X11.root;
        sc   = XRRGetScreenInfo(_glfwLibrary.X11.display, root);

        // Remember old size and flag that we have changed the mode
        if (!_glfwLibrary.X11.FS.modeChanged)
        {
            _glfwLibrary.X11.FS.oldSizeID = XRRConfigCurrentConfiguration(sc, &_glfwLibrary.X11.FS.oldRotation);
            _glfwLibrary.X11.FS.oldWidth  = DisplayWidth(_glfwLibrary.X11.display,
                                                         _glfwLibrary.X11.screen);
            _glfwLibrary.X11.FS.oldHeight = DisplayHeight(_glfwLibrary.X11.display,
                                                          _glfwLibrary.X11.screen);

            _glfwLibrary.X11.FS.modeChanged = GL_TRUE;
        }

        if (rate > 0)
        {
            // Set desired configuration
            XRRSetScreenConfigAndRate(_glfwLibrary.X11.display,
                                      sc,
                                      root,
                                      mode,
                                      RR_Rotate_0,
                                      (short) rate,
                                      CurrentTime);
        }
        else
        {
            // Set desired configuration
            XRRSetScreenConfig(_glfwLibrary.X11.display,
                               sc,
                               root,
                               mode,
                               RR_Rotate_0,
                               CurrentTime);
        }

        XRRFreeScreenConfigInfo(sc);
#endif /*_GLFW_HAS_XRANDR*/
    }
    else if (_glfwLibrary.X11.VidMode.available)
    {
#if defined(_GLFW_HAS_XF86VIDMODE)
        XF86VidModeModeInfo **modelist;
        int modecount;

        // Get a list of all available display modes
        XF86VidModeGetAllModeLines(_glfwLibrary.X11.display,
                                   _glfwLibrary.X11.screen,
                                   &modecount, &modelist);

        // Unlock mode switch if necessary
        if (_glfwLibrary.X11.FS.modeChanged)
        {
            XF86VidModeLockModeSwitch(_glfwLibrary.X11.display,
                                      _glfwLibrary.X11.screen,
                                      0);
        }

        // Change the video mode to the desired mode
        XF86VidModeSwitchToMode(_glfwLibrary.X11.display,
                                _glfwLibrary.X11.screen,
                                modelist[mode]);

        // Set viewport to upper left corner (where our window will be)
        XF86VidModeSetViewPort(_glfwLibrary.X11.display,
                               _glfwLibrary.X11.screen,
                               0, 0);

        // Lock mode switch
        XF86VidModeLockModeSwitch(_glfwLibrary.X11.display,
                                  _glfwLibrary.X11.screen,
                                  1);

        // Remember old mode and flag that we have changed the mode
        if (!_glfwLibrary.X11.FS.modeChanged)
        {
            _glfwLibrary.X11.FS.oldMode = *modelist[0];
            _glfwLibrary.X11.FS.modeChanged = GL_TRUE;
        }

        XFree(modelist);
#endif /*_GLFW_HAS_XF86VIDMODE*/
    }
}


//========================================================================
// Change the current video mode
//========================================================================

void _glfwSetVideoMode(int* width, int* height, int* rate)
{
    int bestmode;

    // Find a best match mode
    bestmode = _glfwGetClosestVideoMode(width, height, rate);

    // Change mode
    _glfwSetVideoModeMODE(bestmode, *rate);
}


//========================================================================
// Restore the previously saved (original) video mode
//========================================================================

void _glfwRestoreVideoMode(void)
{
    if (_glfwLibrary.X11.FS.modeChanged)
    {
        if (_glfwLibrary.X11.RandR.available)
        {
#if defined(_GLFW_HAS_XRANDR)
            XRRScreenConfiguration* sc;

            if (_glfwLibrary.X11.RandR.available)
            {
                sc = XRRGetScreenInfo(_glfwLibrary.X11.display,
                                      _glfwLibrary.X11.root);

                XRRSetScreenConfig(_glfwLibrary.X11.display,
                                   sc,
                                   _glfwLibrary.X11.root,
                                   _glfwLibrary.X11.FS.oldSizeID,
                                   _glfwLibrary.X11.FS.oldRotation,
                                   CurrentTime);

                XRRFreeScreenConfigInfo(sc);
            }
#endif /*_GLFW_HAS_XRANDR*/
        }
        else if (_glfwLibrary.X11.VidMode.available)
        {
#if defined(_GLFW_HAS_XF86VIDMODE)
            // Unlock mode switch
            XF86VidModeLockModeSwitch(_glfwLibrary.X11.display,
                                      _glfwLibrary.X11.screen,
                                      0);

            // Change the video mode back to the old mode
            XF86VidModeSwitchToMode(_glfwLibrary.X11.display,
                                    _glfwLibrary.X11.screen,
                                    &_glfwLibrary.X11.FS.oldMode);
#endif /*_GLFW_HAS_XF86VIDMODE*/
        }

        _glfwLibrary.X11.FS.modeChanged = GL_FALSE;
    }
}


//////////////////////////////////////////////////////////////////////////
//////                       GLFW platform API                      //////
//////////////////////////////////////////////////////////////////////////


//========================================================================
// List available video modes
//========================================================================

GLFWvidmode* _glfwPlatformGetVideoModes(int* found)
{
    XVisualInfo* visuals;
    XVisualInfo dummy;
    int i, j, visualCount, sizeCount, rgbCount;
    int* rgbs;
    _GLFWvidsize* sizes;
    GLFWvidmode* result;

    visuals = XGetVisualInfo(_glfwLibrary.X11.display, 0, &dummy, &visualCount);
    if (visuals == NULL)
    {
        _glfwSetError(GLFW_PLATFORM_ERROR,
                      "X11: Failed to retrieve the available visuals");
        return 0;
    }

    rgbs = (int*) malloc(sizeof(int) * visualCount);
    rgbCount = 0;

#if defined(_GLFW_GLX)
    for (i = 0;  i < visualCount;  i++)
    {
        int gl, rgba, rgb, r, g, b;

        glXGetConfig(_glfwLibrary.X11.display, &visuals[i], GLX_USE_GL, &gl);
        glXGetConfig(_glfwLibrary.X11.display, &visuals[i], GLX_RGBA, &rgba);

        if (!gl || !rgba)
        {
            // The visual lacks OpenGL or true color, so skip it
            continue;
        }

        // Convert to RGB channel depths and encode
        _glfwSplitBPP(visuals[i].depth, &r, &g, &b);
        rgb = (r << 16) | (g << 8) | b;

        for (j = 0;  j < rgbCount;  j++)
        {
            if (rgbs[j] == rgb)
                break;
        }

        if (j < rgbCount)
        {
            // This channel depth is a duplicate, so skip it
            continue;
        }

        rgbs[rgbCount] = rgb;
        rgbCount++;
    }

    XFree(visuals);
#endif

    // Build all permutations of channel depths and resolutions

    sizes = getResolutions(&sizeCount);

    result = (GLFWvidmode*) malloc(sizeof(GLFWvidmode) * rgbCount * sizeCount);
    *found = 0;

    for (i = 0;  i < rgbCount;  i++)
    {
        for (j = 0;  j < sizeCount;  j++)
        {
            result[*found].width     = sizes[j].width;
            result[*found].height    = sizes[j].height;
            result[*found].redBits   = (rgbs[i] >> 16) & 255;
            result[*found].greenBits = (rgbs[i] >> 8) & 255;
            result[*found].blueBits  = rgbs[i] & 255;

            (*found)++;
        }
    }

    free(sizes);
    free(rgbs);

    return result;
}


//========================================================================
// Get the desktop video mode
//========================================================================

void _glfwPlatformGetDesktopMode(GLFWvidmode* mode)
{
    int bpp;

    // Get and split display depth
    bpp = DefaultDepth(_glfwLibrary.X11.display, _glfwLibrary.X11.screen);
    _glfwSplitBPP(bpp, &mode->redBits, &mode->greenBits, &mode->blueBits);

    if (_glfwLibrary.X11.FS.modeChanged)
    {
        if (_glfwLibrary.X11.RandR.available)
        {
#if defined(_GLFW_HAS_XRANDR)
            mode->width  = _glfwLibrary.X11.FS.oldWidth;
            mode->height = _glfwLibrary.X11.FS.oldHeight;
#endif /*_GLFW_HAS_XRANDR*/
        }
        else if (_glfwLibrary.X11.VidMode.available)
        {
#if defined(_GLFW_HAS_XF86VIDMODE)
            mode->width  = _glfwLibrary.X11.FS.oldMode.hdisplay;
            mode->height = _glfwLibrary.X11.FS.oldMode.vdisplay;
#endif /*_GLFW_HAS_XF86VIDMODE*/
        }
    }
    else
    {
        mode->width = DisplayWidth(_glfwLibrary.X11.display,
                                   _glfwLibrary.X11.screen);
        mode->height = DisplayHeight(_glfwLibrary.X11.display,
                                     _glfwLibrary.X11.screen);
    }
}

