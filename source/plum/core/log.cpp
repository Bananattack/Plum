#include <cstdio>
#include <cstdarg>

namespace plum
{
    namespace
    {
        const char* const LOG_FILE = "plum.log";
    }

    void clearLog()
    {
        std::fclose(std::fopen(LOG_FILE, "w"));
    }

    void logFormat(const char* format, ...)
    {
        std::FILE* f = std::fopen(LOG_FILE, "a");
        if(!f)
        {
            return;
        }
        std::va_list args;
        va_start(args, format);
        std::vfprintf(f, format, args);
        va_end(args);
        std::fclose(f);
    }
}