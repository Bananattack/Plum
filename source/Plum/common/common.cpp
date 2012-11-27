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

    std::string toLower(const std::string& s)
    {
        std::string t(s);

        for (uint i = 0; i < t.length(); i++)
        {
            if (t[i] >= 'A' && t[i] <= 'Z')
            {
                t[i] ^= 32;
            }
        }

        return t;
    }

    std::string toUpper(const std::string& s)
    {
        std::string t(s);

        for (uint i = 0; i < t.length(); i++)
        {
            if (t[i] >= 'a' && t[i] <= 'z')
            {
                t[i] ^= 32;
            }
        }

        return t;
    }
}