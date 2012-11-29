#pragma once

#include <string>
extern "C"
{
    #include <zzip/lib.h>
}

namespace plum
{
    class Directory
    {
        private:
            bool closed;

            bool real;
            ZZIP_DIR* dir;
        public:
            Directory(const std::string& filename);
            ~Directory();

            bool close();

            bool isReal()
            {
                return real;
            }

            bool active()
            {
                return !closed && dir != NULL;
            }

            bool read(std::string& value);

    };
}
