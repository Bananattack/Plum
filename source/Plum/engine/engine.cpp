#include "log.h"
#include "engine.h"
#include "../audio/audio.h"
#include "../video/video.h"

#ifdef _WIN32
#define NOMINMAX
#include <SDL_syswm.h>
#endif

#include <SDL.h>

namespace plum
{
    Engine::Engine()
        : initialized(false), config("plum.cfg")
    {
        logFormat(">> Initializing...\n");

        auto xres = config.get<int>("xres", 320);
        auto yres = config.get<int>("yres", 240);
        auto windowed = config.get<bool>("windowed", true);

        logFormat("    (Settings: %dx%d resolution, %s mode)\n\n", xres, yres, windowed ? "windowed" : "fullscreen");

        logFormat("    Initializing SDL...");
        if(SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_NOPARACHUTE) != 0)
        {
            quit("Couldn't initialize SDL.\n");
        }

#ifdef _WIN32
        {
            SDL_SysWMinfo info;
            SDL_VERSION(&info.version);
            auto windowHandle = SDL_GetWMInfo(&info) ? info.window : nullptr;
            if(windowHandle)
            {
                SetClassLong(windowHandle, GCL_HICON, (long) LoadIcon(GetModuleHandle(0), "APPICON"));
            }
        }
#endif

        SDL_ShowCursor(config.get<bool>("hide_cursor", false) ? SDL_DISABLE : SDL_ENABLE);

        logFormat(" OK!\n");

        logFormat("    Initializing video engine...");
        setTitle("Plum");
        video = Video::create(xres, yres, windowed);
        logFormat(" OK!\n");

        logFormat("    Initializing sound engine...");
        audio = Audio::create(config.get<bool>("no_sound", false));
        logFormat(" OK!\n");

        destroyed = false;
        initialized = true;
        logFormat(">> Initialization complete!\n\n");
    }

    Engine::~Engine()
    {
        if(!initialized)
        {
            logFormat("\n>> Shutdown before program was fully initialized, probably means fatal errors. Uh oh!\n");
            return;
        }
        logFormat("\n>> Destroying...\n");

        logFormat("    Destroying sound engine...");
        delete audio;
        logFormat(" OK!\n");

        logFormat("    Destroying video engine...");
        delete video;
        logFormat(" OK!\n");

        logFormat("    Destroying SDL...");
        SDL_Quit();
        logFormat(" OK!\n");

        logFormat(">> Destroyed!\n");
    }

    void Engine::quit(std::string message) 
    {
        logFormat("\n>> Shutdown requested");
        if(message.length())
        {
            // If we're initialized enough, we can draw the error on-screen!
            if(initialized)
            {
            }

            logFormat(", with quit message:\n%s", message.c_str());
            fprintf(stderr, "%s", message.c_str());
#ifdef _WIN32
            SDL_SysWMinfo info;
            SDL_VERSION(&info.version);
            HWND hWnd = SDL_GetWMInfo(&info) ? info.window : HWND_DESKTOP;

            MessageBoxA(hWnd, message.c_str(), "Error", 0);
#endif
            throw SystemExit(1);
        }
        logFormat(".\n");
        throw SystemExit(0);
    }

    size_t Engine::addUpdateHook(const std::function<void()>& hook)
    {
        updateHooks.push_back(hook);
        return updateHooks.size() - 1;
    }

    void Engine::removeUpdateHook(size_t index)
    {
        if(index < 0 && index >= updateHooks.size())
        {
            throw std::out_of_range("attempt to remove invalid index");
        }
        else
        {
            updateHooks.erase(updateHooks.begin() + index);
        }
    }

    void Engine::handleMouseButtonEvent(SDL_MouseButtonEvent e)
    {
        bool state = (e.type == SDL_MOUSEBUTTONDOWN ? true : false);
        // do something with event.button;
        switch(e.button)
        {
            case SDL_BUTTON_LEFT:
                mouse.button[MOUSE_LEFT].setPressed(state);
                break;
            case SDL_BUTTON_MIDDLE:
                mouse.button[MOUSE_MIDDLE].setPressed(state);
                break;
            case SDL_BUTTON_RIGHT:
                mouse.button[MOUSE_RIGHT].setPressed(state);
                break;
            case SDL_BUTTON_WHEELUP:
                if(state)
                {
                    mouse.wheelPosition--;
                }
                break;
            case SDL_BUTTON_WHEELDOWN:
                if(state)
                {
                    mouse.wheelPosition++;
                }
                break;
        }
    }

    void Engine::poll()
    {
        SDL_Event event;
        //ControlMap::iterator it;
        while (!destroyed && SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                /*case SDL_VIDEORESIZE:
                    // I hate you SDL. Why do you nuke the GL context on resize events. You're a terrible person.
                    if(windowed)
                    {
                        adjustSize(event.resize.w, event.resize.h, windowed);
                    }
                    break;*/
                case SDL_MOUSEMOTION:
                    mouse.x = (double) event.motion.x * (double) video->getScreenWidth() / (double) video->getWindowWidth();
                    mouse.y = (double) event.motion.y * (double) video->getScreenHeight() / (double) video->getWindowHeight();
                    break;
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    handleMouseButtonEvent(event.button);
                    break;
                case SDL_KEYDOWN:
                    key[event.key.keysym.sym].setPressed(true);
                    break;
                case SDL_KEYUP:
                    // do something with event.key;
                    key[event.key.keysym.sym].setPressed(false);
                    break;
                case SDL_QUIT:
                    // Close the program
                    quit();
                    break;
                default:
                    break;
            }
        }
    }

    void Engine::refresh()
    {
        poll();
        SDL_GL_SwapBuffers();

        timer.processInput(key[KEY_TILDE], key[KEY_LSHIFT]);
        if(key[KEY_TILDE].isPressed())
        {
            audio->setMasterPitch(1.0 * TIMER_FAST_MULTIPLIER);
        }
        else if(key[KEY_LSHIFT].isPressed())
        {
            audio->setMasterPitch(1.0 / TIMER_SLOW_DIVISOR);
        }
        else
        {
            audio->setMasterPitch(1.0);
        }
        audio->update();
        timer.update();

        for(auto it = updateHooks.begin(), end = updateHooks.end(); it != end; ++it)
        {
            (*it)();
        }

        if((key[KEY_LALT].isPressed() || key[KEY_RALT].isPressed()) && (key[KEY_F4].isPressed() || key[KEY_X].isPressed()))
        {
            quit();
        }
        if(timer.gap == 0)
        {
            SDL_Delay(10);
        }
    }

    void Engine::setTitle(std::string title)
    {
        titlePrefix = title;
        SDL_WM_SetCaption(title.c_str(), title.c_str());
    }
}
