#ifndef PLUM_FILE_H
#define PLUM_FILE_H

#include <string>
#include <cstdio>
#include <cstdint>
#include <cstring>

namespace plum
{
    enum class FileOpenMode
    {
        Read, // Load from a file. File must exist.
        Write, // Save to a file. Overwrite existing file, if any.
        Append, // Add to end of existing file, or create new.
    };

    enum class FileSeekMode
    {
        Start, // Relative to the start of file.
        Current, // Relative to the current spot in the file.
        End, // Relative to the end of the file.
    };

    class File
    {
        public:
            File(const std::string& filename, FileOpenMode mode);
            ~File();

            bool isActive() const;
            bool close();

            bool readUnsigned8(uint8_t& value);
            bool readUnsigned16(uint16_t& value);
            bool readUnsigned32(uint32_t& value);
            bool readInt8(int8_t& value);
            bool readInt16(int16_t& value);
            bool readInt32(int32_t& value);
            bool readFloat(float& value);
            bool readDouble(double& value);
            bool readString(std::string& value);
            bool readLine(std::string& value);
            size_t readRaw(void* raw, size_t length);
            
            bool writeUnsigned8(uint8_t value);
            bool writeUnsigned16(uint16_t value);
            bool writeUnsigned32(uint32_t value);
            bool writeInt8(int8_t value);
            bool writeInt16(int16_t value);
            bool writeInt32(int32_t value);
            bool writeFloat(float value);
            bool writeDouble(double value);
            bool writeString(const std::string& value);
            bool writeLine(const std::string& value);
            size_t writeRaw(const void* raw, size_t length);

            bool seek(int position, FileSeekMode mode);
            long tell();

            void flush();

        private:
            std::FILE* file;
            bool writing;
    };
}

#endif
