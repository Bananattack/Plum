#include "engine/log.h"
#include "engine/engine.h"
#include "script/script.h"
#include "audio/audio.h"
#include "video/video.h"

#include <cstdlib>
#include <stdexcept>

int main(int argc, char** argv)
{
    plum::clearLog();
    try
    {
        plum::Config config("plum.cfg");
        auto silent = config.get<bool>("silent", false);
        auto xres = config.get<int>("xres", 320);
        auto yres = config.get<int>("yres", 240);
        auto windowed = config.get<bool>("windowed", true);

        plum::Engine engine;
        plum::Audio audio(engine, silent);
        plum::Video video(engine, xres, yres, windowed);
        try
        {
            plum::Script script(engine, audio, video);
            script.run("system.lua");
        }
        catch(std::runtime_error& e)
        {
            engine.quit(e.what());
        }
        engine.quit();
    }
    catch(const plum::SystemExit& e)
    {
        std::exit(e.status());
    }

    return 0;
}
