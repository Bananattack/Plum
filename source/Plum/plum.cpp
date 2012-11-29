#include "engine/log.h"
#include "engine/engine.h"
#include "script/script.h"

#include <cstdlib>
#include <stdexcept>

int main(int argc, char** argv)
{
    plum::clearLog();
    try
    {
        plum::Engine engine;
        plum::Script script(&engine);

    //    freopen("stdout.log", "w", stdout);
    //    freopen("stderr.log", "w", stderr);
        try
        {
            script.run("system.lua");
        }
        catch(std::runtime_error& e)
        {
            engine.quit(e.what());
        }
        engine.quit();
    }
    catch(plum::SystemExit& e)
    {
        std::exit(e.status());
    }

    return 0;
}
