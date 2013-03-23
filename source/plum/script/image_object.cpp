
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

    namespace
    {
        typedef Image Self;
        int create(lua_State* L)
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

        int draw(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto transform = script::is<nullptr_t>(L, 4) ? nullptr : script::ptr<Transform>(L, 4);

            if(transform)
            {
                img->draw(x, y, *transform, script::instance(L).screen());
            }
            else
            {
                img->draw(x, y, script::instance(L).screen());
            }

            return 0;
        }

        int drawFrame(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto sheet = script::ptr<Sheet>(L, 2);
            auto frame = script::get<int>(L, 3);
            auto x = script::get<int>(L, 4);
            auto y = script::get<int>(L, 5);
            auto transform = script::is<nullptr_t>(L, 6) ? nullptr : script::ptr<Transform>(L, 6);

            if(transform)
            {
                img->drawFrame(*sheet, frame, x, y, *transform, script::instance(L).screen());
            }
            else
            {
                img->drawFrame(*sheet, frame, x, y, script::instance(L).screen());
            }

            return 0;
        }

        int refresh(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            img->refresh();

            return 1;
        }

        int get_width(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            script::push(L, img->canvas().getWidth());

            return 1;
        }

        int get_height(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            script::push(L, img->canvas().getHeight());

            return 1;
        }

        int getTrueWidth(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            script::push(L, img->canvas().getTrueWidth());

            return 1;
        }

        int getTrueHeight(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            script::push(L, img->canvas().getTrueHeight());

            return 1;
        }

        int get_canvas(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1); 

            // Push reference to this, so the image stays around
            // as long as it's required for the child.
            lua_pushvalue(L, 1);
            int ref = luaL_ref(L, LUA_REGISTRYINDEX);

            script::push(L, &img->canvas(), ref);
            return 1;
        }
    }

    namespace script
    {
        void initImageObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Self>());
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
                {"draw", draw},
                {"drawFrame", drawFrame},
                {"refresh", refresh},
                {"get_width", get_width},
                {"get_height", get_height},
                {"get_canvas", get_canvas},
                {"get_trueWidth", getTrueWidth},
                {"get_trueHeight", getTrueHeight},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);
                
            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Image = <function create>
            script::push(L, "Image");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}
