#ifndef PLUM_FILE_H
#define PLUM_FILE_H

#include <string>
extern "C"
{
    #include <zzip/lib.h>
}
#include <cstdint>

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
            File(const std::string& filename, FileOpenMode mode);
            ~File();

            bool close();
            bool active()
            {
                return !closed && rawPointer != nullptr;
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
            bool writeString(const std::string& value, size_t size);
            bool writeLine(const std::string& value, size_t size);
    };
}

#endif