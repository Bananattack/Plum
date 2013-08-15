#include "../core/input.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Mouse>()
        {
            return "plum.Mouse";
        }

        namespace
        {
            enum
            {
                LeftAttribute = 1,
                MiddleAttribute = 2,
                RightAttribute = 3
            };
        }

        void initMouseObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Mouse>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__index", [](lua_State* L)
                {
                    return script::wrapped<Mouse>(L, 1)->index(L);
                }},
                {"__newindex", [](lua_State* L)
                {
                    return script::wrapped<Mouse>(L, 1)->newindex(L);
                }},
                {"__tostring", [](lua_State* L)
                {
                    return script::wrapped<Mouse>(L, 1)->tostring(L);
                }},
                {"get_x", [](lua_State* L)
                {
                    auto mouse = script::ptr<Mouse>(L, 1);
                    script::push(L, mouse->getX());
                    return 1;
                }},
                {"get_y", [](lua_State* L)
                {
                    auto mouse = script::ptr<Mouse>(L, 1);
                    script::push(L, mouse->getY());
                    return 1;
                }},
                {"get_scroll", [](lua_State* L)
                {
                    auto mouse = script::ptr<Mouse>(L, 1);
                    script::push(L, mouse->getScroll());
                    return 1;
                }},
                {"get_left", [](lua_State* L)
                {
                    script::wrapped<Mouse>(L, 1)->getAttribute(L, LeftAttribute);
                    return 1;
                }},
                {"get_middle", [](lua_State* L)
                {
                    script::wrapped<Mouse>(L, 1)->getAttribute(L, MiddleAttribute);
                    return 1;
                }},
                {"get_right", [](lua_State* L)
                {
                    script::wrapped<Mouse>(L, 1)->getAttribute(L, RightAttribute);
                    return 1;
                }},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);
            lua_pop(L, 1);
        }

        void pushMouseObject(lua_State* L, Mouse& mouse)
        {
            int screenRef = luaL_ref(L, LUA_REGISTRYINDEX);
            auto wrap = script::push(L, &mouse, screenRef);

            lua_pushvalue(L, -1);
            int mouseRef = luaL_ref(L, LUA_REGISTRYINDEX);

            script::push(L, &mouse.left(), mouseRef);
            wrap->setAttribute(L, LeftAttribute);
            lua_pop(L, 1);

            script::push(L, &mouse.middle(), mouseRef);
            wrap->setAttribute(L, MiddleAttribute);
            lua_pop(L, 1);

            script::push(L, &mouse.right(), mouseRef);
            wrap->setAttribute(L, RightAttribute);
            lua_pop(L, 1);
        }
    }
}