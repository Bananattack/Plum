#include "../plum.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Sound>()
        {
            return "plum.Sound";
        }
    }

    namespace
    {
        typedef Sound Self;
        int create(lua_State* L)
        {
            auto filename = script::get<const char*>(L, 1);
            script::push(L, script::instance(L).engine().audio.loadSound(filename), LUA_NOREF);
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

        int play(lua_State* L)
        {
            auto s = script::ptr<Sound>(L, 1);
            if(!s)
            {
                script::push(L, 0);
                return 1;
            }

            int volume = luaL_optint(L, 2, 100);
            script::push(L, script::instance(L).engine().audio.playSound(s, volume));
            return 1;
        }
    }

    namespace script
    {
        void initSoundObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Sound>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__index", index},
                {"__newindex", newindex},
                {"__tostring", tostring},
                {"__gc", gc},
                {"play", play},
                {nullptr, nullptr}
            };
            luaL_register(L, nullptr, functions);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.sound = <function soundNew>
            lua_pushstring(L, "Sound");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}