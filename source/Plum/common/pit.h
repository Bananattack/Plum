#pragma once

namespace Plum
{
	ZZIP_FILE* zzip_fopen_plum(const char* filename, const char* mode);

	class PitCoronaFile : public corona::DLLImplementation<corona::File>
	{
		public:
			PitCoronaFile(ZZIP_FILE* f)
			{
				logFormat("Allocating file 0x%x", f);
				file = f;
			}

			virtual ~PitCoronaFile()
			{
				logFormat("Deleting file 0x%x", file);
				zzip_close(file);
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

			virtual bool COR_CALL seek(int position, corona::File::SeekMode mode)
			{
				int m;
				switch (mode)
				{
					case BEGIN:   m = SEEK_SET; break;
					case CURRENT: m = SEEK_CUR; break;
					case END:     m = SEEK_END; break;
					default: return false;
				}
				return zzip_seek(file, position, m) != -1;
			}

			virtual int COR_CALL tell()
			{
				return zzip_tell(file);
			}
		private:
			ZZIP_FILE* file;
	};

	class PitAudiereFile : public audiere::RefImplementation<audiere::File>
	{
		public:
			PitAudiereFile(ZZIP_FILE* f) 
			{
				file = f;
			}

			virtual ~PitAudiereFile()
			{
				zzip_fclose(file);
			}

			virtual int ADR_CALL read(void* buffer, int size)
			{
				int result = zzip_fread(buffer, 1, size, file);
				/*printf("Read %d/%d bytes:", result, size);
				fwrite(buffer, 1, result, stdout);
				printf("\n");*/
				return result;
			}

			virtual bool ADR_CALL seek(int position, audiere::File::SeekMode mode)
			{
				int m;
				switch (mode)
				{
					case BEGIN:   m = SEEK_SET; break;
					case CURRENT: m = SEEK_CUR; break;
					case END:     m = SEEK_END; break;
					default: return false;
				}
				return zzip_seek(file, position, m) != -1;
			}

			virtual int ADR_CALL tell()
			{
				return zzip_tell(file);
			}
		private:
			ZZIP_FILE* file;
	};

	corona::File* OpenPitCoronaFile(const char* filename, bool writeable);
	audiere::File* OpenPitAudiereFile(const char* filename, bool writeable);
}