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

    Script::Script(Engine& engine, Timer& timer, Keyboard& keyboard, Mouse& mouse, Audio& audio, Screen& screen)
        : L(luaL_newstate()),
        engine_(engine),
        timer_(timer),
        keyboard_(keyboard),
        mouse_(mouse),
        audio_(audio),
        screen_(screen)
    {
        luaL_openlibs(L);

        lua_gc(L, LUA_GCSETSTEPMUL, 400);

        // Allow the static script methods to be able to use instance variables,
        // by looking up with the lua_State.
        instances[L] = this;

        // Load library functions.
        script::initLibrary(L);

        luaL_dostring(L,
            "function plum.RequireModuleFromPit(modulename)\r\n"
                // Find the source in a .pit file.
            "    local modulepath = string.gsub(modulename, '%.', '/')\r\n"
            "    for path in string.gmatch(package.path, '([^;]+)') do\r\n"
            "        local filename = string.gsub(path, '%?', modulepath)\r\n"
            "        local f = plum.File(filename, 'r')\r\n"
            "        if f then\r\n"
                        // Read and compile the chunk.
            "            local chunk = assert(loadstring(f:readFully(), '@' .. filename))\r\n"
            "            f:close()\r\n"
                        // Success!
            "            return chunk\r\n"
            "        end\n"
            "    end\n"
                // Failed to open it.
            "    return '\\r\\n\\tno pit module \\'' .. modulename .. '\\''\r\n"
            "end\r\n"
            "\r\n"

            // Add a few more package path rules that agree with our loader a lot more.
            "package.path = package.path .. ';?.lua;?/init.lua;?\\\\init.lua'\r\n"
            // Install the loader so that it's called just before the DLL loader
            "table.insert(package.loaders, 3, plum.RequireModuleFromPit)\r\n"
        );
    }

    Script::~Script()
    {
        lua_close(L);
        instances.erase(L);
    }

    void Script::run(const std::string& filename)
    {
        File f(filename.c_str(), FileRead);
        if(!f.isActive())
        {
            throw std::runtime_error("The script file '" + filename + "' was not found.");
        }
        
        f.seek(0, SeekEnd);
        unsigned int length = f.tell();
        f.seek(0, SeekStart);

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
