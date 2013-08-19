#include "core/audio.h"
#include "core/screen.h"
#include "core/config.h"
#include "core/engine.h"
#include "core/timer.h"
#include "core/input.h"
#include "script/script.h"

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#include <windows.h>

    int main(int argc, char** argv);
    int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
    {
        return main(__argc, __argv);
    }

    namespace
    {
        void redirect(bool console)
        {
            if(console && AllocConsole())
            {
                *stdout = *_fdopen(_open_osfhandle((intptr_t) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");
                *stderr = *_fdopen(_open_osfhandle((intptr_t) GetStdHandle(STD_ERROR_HANDLE), _O_TEXT), "w");
            }
            else
            {
                freopen("stdout.log", "w", stdout);
                freopen("stderr.log", "w", stderr);
            }
            setvbuf(stdout, nullptr, _IONBF, 0);
            setvbuf(stderr, nullptr, _IONBF, 0);
        }
    }

#else
    namespace
    {
        void redirect(bool console)
        {
        }
    }
#endif

int main(int argc, char** argv)
{
    try
    {
        plum::Config config("plum.cfg");
        auto silent = config.get<bool>("silent", false);
        auto console = config.get<bool>("console", false);

        redirect(console);

        plum::Engine engine;
        plum::Timer timer(engine);
        plum::Audio audio(engine, silent);

        auto hook = engine.addUpdateHook([&]() {
            if(timer.getSpeed() == plum::TimerSpeed::Fast)
            {
                audio.setPitch(1.0 * plum::Timer::FastForwardMultiplier);
            }
            else if(timer.getSpeed() == plum::TimerSpeed::Slow)
            {
                audio.setPitch(1.0 / plum::Timer::SlowMotionDivisor);
            }
            else
            {
                audio.setPitch(1.0);
            }
        });

        try
        {
            plum::Script script(argc, argv, engine, timer, audio);
            script.run("system.lua");
        }
        catch(const std::runtime_error& e)
        {
            engine.quit(e.what());
        }
    }
    catch(const plum::SystemExit& e)
    {
        if(e.status())
        {
            std::exit(e.status());
        }
    }

    return 0;
}