
#include "../core/input.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Joystick>()
        {
            return "plum.Joystick";
        }

        namespace
        {
            enum
            {
                ButtonAttribute = 1,
                AxisAttribute = 2
            };
        }

        void initJoystickObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Joystick>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__index", [](lua_State* L)
                {
                    return script::wrapped<Joystick>(L, 1)->index(L);
                }},
                {"__newindex", [](lua_State* L)
                {
                    return script::wrapped<Joystick>(L, 1)->newindex(L);
                }},
                {"__tostring", [](lua_State* L)
                {
                    return script::wrapped<Joystick>(L, 1)->tostring(L);
                }},
                {"get_active", [](lua_State* L)
                {
                    auto joystick = script::ptr<Joystick>(L, 1);
                    script::push(L, joystick->isActive());
                    return 1;
                }},
                {"get_name", [](lua_State* L)
                {
                    auto joystick = script::ptr<Joystick>(L, 1);
                    script::push(L, joystick->getName());
                    return 1;
                }},
                {"get_axisCount", [](lua_State* L)
                {
                    auto joystick = script::ptr<Joystick>(L, 1);
                    script::push(L, joystick->getAxisCount());
                    return 1;
                }},
                {"get_buttonCount", [](lua_State* L)
                {
                    auto joystick = script::ptr<Joystick>(L, 1);
                    script::push(L, joystick->getButtonCount());
                    return 1;
                }},
                {"get_button", [](lua_State* L)
                {
                    script::wrapped<Joystick>(L, 1)->getAttribute(L, ButtonAttribute);
                    return 1;
                }},
                {"get_axis", [](lua_State* L)
                {
                    script::wrapped<Joystick>(L, 1)->getAttribute(L, AxisAttribute);
                    return 1;
                }},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);
            lua_pop(L, 1);


            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Image = <function create>
            script::push(L, "Joystick");
            lua_pushcfunction(L, [](lua_State* L)
            {
                auto index = (unsigned int) script::get<int>(L, 1);
                auto joystick = new Joystick(script::instance(L).engine(), index);
                auto wrap = script::push(L, joystick, LUA_NOREF);

                lua_newtable(L);
                for(unsigned int i = 0; i < Joystick::ButtonMax; i++)
                {
                    // Designate the parent of this joystick button.
                    lua_pushvalue(L, -2);
                    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
                    script::push(L, &joystick->button(i), ref);
                    // joy.button[index] = <ref to C-side input object>
                    lua_rawseti(L, -2, i + 1);
                }
                wrap->setAttribute(L, ButtonAttribute);
                lua_pop(L, 1);

                lua_newtable(L);
                for(unsigned int i = 0; i < Joystick::AxisMax; i++)
                {
                    // Designate the parent of this joystick button.
                    lua_pushvalue(L, -2);
                    pushAxisObject(L, joystick->axis(i));
                    // joy.button[index] = <ref to C-side input object>
                    lua_rawseti(L, -2, i + 1);
                }
                wrap->setAttribute(L, AxisAttribute);
                lua_pop(L, 1);

                return 1;
            });
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}