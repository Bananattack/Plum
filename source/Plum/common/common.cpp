#include <cstdio>
#include <cstdarg>

namespace plum
{
    const char* const LOG_FILE = "plum.log";

    void clearLog()
    {
        fclose(fopen(LOG_FILE, "w"));
    }

    void logFormat(const char* format, ...)
    {
        FILE* f = fopen(LOG_FILE, "a");
        if(!f)
        {
            return;
        }
        va_list args;
        va_start(args, format);
        vfprintf(f, format, args);
        va_end(args);
        fclose(f);
    }
}