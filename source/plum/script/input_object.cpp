
#include "../core/input.h"
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

    namespace script
    {
        void initInputObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Input>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__index", [](lua_State* L)
				{
					return script::wrapped<Input>(L, 1)->index(L);
				}},
                {"__newindex", [](lua_State* L)
				{
					return script::wrapped<Input>(L, 1)->newindex(L);
				}},
                {"__tostring", [](lua_State* L)
				{
					return script::wrapped<Input>(L, 1)->tostring(L);
				}},
                {"get_pressed", [](lua_State* L)
				{
					auto inp = script::ptr<Input>(L, 1);
					script::push(L, inp->isPressed());
					return 1;
				}},
                {"set_pressed", [](lua_State* L)
				{
					auto inp = script::ptr<Input>(L, 1);
					inp->setPressed(script::get<bool>(L, 2));
					return 0;
				}},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.input = <function inputNew>
            script::push(L, "_Input");
            lua_pushcfunction(L, [](lua_State* L)
			{
				Input* input = (Input*) lua_touserdata(L, 1);
				script::push(L, input, LUA_NOREF);
				return 1;
			});
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}