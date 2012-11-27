#include "../plum.h"

namespace plum
{    
    // Path helpers.
    // Lifted from ika shamelessly
    namespace Path
    {
        // ifdef and blah blah for platform independance
        #ifdef PLUM_WIN32
            const char* delimiters = "\\/";
        #else // assume Unix
            const char* delimiters = "/";
        #endif

        std::string getDirectory(const std::string& s)
        {
            uint p = s.find_last_of(delimiters);
            if (p == std::string::npos)
            {
                return "";
            }

            return s.substr(0, p + 1);
        }

        std::string getFileName(const std::string& s)
        {
            uint p = s.find_last_of(Path::delimiters);

            if (p == std::string::npos)
            {
                return s;
            }
            else
            {
                return s.substr(p + 1);
            }
        }

        std::string getExtension(const std::string& s)
        {
            uint pos = s.rfind('.');
            if (pos == std::string::npos)
            {
                return "";
            }
            else
            {
                return s.substr(pos + 1);
            }
        }

        std::string replaceExtension(const std::string& s, const std::string& extension)
        {
            uint pos = s.rfind('.');
            if (pos == std::string::npos)
            {
                return s + "." + extension;
            }
            else
            {
                return s.substr(0, pos + 1) + extension;
            }
        }

        bool equals(const std::string& s, const std::string& t)
        {
            #ifdef WIN32
                return toUpper(s) == toUpper(t);
            #else
                return s == t;
            #endif
        }
    }
}