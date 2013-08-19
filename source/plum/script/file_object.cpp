
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

        void initFileObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<File>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", [](lua_State* L) { return script::wrapped<File>(L, 1)->gc(L); }},
                {"__index", [](lua_State* L) { return script::wrapped<File>(L, 1)->index(L); }},
                {"__newindex", [](lua_State* L) { return script::wrapped<File>(L, 1)->newindex(L); }},
                {"__tostring", [](lua_State* L) { return script::wrapped<File>(L, 1)->tostring(L); }},
                {"__pairs", [](lua_State* L) { return script::wrapped<File>(L, 1)->pairs(L); }},
                {"close", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    file->close();
                    return 0;
                }},
                {"readUnsigned8", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);

                    uint8_t value;
                    if(file->readUnsigned8(value))
                    {
                        script::push(L, value);
                        return 1;
                    }
                    return 0;
                }},
                {"readUnsigned16", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);

                    uint16_t value;
                    if(file->readUnsigned16(value))
                    {
                        script::push(L, value);
                        return 1;
                    }
                    return 0;
                }},
                {"readUnsigned32", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);

                    uint32_t value;
                    if(file->readUnsigned32(value))
                    {
                        script::push(L, value);
                        return 1;
                    }
                    return 0;
                }},
                {"readInt8", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);

                    int8_t value;
                    if(file->readInt8(value))
                    {
                        script::push(L, value);
                        return 1;
                    }
                    return 0;
                }},
                {"readInt16", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);

                    int16_t value;
                    if(file->readInt16(value))
                    {
                        script::push(L, value);
                        return 1;
                    }
                    return 0;
                }},
                {"readInt32", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);

                    int32_t value;
                    if(file->readInt32(value))
                    {
                        script::push(L, value);
                        return 1;
                    }
                    return 0;
                }},
                {"readFloat", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);

                    float value;
                    if(file->readFloat(value))
                    {
                        script::push(L, value);
                        return 1;
                    }
                    return 0;
                }},
                {"readDouble", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);

                    double value;
                    if(file->readDouble(value))
                    {
                        script::push(L, value);
                        return 1;
                    }
                    return 0;
                }},
                {"readString", [](lua_State* L)
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
                }},
                {"readLine", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);

                    std::string value;
                    if(file->readLine(value))
                    {
                        script::push(L, value.c_str());
                        return 1;
                    }
                    return 0;
                }},
                {"readFully", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);

                    // Get current position.
                    auto pos = file->tell();
                    if(pos == -1)
                    {
                        return 0;
                    }

                    // Length to read = position of end - current.
                    auto length = 0;
                    if(!file->seek(pos, FileSeekMode::End))
                    {
                        return 0;
                    }
                    length = file->tell() - pos;
                    if(!file->seek(pos, FileSeekMode::Start))
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
                }},
                {"writeUnsigned8", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    auto value = script::get<int>(L, 2);

                    script::push(L, file->writeUnsigned8(value));
                    return 1;
                }},
                {"writeUnsigned16", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    auto value = script::get<int>(L, 2);

                    script::push(L, file->writeUnsigned16(value));
                    return 1;
                }},
                {"writeUnsigned32", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    auto value = script::get<int>(L, 2);

                    script::push(L, file->writeUnsigned32(value));
                    return 1;
                }},
                {"writeInt8", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    auto value = script::get<int>(L, 2);

                    script::push(L, file->writeInt8(value));
                    return 1;
                }},
                {"writeInt16", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    auto value = script::get<int>(L, 2);

                    script::push(L, file->writeInt16(value));
                    return 1;
                }},
                {"writeInt32", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    auto value = script::get<int>(L, 2);

                    script::push(L, file->writeInt32(value));
                    return 1;
                }},
                {"writeFloat", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    auto value = script::get<double>(L, 2);

                    script::push(L, file->writeFloat((float) value));
                    return 1;
                }},
                {"writeDouble", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    auto value = script::get<double>(L, 2);

                    script::push(L, file->writeDouble(value));
                    return 1;
                }},
                {"writeString", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    size_t length = 0;
                    const char* buffer = luaL_checklstring(L, 2, &length);
                    std::string value(buffer, buffer + length);
                    script::push(L, file->writeString(value));
                    return 1;
                }},
                {"writeLine", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    size_t length = 0;
                    const char* buffer = luaL_checklstring(L, 2, &length);
                    std::string value(buffer, buffer + length);
                    script::push(L, file->writeLine(value));
                    return 1;
                }},
                {"tell", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    script::push(L, int(file->tell()));
                    return 1;
                }},
                {"seek", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    auto position = script::get<int>(L, 2);
                    auto mode = FileSeekMode(script::get<int>(L, 3));
                    script::push(L, file->seek(position, mode));
                    return 1;
                }},
                {"flush", [](lua_State* L)
                {
                    auto file = script::ptr<File>(L, 1);
                    file->flush();
                    return 0;
                }},
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
                auto mode = FileOpenMode(script::get<int>(L, 2));
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
