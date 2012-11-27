#include "plum.h"

#include <cstdlib>
#include <stdexcept>

int main(int argc, char** argv)
{
    try
    {
        plum::Engine engine;
        plum::Script script(engine);

        plum::clearLog();
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
