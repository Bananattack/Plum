
#include "../engine/input.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Input>()
        {
            return "plum.Input";
        }
    }

    namespace
    {
        typedef Input Self;
        int create(lua_State* L)
        {
            Input* input = (Input*) lua_touserdata(L, 1);
            script::push(L, input, LUA_NOREF);
            return 1;
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

        int getpressed(lua_State* L)
        {
            auto inp = script::ptr<Input>(L, 1);
            script::push(L, inp->isPressed());
            return 1;
        }

        int setpressed(lua_State* L)
        {
            auto inp = script::ptr<Input>(L, 1);
            inp->setPressed(script::get<bool>(L, 2));
            return 0;
        }
    }

    namespace script
    {
        void initInputObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Self>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__index", index},
                {"__newindex", newindex},
                {"__tostring", tostring},
                {"getpressed", getpressed},
                {"setpressed", setpressed},
                {nullptr, nullptr}
            };
            luaL_register(L, nullptr, functions);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.input = <function inputNew>
            lua_pushstring(L, "_Input");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}