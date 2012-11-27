#include "../plum.h"

namespace plum
{
    const char* const LOG_FILE = "plum.log";

    void clearLog()
    {
        fclose(fopen(LOG_FILE, "wb"));
    }

    void logFormat(const char* format, ...)
    {
        FILE* f = fopen(LOG_FILE, "ab");
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