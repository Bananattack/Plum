#include <vector>
#include <unordered_map>

#include "../core/file.h"
#include "../core/engine.h"
#include "script.h"

namespace plum
{
    namespace
    {
        std::unordered_map<lua_State*, Script*> instances;
    }

    Script::Script(int argc, char** argv, Engine& engine, Timer& timer, Audio& audio)
        : L(luaL_newstate()),
        argc_(argc),
        argv_(argv),
        engine_(engine),
        timer_(timer),
        audio_(audio)
    {
        luaL_openlibs(L);
        lua_gc(L, LUA_GCSETSTEPMUL, 400);

        instances[L] = this;
        script::initLibrary(L);

        luaL_dostring(L, "package.path = package.path .. ';?.lua;?/init.lua;?\\\\init.lua'");
    }

    Script::~Script()
    {
        lua_close(L);
        instances.erase(L);
    }

    void Script::run(const std::string& filename)
    {
        File f(filename.c_str(), FileOpenMode::Read);
        if(!f.isActive())
        {
            throw std::runtime_error("The script file '" + filename + "' was not found.");
        }
        
        f.seek(0, FileSeekMode::End);
        auto length = f.tell();
        f.seek(0, FileSeekMode::Start);

        std::vector<char> buf(length);
        f.readRaw(buf.data(), length);

        std::string scriptname("@" + filename);
        if(luaL_loadbuffer(L, buf.data(), buf.size(), scriptname.c_str()) || lua_pcall(L, 0, LUA_MULTRET, 0))
        {
            throw std::runtime_error("Error found in script:\r\n" + std::string(lua_tostring(L, -1)));
        }
    }

    namespace script
    {
        Script& instance(lua_State* L)
        {
            return *instances[L];
        }
    }
}
