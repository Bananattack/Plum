#include "../plum.h"
#include "directory.h"

namespace plum
{
    Directory::Directory(const std::string& filename)
    {
        static zzip_strings_t PLUM_PIT_EXT[] = { ".pit", ".PIT", 0 };
        
        closed = false;
        dir = zzip_opendir_ext_io(filename.c_str(), 0, PLUM_PIT_EXT, 0);
        real = dir && zzip_dir_real(dir);
    }

    Directory::~Directory()
    {
        close();
    }

    bool Directory::close()
    {
        if(active())
        {
            zzip_closedir(dir);
            closed = true;
            return true;
        }
        return false;
    }

    bool Directory::read(std::string& value)
    {
        if(!active())
        {
            value.clear();
            return false;
        }

        ZZIP_DIRENT* dirent;
        // Ignore current dir . and subdir ..
        do
        {
            dirent = zzip_readdir(dir);
        } while(dirent && (!strcmp(dirent->d_name, ".") || !strcmp(dirent->d_name, "..")));

        if(dirent)
        {
            value = std::string(dirent->d_name);
            
            return true;
        }
        else
        {
            close();
            value.clear();
            return false;
        }
    }
}
