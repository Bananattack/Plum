#include "../core/input.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Axis>()
        {
            return "plum.Input";
        }

        namespace
        {
            enum
            {
                PlusAttribute = 1,
                MinusAttribute = 2
            };
        }

        void initAxisObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Axis>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__index", [](lua_State* L)
                {
                    return script::wrapped<Axis>(L, 1)->index(L);
                }},
                {"__newindex", [](lua_State* L)
                {
                    return script::wrapped<Axis>(L, 1)->newindex(L);
                }},
                {"__tostring", [](lua_State* L)
                {
                    return script::wrapped<Axis>(L, 1)->tostring(L);
                }},
                {"get_value", [](lua_State* L)
                {
                    auto axis = script::ptr<Axis>(L, 1);
                    script::push(L, axis->value);
                    return 1;
                }},
                {"get_plus", [](lua_State* L)
                {
                    script::wrapped<Axis>(L, 1)->getAttribute(L, PlusAttribute);
                    return 1;
                }},
                {"get_minus", [](lua_State* L)
                {
                    script::wrapped<Axis>(L, 1)->getAttribute(L, MinusAttribute);
                    return 1;
                }},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);
            lua_pop(L, 1);
        }

        void pushAxisObject(lua_State* L, Axis& axis)
        {
            int joystickRef = luaL_ref(L, LUA_REGISTRYINDEX);
            auto wrap = script::push(L, &axis, joystickRef);

            lua_pushvalue(L, -1);
            int axisRef = luaL_ref(L, LUA_REGISTRYINDEX);

            script::push(L, &axis.plus, axisRef);
            wrap->setAttribute(L, PlusAttribute);
            lua_pop(L, 1);

            script::push(L, &axis.minus, axisRef);
            wrap->setAttribute(L, MinusAttribute);
            lua_pop(L, 1);
        }
    }
}