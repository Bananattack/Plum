
#include "script.h"
#include "../core/image.h"
#include "../core/canvas.h"
#include "../core/sheet.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Image>()
        {
            return "plum.Image";
        }
    }

    namespace script
    {
        void initImageObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Image>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", [](lua_State* L)
                {
                    return script::wrapped<Image>(L, 1)->gc(L);
                }},
                {"__index", [](lua_State* L)
                {
                    return script::wrapped<Image>(L, 1)->index(L);
                }},
                {"__newindex", [](lua_State* L)
                {
                    return script::wrapped<Image>(L, 1)->newindex(L);
                }},
                {"__tostring", [](lua_State* L)
                {
                    return script::wrapped<Image>(L, 1)->tostring(L);
                }},
                {"draw", [](lua_State* L)
                {
                    auto img = script::ptr<Image>(L, 1);
                    auto x = script::get<int>(L, 2);
                    auto y = script::get<int>(L, 3);
                    auto transform = script::is<nullptr_t>(L, 5) ? nullptr : script::ptr<Transform>(L, 4);
                    auto screen = script::is<nullptr_t>(L, 5) ? script::ptr<Screen>(L, 4) : script::ptr<Screen>(L, 5);

                    if(transform)
                    {
                        img->draw(x, y, *transform, *screen);
                    }
                    else
                    {
                        img->draw(x, y, *screen);
                    }

                    return 0;
                }},
                {"drawFrame", [](lua_State* L)
                {
                    auto img = script::ptr<Image>(L, 1);
                    auto sheet = script::ptr<Sheet>(L, 2);
                    auto frame = script::get<int>(L, 3);
                    auto x = script::get<int>(L, 4);
                    auto y = script::get<int>(L, 5);
                    auto transform = script::is<nullptr_t>(L, 7) ? nullptr : script::ptr<Transform>(L, 6);
                    auto screen = script::is<nullptr_t>(L, 7) ? script::ptr<Screen>(L, 6) : script::ptr<Screen>(L, 7);

                    if(transform)
                    {
                        img->drawFrame(*sheet, frame, x, y, *transform, *screen);
                    }
                    else
                    {
                        img->drawFrame(*sheet, frame, x, y, *screen);
                    }

                    return 0;
                }},
                {"refresh", [](lua_State* L)
                {
                    auto img = script::ptr<Image>(L, 1);
                    img->refresh();

                    return 1;
                }},
                {"get_width", [](lua_State* L)
                {
                    auto img = script::ptr<Image>(L, 1);
                    script::push(L, img->canvas().getWidth());

                    return 1;
                }},
                {"get_height", [](lua_State* L)
                {
                    auto img = script::ptr<Image>(L, 1);
                    script::push(L, img->canvas().getHeight());

                    return 1;
                }},
                {"get_canvas", [](lua_State* L)
                {
                    auto img = script::ptr<Image>(L, 1); 

                    // Push reference to this, so the image stays around
                    // as long as it's required for the child.
                    lua_pushvalue(L, 1);
                    int ref = luaL_ref(L, LUA_REGISTRYINDEX);

                    script::push(L, &img->canvas(), ref);
                    return 1;
                }},
                {"get_trueWidth", [](lua_State* L)
                {
                    auto img = script::ptr<Image>(L, 1);
                    script::push(L, img->canvas().getTrueWidth());

                    return 1;
                }},
                {"get_trueHeight", [](lua_State* L)
                {
                    auto img = script::ptr<Image>(L, 1);
                    script::push(L, img->canvas().getTrueHeight());

                    return 1;
                }},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);
                
            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Image = <function create>
            script::push(L, "Image");
            lua_pushcfunction(L, [](lua_State* L)
            {
                if(script::is<const char*>(L, 1))
                {
                    auto filename = script::get<const char*>(L, 1);
                    script::push(L, new Image(Canvas::load(filename)), LUA_NOREF);

                    return 1;
                }
                else if(script::is<Canvas>(L, 1))
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    script::push(L, new Image(*canvas), LUA_NOREF);

                    return 1;
                }
                luaL_error(L, "Attempt to call plum.Image constructor with invalid argument types.\r\nMust be (string filename) or (plum.Canvas canvas).");
                return 0;
            });
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}
