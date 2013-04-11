
#include "../core/screen.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Screen>()
        {
            return "plum.Screen";
        }

        namespace
        {
            enum
            {
                KeyAttribute = 1
            };
        }

        void initScreenObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Screen>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", [](lua_State* L)
                {
                    return script::wrapped<Screen>(L, 1)->gc(L);
                }},
                {"__index", [](lua_State* L)
                {
                    return script::wrapped<Screen>(L, 1)->index(L);
                }},
                {"__newindex", [](lua_State* L)
                {
                    return script::wrapped<Screen>(L, 1)->newindex(L);
                }},
                {"__tostring", [](lua_State* L)
                {
                    return script::wrapped<Screen>(L, 1)->tostring(L);
                }},
                {"clear", [](lua_State* L)
                {
                    auto screen = script::ptr<Screen>(L, 1);
                    if(script::is<nullptr_t>(L, 3))
                    {
                        Color color = script::get<int>(L, 2);
                        screen->clear(color);
                    }
                    else
                    {
                        int x = script::get<int>(L, 2);
                        int y = script::get<int>(L, 3);
                        int x2 = script::get<int>(L, 4);
                        int y2 = script::get<int>(L, 5);
                        Color color = script::get<int>(L, 6);
                        screen->clear(x, y, x2, y2, color);
                    }
                    return 0;
                }},
                {"get_width", [](lua_State* L)
                {
                    auto screen = script::ptr<Screen>(L, 1);
                    script::push(L, screen->getWidth());
                    return 1;
                }},
                {"get_height", [](lua_State* L)
                {
                    auto screen = script::ptr<Screen>(L, 1);
                    script::push(L, screen->getHeight());
                    return 1;
                }},
                {"get_opacity", [](lua_State* L)
                {
                    auto screen = script::ptr<Screen>(L, 1);
                    script::push(L, screen->getOpacity());
                    return 1;
                }},
                {"set_opacity", [](lua_State* L)
                {
                    auto screen = script::ptr<Screen>(L, 1);
                    int v = script::get<int>(L, 2);
                    screen->setOpacity(v);
                    return 0;
                }},
                {"get_title", [](lua_State* L)
                {
                    auto screen = script::ptr<Screen>(L, 1);
                    script::push(L, screen->getTitle().c_str());
                    return 1;
                }},
                {"set_title", [](lua_State* L)
                {
                    auto screen = script::ptr<Screen>(L, 1);
                    screen->setTitle(script::get<const char*>(L, 2));
                    return 0;
                }},
                {"get_key", [](lua_State* L)
                {
                    script::wrapped<Screen>(L, 1)->getAttribute(L, KeyAttribute);
                    return 1;
                }},
                {"get_close", [](lua_State* L)
                {
                    auto screen = script::ptr<Screen>(L, 1);

                    // Push reference to this, so the image stays around
                    // as long as it's required for the child.
                    lua_pushvalue(L, 1);
                    int ref = luaL_ref(L, LUA_REGISTRYINDEX);

                    script::push(L, &screen->closeButton(), ref);
                    return 1;
                }},
                {nullptr, nullptr},
            };
            luaL_setfuncs(L, functions, 0);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Screen = <function create>
            script::push(L, "Screen");
            lua_pushcfunction(L, [](lua_State* L)
            {
                int width = script::get<int>(L, 1);
                int height = script::get<int>(L, 2);
                int scale = script::get<int>(L, 3, 1);
                int win = script::get<int>(L, 4, true);

                auto scr = new Screen(script::instance(L).engine(), width, height, scale, win);
                auto wrap = script::push(L, scr, LUA_NOREF);

                pushKeyboardTable(L, scr->keyboard());
                wrap->setAttribute(L, KeyAttribute);
                lua_pop(L, 1);
                
                return 1;
            });
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}