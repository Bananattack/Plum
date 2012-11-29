
#include "script.h"
#include "../engine/directory.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Directory>()
        {
            return "plum.Directory";
        }
    }

    namespace
    {
        typedef Directory Self;
        int create(lua_State* L)
        {
            auto filename = script::get<const char*>(L, 1);
            auto dir = new Directory(filename);
            // Failure.
            if(!dir->active())
            {    
                delete dir;
                return 0;
            }
            script::push(L, dir, LUA_NOREF);
            return 1;
        }

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

        int close(lua_State* L)
        {
            auto dir = script::ptr<Directory>(L, 1);
            dir->close();
            return 0;
        }

        int getreal(lua_State* L)
        {
            auto dir = script::ptr<Directory>(L, 1);
            script::push(L, dir->isReal());
            return 1;
        }

        int next(lua_State* L)
        {
            auto dir = script::ptr<Directory>(L, 1);

            std::string value;
            if(dir->read(value))
            {
                script::push(L, value.c_str());
                return 1;
            }
            return 0;
        }
    }

    namespace script
    {
        void initDirectoryObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Directory>());
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
                {"close", close},
                {"next", next},
                {"getreal", getreal},
                {nullptr, nullptr},
            };
            luaL_register(L, nullptr, functions);

            lua_pop(L, 1);
                
            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum[classname] = <function create>
            lua_pushstring(L, "Directory");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}
