
#include "script.h"
#include "../core/file.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<File>()
        {
            return "plum.File";
        }
    }

    namespace
    {
        typedef File Self;

        int gc(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->gc(L);
        }

        int index(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->index(L);
        }

        int newindex(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->newindex(L);
        }

        int tostring(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->tostring(L);
        }

        int pairs(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->pairs(L);
        }

        int close(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            file->close();
            return 0;
        }

        int readU8(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);

            uint8_t value;
            if(file->readU8(value))
            {
                script::push(L, value);
                return 1;
            }
            return 0;
        }

        int readU16(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);

            uint16_t value;
            if(file->readU16(value))
            {
                script::push(L, value);
                return 1;
            }
            return 0;
        }

        int readU32(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);

            uint32_t value;
            if(file->readU32(value))
            {
                script::push(L, value);
                return 1;
            }
            return 0;
        }

        int readInt8(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);

            int8_t value;
            if(file->readInt8(value))
            {
                script::push(L, value);
                return 1;
            }
            return 0;
        }

        int readInt16(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);

            int16_t value;
            if(file->readInt16(value))
            {
                script::push(L, value);
                return 1;
            }
            return 0;
        }

        int readInt32(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);

            int32_t value;
            if(file->readInt32(value))
            {
                script::push(L, value);
                return 1;
            }
            return 0;
        }

        int readFloat(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);

            float value;
            if(file->readFloat(value))
            {
                script::push(L, value);
                return 1;
            }
            return 0;
        }

        int readDouble(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);

            double value;
            if(file->readDouble(value))
            {
                script::push(L, value);
                return 1;
            }
            return 0;
        }

        int readString(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            auto blockSize = script::get<int>(L, 2);

            std::string buffer(blockSize, 0);
            if(file->readString(buffer))
            {
                lua_pushlstring(L, buffer.data(), buffer.size());
                return 1;
            }
            return 0;
        }

        int readLine(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);

            std::string value;
            if(file->readLine(value))
            {
                script::push(L, value.c_str());
                return 1;
            }
            return 0;
        }
            
        int readFully(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);

            // Get current position.
            unsigned int pos = file->tell();
            if(pos == -1)
            {
                return 0;
            }

            // Length to read = position of end - current.
            unsigned int length = 0;
            if(!file->seek(pos, SeekEnd))
            {
                return 0;
            }
            length = file->tell() - pos;
            if(!file->seek(pos, SeekStart))
            {
                return 0;
            }

            // Read the entire file into a string.
            std::string buf(length, 0);
            if(file->readString(buf))
            {
                lua_pushlstring(L, buf.data(), buf.size());
                return 1;
            }
            return 0;
        }

        int writeU8(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            auto value = script::get<int>(L, 2);

            script::push(L, file->writeU8(value));
            return 1;
        }

        int writeU16(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            auto value = script::get<int>(L, 2);

            script::push(L, file->writeU16(value));
            return 1;
        }

        int writeU32(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            auto value = script::get<int>(L, 2);

            script::push(L, file->writeU32(value));
            return 1;
        }

        int writeInt8(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            auto value = script::get<int>(L, 2);

            script::push(L, file->writeInt8(value));
            return 1;
        }

        int writeInt16(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            auto value = script::get<int>(L, 2);

            script::push(L, file->writeInt16(value));
            return 1;
        }

        int writeInt32(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            auto value = script::get<int>(L, 2);

            script::push(L, file->writeInt32(value));
            return 1;
        }

        int writeFloat(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            auto value = script::get<double>(L, 2);

            script::push(L, file->writeFloat((float) value));
            return 1;
        }

        int writeDouble(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            auto value = script::get<double>(L, 2);

            script::push(L, file->writeDouble(value));
            return 1;
        }

        int writeString(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            size_t length = 0;
            const char* buffer = luaL_checklstring(L, 2, &length);
            std::string value(buffer, buffer + length);
            script::push(L, file->writeString(value));
            return 1;
        }

        int writeLine(lua_State* L)
        {
            auto file = script::ptr<File>(L, 1);
            size_t length = 0;
            const char* buffer = luaL_checklstring(L, 2, &length);
            std::string value(buffer, buffer + length);
            script::push(L, file->writeLine(value));
            return 1;
        }
    }

    namespace script
    {
        void initFileObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<File>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", gc},
                {"__index", index},
                {"__newindex", newindex},
                {"__tostring", tostring},
				{"__pairs", pairs},
                {"close", close},
                {"readU8", readU8},
                {"readU16", readU16},
                {"readU32", readU32},
                {"readInt8", readInt8},
                {"readInt16", readInt16},
                {"readInt32", readInt32},
                {"readFloat", readFloat},
                {"readDouble", readDouble},
                {"readString", readString},
                {"readLine", readLine},
                {"readFully", readFully},
                {"writeU8", writeU8},
                {"writeU16", writeU16},
                {"writeU32", writeU32},
                {"writeInt8", writeInt8},
                {"writeInt16", writeInt16},
                {"writeInt32", writeInt32},
                {"writeFloat", writeFloat},
                {"writeDouble", writeDouble},
                {"writeString", writeString},
                {"writeLine", writeLine},
                {nullptr, nullptr},
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);
                
            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.File = <function create>
            script::push(L, "File");
            lua_pushcfunction(L, [](lua_State* L)
            {
                auto filename = script::get<const char*>(L, 1);
                auto modeString = script::get<const char*>(L, 2);
                FileOpenMode mode;

                if(modeString[0] == 'r')
                {
                    mode = FileRead;
                }
                else if(modeString[0] == 'w')
                {
                    mode = FileWrite;
                }
                else if(modeString[0] == 'a')
                {
                    mode = FileAppend;
                }
                else
                {
                    luaL_error(L, "Attempt to call plum.File constructor with invalid mode argument (argument #2).\r\nMust be 'r', 'w' or 'a'.");
                    return 0;
                }

                auto f = new File(filename, mode);
                // Failure.
                if(!f->isActive())
                {    
                    delete f;
                    return 0;
                }

                script::push(L, f, LUA_NOREF);
                return 1;
            });
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}
