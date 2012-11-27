#pragma once
namespace plum
{
    enum FileOpenMode
    {
        FileRead, // Load from a file. File must exist.
        FileWrite, // Save to a file. Overwrite existing file, if any.
        FileAppend, // Add to end of existing file, or create new.
    };

    enum FileSeekMode
    {
        SeekStart, // Relative to the start of file.
        SeekCurrent, // Relative to the current spot in the file.
        SeekEnd, // Relative to the end of the file.
    };

    class File
    {
        private:
            // Write mode uses physical files, read mode uses a zzip wrapped file.
            bool writing;
            // So we don't double close.
            bool closed;

            // The actual file storage.
            union
            {
                void* rawPointer;
                FILE* physicalFile;
                ZZIP_FILE* zzipFile;
            };

        public:
            File(const char* filename, FileOpenMode mode);
            ~File();

            bool close();
            bool active()
            {
                return !closed && rawPointer != NULL;
            }

            bool readU8(uint8_t& value);
            bool readU16(uint16_t& value);
            bool readU32(uint32_t& value);
            bool readInt8(int8_t& value);
            bool readInt16(int16_t& value);
            bool readInt32(int32_t& value);
            bool readFloat(float& value);
            bool readDouble(double& value);
            bool readLine(std::string& value);
            
            int readRaw(void* buffer, size_t size);
            int writeRaw(const void* buffer, size_t size);

            bool readVergeCompressed(void* buffer, size_t size);

            bool seek(int position, FileSeekMode mode);
            int tell();

            bool writeU8(uint8_t value);
            bool writeU16(uint16_t value);
            bool writeU32(uint32_t value);
            bool writeInt8(int8_t value);
            bool writeInt16(int16_t value);
            bool writeInt32(int32_t value);
            bool writeFloat(float value);
            bool writeDouble(double value);
            bool writeString(const char* value, size_t size);
            bool writeLine(const char* value, size_t size);
    };

    class CoronaPlumFile : public corona::DLLImplementation<corona::File>
    {
        public:
            CoronaPlumFile(plum::File* f)
            {
                file = f;
            }

            virtual ~CoronaPlumFile()
            {
                delete file;
            }

            virtual int COR_CALL read(void* buffer, int size)
            {
                return file->readRaw(buffer, size);
            }

            virtual int COR_CALL write(const void *buffer, int size)
            {
                return file->writeRaw(buffer, size);
            }

            virtual bool COR_CALL seek(int position, corona::File::SeekMode mode)
            {
                FileSeekMode m;
                switch (mode)
                {
                    case BEGIN: m = SeekStart; break;
                    case CURRENT: m = SeekCurrent; break;
                    case END: m = SeekEnd; break;
                    default: return false;
                }
                return file->seek(position, m);
            }

            virtual int COR_CALL tell()
            {
                return file->tell();
            }
        private:
            plum::File* file;
    };
    
    class AudierePlumFile : public audiere::RefImplementation<audiere::File>
    {
        public:
            AudierePlumFile(plum::File* f) 
            {
                file = f;
            }

            virtual ~AudierePlumFile()
            {
                delete file;
            }

            virtual int ADR_CALL read(void* buffer, int size)
            {
                return file->readRaw(buffer, size);
            }

            virtual bool ADR_CALL seek(int position, audiere::File::SeekMode mode)
            {
                FileSeekMode m;
                switch (mode)
                {
                    case BEGIN: m = SeekStart; break;
                    case CURRENT: m = SeekCurrent; break;
                    case END: m = SeekEnd; break;
                    default: return false;
                }
                return file->seek(position, m);
            }

            virtual int ADR_CALL tell()
            {
                return file->tell();
            }
        private:
            plum::File* file;
    };


    // Useful helper for opening Corona and Audiere files.
    template<typename T> T* OpenLibraryFileWrapper(const char* filename, bool writeable)
    {
        return new T(new File(filename, writeable ? FileWrite : FileRead));
    }
}