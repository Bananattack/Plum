#ifndef PLUM_FILE_H
#define PLUM_FILE_H

#include <string>
#include <cstdio>
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
        public:
            File(const std::string& filename, FileOpenMode mode);
            ~File();

            bool isActive() const;
            bool close();

            bool readU8(uint8_t& value);
            bool readU16(uint16_t& value);
            bool readU32(uint32_t& value);
            bool readInt8(int8_t& value);
            bool readInt16(int16_t& value);
            bool readInt32(int32_t& value);
            bool readFloat(float& value);
            bool readDouble(double& value);
            bool readString(std::string& value);
            bool readLine(std::string& value);
            size_t readRaw(void* raw, size_t length);
            
            bool writeU8(uint8_t value);
            bool writeU16(uint16_t value);
            bool writeU32(uint32_t value);
            bool writeInt8(int8_t value);
            bool writeInt16(int16_t value);
            bool writeInt32(int32_t value);
            bool writeFloat(float value);
            bool writeDouble(double value);
            bool writeString(const std::string& value);
            bool writeLine(const std::string& value);
            size_t writeRaw(const void* raw, size_t length);

            bool seek(int position, FileSeekMode mode);
            int tell();

        private:
            std::FILE* file;
            bool writing;
    };
}

#endif