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
        lua_getglobal(L, "debug");
        lua_getfield(L, -1, "traceback");

        lua_getglobal(L, "require");
        script::push(L, "system");

        if(lua_pcall(L, 1, 0, -3))
        {
            throw std::runtime_error(lua_tostring(L, -1));
        }
        lua_pop(L, 2);
    }

    namespace script
    {
        Script& instance(lua_State* L)
        {
            return *instances[L];
        }
    }
}
