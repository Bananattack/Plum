#ifndef PLUM_PIT_H
#define PLUM_PIT_H

namespace Plum
{
	ZZIP_FILE* zzip_fopen_plum(const char* filename, const char* mode);

	class PitCoronaFile : public corona::DLLImplementation<corona::File>
	{
		private:
			ZZIP_FILE* file;
		public:
			PitCoronaFile(ZZIP_FILE* f) : file(f) {}
			~PitCoronaFile()
			{
				zzip_fclose(file);
			}

			virtual int COR_CALL read(void* buffer, int size)
			{
				return zzip_fread(buffer, 1, size, file);
			}

			virtual int COR_CALL write(const void *buffer, int size)
			{
				// zzip has no write utility, so neither does this.
				// Just don't try and call this to save a png into a zip file and we're fine.
				// (ie. use regular files in that case)
				return 0;
			}

			virtual bool COR_CALL seek(int position, SeekMode mode)
			{
				int m;
				switch (mode)
				{
					case BEGIN:   m = SEEK_SET; break;
					case CURRENT: m = SEEK_CUR; break;
					case END:     m = SEEK_END; break;
					default: return false;
				}
				return zzip_seek(file, position, m) == 0;
			}

			virtual int COR_CALL tell()
			{
				return zzip_tell(file);
			}
	};

	corona::File* OpenPitCoronaFile(const char* filename, bool writeable);
}

#endif