#include "../plum.h"

namespace Plum
{
	// Basically rips off zzip_fopen, but with the ability to check .pit files instead.
	static ZZIP_FILE* zzip_fopen_plum(const char* filename, const char* mode)
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

	File::File(const char* filename, FileOpenMode mode)
	{
		if(mode == FileWrite)
		{
			writing = true;
			physicalFile = fopen(filename, "wb");
		}
		else if(mode == FileAppend)
		{
			writing = true;
			physicalFile = fopen(filename, "ab");
		}
		else if(mode == FileRead)
		{
			writing = false;
			zzipFile = zzip_fopen_plum(filename, "rb");
		}
	}

	File::~File()
	{
		close();
	}

	bool File::close()
	{
		if(active())
		{
			if(writing && physicalFile)
			{
				fclose(physicalFile);
			}
			else if(!writing && zzipFile)
			{
				zzip_close(zzipFile);
			}
			closed = true;
			return true;
		}
		return false;
	}

	bool File::readU8(u8& value)
	{
		// Can't access in write mode. Shoo.
		if(writing || !active())
		{
			return false;
		}
		// Read it!
		return zzip_fread(&value, sizeof(u8), 1, zzipFile) == 1;
	}

	// TODO: endian.
	bool File::readU16(u16& value)
	{
		// Can't access in write mode. Shoo.
		if(writing || !active())
		{
			return false;
		}
		// Read it!
		return zzip_fread(&value, sizeof(u16), 1, zzipFile) == 1;
	}

	// TODO: endian.
	bool File::readU32(u32& value)
	{
		// Can't access in write mode. Shoo.
		if(writing || !active())
		{
			return false;
		}
		// Read it!
		return zzip_fread(&value, sizeof(u32), 1, zzipFile) == 1;
	}

	bool File::readInt8(int8& value)
	{
		// Can't access in write mode. Shoo.
		if(writing || !active())
		{
			return false;
		}
		// Read it!
		return zzip_fread(&value, sizeof(int8), 1, zzipFile) == 1;
	}

	// TODO: endian.
	bool File::readInt16(int16& value)
	{
		// Can't access in write mode. Shoo.
		if(writing || !active())
		{
			return false;
		}
		// Read it!
		return zzip_fread(&value, sizeof(int16), 1, zzipFile) == 1;
	}

	// TODO: endian.
	bool File::readInt32(int32& value)
	{
		// Can't access in write mode. Shoo.
		if(writing || !active())
		{
			return false;
		}
		// Read it!
		return zzip_fread(&value, sizeof(int32), 1, zzipFile) == 1;
	}

	static int zzip_getc (ZZIP_FILE *f)
	{
		char c;
		return (zzip_fread(&c, sizeof(char), 1, f) == 0) ? EOF : (int) c;
	}

	// A helper function to emulate fgets() behaviour with zzip.
	static char* zzip_fgets(char *str, int size, ZZIP_FILE *stream)
	{
		int c, i;

		for (i = 0; i < size-1; i++)
		{
			c = zzip_getc(stream);
			if (c == EOF)
			{
				break;
			}
			str[i] = c;
			if (c == '\n')
			{
				str[i++] = '\n';
				break;
			}
		}
		str[i] = 0;

		return str;
	}

	bool File::readLine(std::string& value)
	{
		// Can't access in write mode. Shoo.
		if(writing || !active())
		{
			return false;
		}

		// Adapted from FileReadLn in Verge, with modifications to make it fit nicer.
		char buffer[256];
		// Wipe out old value data.
		value.clear();
		// EOL status: 0 = none, 1 = newline, 2 = eof
		int eol = 0;
		do
		{
			zzip_fgets(buffer, 255, zzipFile);

			// We didn't read anything, this is eof
			if(buffer[0] == '\0')
			{
				eol = 2; 
			}
			// Last character is an end-of-line character, so it's the end of the current line.
			else if(buffer[strlen(buffer) - 1] == '\r' || buffer[strlen(buffer) - 1] == '\n')
			{
				buffer[strlen(buffer) - 1] = '\0';
				eol = 1;
			}

			value += buffer;
		} while(!eol);

		// If we encounter an EOF *and* there was nothing read,
		// return false to make it easy to tell there was a problem.
		if(eol == 2 && value.length() == 0)
		{
			return false;
		}
		// Otherwise, we have a happy line read :D
		else
		{
			return true;
		}
	}

	int File::readRaw(void* buffer, size_t size)
	{
		// Can't access in write mode. Shoo.
		if(writing || !active())
		{
			return 0;
		}
		// Read it!
		return zzip_fread(buffer, 1, size, zzipFile);
	}

	int File::writeRaw(const void* buffer, size_t size)
	{
		// Can't access in read mode. Shoo.
		if(!writing || !active())
		{
			return 0;
		}
		// Read it!
		return fwrite(buffer, 1, size, physicalFile);
	}

	bool File::seek(int position, FileSeekMode mode)
	{
		// Can't seek in write mode. Shoo.
		if(writing || !active())
		{
			return 0;
		}
		
		int m;
		switch (mode)
		{
			case SeekStart:   m = SEEK_SET; break;
			case SeekCurrent: m = SEEK_CUR; break;
			case SeekEnd:     m = SEEK_END; break;
			default: return false;
		}
		return zzip_seek(zzipFile, position, m) != -1;
	}

	int File::tell()
	{
		return (writing || !active()) ? -1 : zzip_tell(zzipFile);
	}

	bool File::writeU8(u8 value)
	{
		// Can't do this in read mode. Shoo.
		if(!writing || !active())
		{
			return false;
		}

		return fwrite(&value, sizeof(u8), 1, physicalFile) == 1;
	}

	bool File::writeU16(u16 value)
	{
		// Can't do this in read mode. Shoo.
		if(!writing || !active())
		{
			return false;
		}

		return fwrite(&value, sizeof(u16), 1, physicalFile) == 1;
	}

	bool File::writeU32(u32 value)
	{
		// Can't do this in read mode. Shoo.
		if(!writing || !active())
		{
			return false;
		}

		return fwrite(&value, sizeof(u32), 1, physicalFile) == 1;
	}

	bool File::writeInt8(int8 value)
	{
		// Can't do this in read mode. Shoo.
		if(!writing || !active())
		{
			return false;
		}

		return fwrite(&value, sizeof(int8), 1, physicalFile) == 1;
	}

	bool File::writeInt16(int16 value)
	{
		// Can't do this in read mode. Shoo.
		if(!writing || !active())
		{
			return false;
		}

		return fwrite(&value, sizeof(int16), 1, physicalFile) == 1;
	}

	bool File::writeInt32(int32 value)
	{
		// Can't do this in read mode. Shoo.
		if(!writing || !active())
		{
			return false;
		}

		return fwrite(&value, sizeof(int32), 1, physicalFile) == 1;
	}

	bool File::writeString(const char* value, size_t size)
	{
		// Can't do this in read mode. Shoo.
		if(!writing || !active())
		{
			return false;
		}
		fwrite(value, size, 1, physicalFile);
		return true;
	}

	bool File::writeLine(const char* value, size_t size)
	{
		// Can't do this in read mode. Shoo.
		if(!writing || !active())
		{
			return false;
		}
		fwrite(value, size, 1, physicalFile);
		fputs("\r\n", physicalFile);
		return true;
	}
}