#ifndef PLUM_GLFW_CONFIG_H
#define PLUM_GLFW_CONFIG_H

#define _GLFW_VERSION_FULL "3.0.0"

#ifdef _WIN32
#define _GLFW_USE_OPENGL
#define _GLFW_WIN32
#define _GLFW_WGL

#elif __APPLE__
#define _GLFW_USE_OPENGL
#define _GLFW_COCOA
#define _GLFW_NSGL

#elif __linux
#define _GLFW_USE_OPENGL
#define _GLFW_X11
#define _GLFW_GLX
#define _GLFW_HAS_GLXGETPROCADDRESS

#else
#error Platform not supported!

#endif
#endif
