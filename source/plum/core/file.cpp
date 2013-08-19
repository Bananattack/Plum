#include "file.h"

namespace plum
{
    namespace
    {
        const char* getModeFlags(FileOpenMode mode)
        {
            switch(mode)
            {
                case FileOpenMode::Write: return "wb";
                case FileOpenMode::Append: return "ab";
                case FileOpenMode::Read: return "rb";
                default: return "rb";
            }
        }

        bool isWriteMode(FileOpenMode mode)
        {
            switch(mode)
            {
                case FileOpenMode::Write: return true;
                case FileOpenMode::Append: return true;
                case FileOpenMode::Read: return false;
                default: return false;
            }
        }
    }

    File::File(const std::string& filename, FileOpenMode mode)
        : file(std::fopen(filename.c_str(), getModeFlags(mode))),
        writing(isWriteMode(mode))
    {
    }

    File::~File()
    {
        close();
    }

    bool File::isActive() const
    {
        return file != nullptr;
    }

    bool File::close()
    {
        if(isActive())
        {
            std::fclose(file);
            file = nullptr;
            return true;
        }
        return false;
    }

    bool File::readUnsigned8(uint8_t& value)
    {
        if(writing || !isActive())
        {
            return false;
        }
        return std::fread(&value, sizeof(uint8_t), 1, file) == 1;
    }

    bool File::readUnsigned16(uint16_t& value)
    {
        if(writing || !isActive())
        {
            return false;
        }
        return std::fread(&value, sizeof(uint16_t), 1, file) == 1;
    }

    bool File::readUnsigned32(uint32_t& value)
    {
        if(writing || !isActive())
        {
            return false;
        }
        return std::fread(&value, sizeof(uint32_t), 1, file) == 1;
    }

    bool File::readInt8(int8_t& value)
    {
        if(writing || !isActive())
        {
            return false;
        }
        return std::fread(&value, sizeof(int8_t), 1, file) == 1;
    }

    bool File::readInt16(int16_t& value)
    {
        if(writing || !isActive())
        {
            return false;
        }
        return std::fread(&value, sizeof(int16_t), 1, file) == 1;
    }

    bool File::readInt32(int32_t& value)
    {
        if(writing || !isActive())
        {
            return false;
        }
        return std::fread(&value, sizeof(int32_t), 1, file) == 1;
    }

    bool File::readFloat(float& value)
    {
        if(writing || !isActive())
        {
            return false;
        }

        return std::fread(&value, sizeof(float), 1, file) == 1;
    }
    
    bool File::readDouble(double& value)
    {
        if(writing || !isActive())
        {
            return false;
        }

        return std::fread(&value, sizeof(double), 1, file) == 1;
    }

    bool File::readString(std::string& value)
    {
        if(writing || !isActive())
        {
            return false;
        }
        return std::fread(&value[0], 1, value.size(), file) > 0;
    }

    size_t File::readRaw(void* raw, size_t length)
    {
        if(writing || !isActive())
        {
            return false;
        }
        return std::fread(raw, 1, length, file);
    }

    /*
        Inspired by Verge's FileReadLn implemenation. Here is the license!

        Copyright (c) 1997-2007, Benjamin Eirich (vecna).
        Additional code by:
            Jesse Rusak [mac port]
            Matthew Gambrell (zeromus) [misc code contributions]
            Charles Rector (aen) [misc code contributions]
            Shamus Peveril (Kildorf) [misc code contributions]
            Andrew G. Crowell (Overkill) [misc code contributions]

        All rights reserved.

        Redistribution and use in source and binary forms, with or without modification, 
        are permitted provided that the following conditions are met:

        * Redistributions of source code must retain the above copyright notice, this 
        list of conditions and the following disclaimer. 
        * Redistributions in binary form must reproduce the above copyright notice, 
        this list of conditions and the following disclaimer in the documentation 
        and/or other materials provided with the distribution. 
        * Neither the name of VERGE-RPG.COM nor the names of its contributors may be
        used to endorse or promote products derived from this software without specific
        prior written permission. 

        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
        ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
        WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
        DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE 
        FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
        DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
        SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
        CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
        OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    
    */
    bool File::readLine(std::string& value)
    {
        if(writing || !isActive())
        {
            return false;
        }

        value.clear();
        bool eol = false;
        bool eof = false;
        do
        {
            char buffer[256];
            if(!std::fgets(buffer, sizeof(buffer), file))
            {
                eof = true;
            }
            else
            {
                size_t len = std::strlen(buffer);
                if(buffer[len - 2] == '\r' && buffer[len - 1] == '\n')
                {
                    buffer[len - 2] = 0;
                    eol = true;
                }
                else if(buffer[len - 1] == '\r' || buffer[len - 1] == '\n')
                {
                    buffer[len - 1] = 0;
                    eol = true;
                }
                value.append(buffer);
            }            
        } while(!eol && !eof);

        return !eof || value.length() > 0;
    }

    bool File::writeUnsigned8(uint8_t value)
    {
        if(!writing || !isActive())
        {
            return false;
        }

        return std::fwrite(&value, sizeof(uint8_t), 1, file) == 1;
    }

    bool File::writeUnsigned16(uint16_t value)
    {
        if(!writing || !isActive())
        {
            return false;
        }

        return std::fwrite(&value, sizeof(uint16_t), 1, file) == 1;
    }

    bool File::writeUnsigned32(uint32_t value)
    {
        if(!writing || !isActive())
        {
            return false;
        }

        return std::fwrite(&value, sizeof(uint32_t), 1, file) == 1;
    }

    bool File::writeInt8(int8_t value)
    {
        if(!writing || !isActive())
        {
            return false;
        }

        return std::fwrite(&value, sizeof(int8_t), 1, file) == 1;
    }

    bool File::writeInt16(int16_t value)
    {
        if(!writing || !isActive())
        {
            return false;
        }

        return std::fwrite(&value, sizeof(int16_t), 1, file) == 1;
    }

    bool File::writeInt32(int32_t value)
    {
        if(!writing || !isActive())
        {
            return false;
        }

        return std::fwrite(&value, sizeof(int32_t), 1, file) == 1;
    }

    bool File::writeFloat(float value)
    {
        if(!writing || !isActive())
        {
            return false;
        }

        return std::fwrite(&value, sizeof(float), 1, file) == 1;
    }
    
    bool File::writeDouble(double value)
    {
        if(!writing || !isActive())
        {
            return false;
        }

        return std::fwrite(&value, sizeof(double), 1, file) == 1;
    }

    bool File::writeString(const std::string& value)
    {
        if(!writing || !isActive())
        {
            return false;
        }
        std::fwrite(value.data(), value.size(), 1, file);
        return true;
    }

    bool File::writeLine(const std::string& value)
    {
        if(!writing || !isActive())
        {
            return false;
        }
        std::fwrite(value.data(), value.size(), 1, file);
        std::fwrite("\r\n", 1, 2, file);
        return true;
    }

    size_t File::writeRaw(const void* raw, size_t length)
    {
        if(!writing || !isActive())
        {
            return false;
        }
        return std::fwrite(raw, 1, length, file);
    }

    bool File::seek(int position, FileSeekMode mode)
    {
        if(!isActive())
        {
            return 0;
        }
        
        int m;
        switch(mode)
        {
            case FileSeekMode::Start:   m = SEEK_SET; break;
            case FileSeekMode::Current: m = SEEK_CUR; break;
            case FileSeekMode::End:     m = SEEK_END; break;
            default: return false;
        }
        return std::fseek(file, position, m) != -1;
    }

    long File::tell()
    {
        return isActive() ? std::ftell(file) : -1;
    }

    void File::flush()
    {
        if(isActive())
        {
            std::fflush(file);
        }
    }
}
