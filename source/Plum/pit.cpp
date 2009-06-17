#include "plum.h"

// Basically rips off zzip_fopen, but with the ability to check .pit files instead.
ZZIP_FILE* zzip_fopen_plum(const char* filename, const char* mode)
{
	static zzip_strings_t PLUM_PIT_EXT[] = { ".pit", ".PIT", 0 };
    int o_flags = 0;
    int o_modes = 0664;

#   ifndef O_BINARY
#   define O_BINARY 0
#   endif
#   ifndef O_NOCTTY
#   define O_NOCTTY 0
#   endif
#   ifndef O_SYNC
#   define O_SYNC 0
#   endif
#   ifndef O_NONBLOCK
#   define O_NONBLOCK 0
#   endif

    if (!mode)
	{
        mode = "rb";
	}
	// Directly copied from zzip/file.c
    for (; *mode; mode++)
    {
        switch (*mode)
        {
			case '0': case '1': case '2': case '3': case '4': 
			case '5': case '6': case '7': case '8': case '9':
				continue; /* ignore if not attached to other info */
			case 'r': o_flags |= mode[1] == '+' ? O_RDWR : O_RDONLY; break;
			case 'w': o_flags |= mode[1] == '+' ? O_RDWR : O_WRONLY; 
					  o_flags |= O_TRUNC; break;
			case 'b': o_flags |= O_BINARY; break;
			case 'f': o_flags |= O_NOCTTY; break;
			case 'i': o_modes |= ZZIP_CASELESS; break;
			case '*': o_modes |= ZZIP_NOPATHS; break;
			case 'x': o_flags |= O_EXCL; break;
			case 's': o_flags |= O_SYNC; break;
			case 'n': o_flags |= O_NONBLOCK; break;
			case 'o': o_modes &=~ 07; 
						  o_modes |= ((mode[1] - '0')) & 07; continue;
			case 'g': o_modes &=~ 070; 
						  o_modes |= ((mode[1] - '0') << 3) & 070; continue;
			case 'u': o_modes &=~ 0700; 
						  o_modes |= ((mode[1] - '0') << 6) & 0700; continue;
			case 'q': o_modes |= ZZIP_FACTORY; break;
			case 'z': /* compression level */
				continue; /* currently ignored, just for write mode */
        }
    }

	return zzip_open_ext_io(filename, o_flags, o_modes, PLUM_PIT_EXT, 0);
}

corona::File* OpenPitCoronaFile(const char* filename, bool writeable)
{
	ZZIP_FILE* file = zzip_fopen_plum(filename, (writeable ? "wb" : "rb"));
	return (file ? new PitCoronaFile(file) : 0);
}