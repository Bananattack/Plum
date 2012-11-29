
#include "script.h"
#include "../video/canvas.h"
#include "../video/image.h"

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
            if(lua_isstring(L, 1))
            {
                auto filename = script::get<const char*>(L, 1);
                script::push(L, new Image(filename), LUA_NOREF);

                return 1;
            }
            else if(script::is<Canvas>(L, 1))
            {
                auto canvas = script::ptr<Canvas>(L, 1);
                script::push(L, new Image(canvas), LUA_NOREF);

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

        int blit(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto mode = BlendMode(script::get<int>(L, 4, BlendAlpha));
            auto tint = Color(script::get<int>(L, 5, Color::White));

            img->blit(x, y, mode, tint);

            return 0;
        }

        int draw(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto transform = script::ptr<Transform>(L, 2);

            img->transformBlit(transform);

            return 0;
        }

        int blitRegion(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto sx = script::get<int>(L, 2);
            auto sy = script::get<int>(L, 3);
            auto sx2 = script::get<int>(L, 4);
            auto sy2 = script::get<int>(L, 5);
            auto dx = script::get<int>(L, 6);
            auto dy = script::get<int>(L, 7);
            auto mode = BlendMode(script::get<int>(L, 8, BlendAlpha));
            auto tint = Color(script::get<int>(L, 9, Color::White));

            img->blitRegion(sx, sy, sx2, sy2, dx, dy, mode, tint);

            return 0;
        }

        int scaleBlit(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto width = script::get<int>(L, 4);
            auto height = script::get<int>(L, 5);
            auto mode = BlendMode(script::get<int>(L, 6, BlendAlpha));
            auto tint = Color(script::get<int>(L, 7, Color::White));

            img->scaleBlit(x, y, width, height, mode, tint);

            return 0;
        }

        int scaleBlitRegion(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto sx = script::get<int>(L, 2);
            auto sy = script::get<int>(L, 3);
            auto sx2 = script::get<int>(L, 4);
            auto sy2 = script::get<int>(L, 5);
            auto dx = script::get<int>(L, 6);
            auto dy = script::get<int>(L, 7);
            auto scaledWidth = script::get<int>(L, 8);
            auto scaledHeight = script::get<int>(L, 9);
            auto mode = BlendMode(script::get<int>(L, 10, BlendAlpha));
            auto tint = Color(script::get<int>(L, 11, Color::White));

            img->scaleBlitRegion(sx, sy, sx2, sy2, dx, dy, scaledWidth, scaledHeight, mode, tint);

            return 0;
        }

        int rotateBlit(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto angle = script::get<double>(L, 4);
            auto mode = BlendMode(script::get<int>(L, 5, BlendAlpha));
            auto tint = Color(script::get<int>(L, 6, Color::White));

            img->rotateBlit(x, y, angle, mode, tint);

            return 0;
        }

        int rotateBlitRegion(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto sx = script::get<int>(L, 2);
            auto sy = script::get<int>(L, 3);
            auto sx2 = script::get<int>(L, 4);
            auto sy2 = script::get<int>(L, 5);
            auto dx = script::get<int>(L, 6);
            auto dy = script::get<int>(L, 7);
            auto angle = script::get<double>(L, 8);
            auto mode = BlendMode(script::get<int>(L, 9, BlendAlpha));
            auto tint = Color(script::get<int>(L, 10, Color::White));

            img->rotateBlitRegion(sx, sy, sx2, sy2, dx, dy, angle, mode, tint);
            return 0;
        }

        int rotateScaleBlit(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto x = script::get<int>(L, 2);
            auto y = script::get<int>(L, 3);
            auto angle = script::get<double>(L, 4);
            auto scale = script::get<double>(L, 5);
            auto mode = BlendMode(script::get<int>(L, 6, BlendAlpha));
            auto tint = Color(script::get<int>(L, 7, Color::White));

            img->rotateScaleBlit(x, y, angle, scale, mode, tint);

            return 0;
        }

        int rotateScaleBlitRegion(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            auto sx = script::get<int>(L, 2);
            auto sy = script::get<int>(L, 3);
            auto sx2 = script::get<int>(L, 4);
            auto sy2 = script::get<int>(L, 5);
            auto dx = script::get<int>(L, 6);
            auto dy = script::get<int>(L, 7);
            auto angle = script::get<double>(L, 8);
            auto scale = script::get<double>(L, 9);
            auto mode = BlendMode(script::get<int>(L, 10, BlendAlpha));
            auto tint = Color(script::get<int>(L, 11, Color::White));

            img->rotateScaleBlitRegion(sx, sy, sx2, sy2, dx, dy, angle, scale, mode, tint);
            return 0;
        }

        int refresh(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            img->refresh();

            return 1;
        }

        int getwidth(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            script::push(L, img->getCanvasWidth());

            return 1;
        }

        int getheight(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            script::push(L, img->getCanvasHeight());

            return 1;
        }

        int gettrueWidth(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            script::push(L, img->getTextureWidth());

            return 1;
        }

        int gettrueHeight(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1);
            script::push(L, img->getTextureHeight());

            return 1;
        }

        int getcanvas(lua_State* L)
        {
            auto img = script::ptr<Self>(L, 1); 

            // Push reference to this, so the image stays around
            // as long as it's required for the child.
            lua_pushvalue(L, 1);
            int ref = luaL_ref(L, LUA_REGISTRYINDEX);

            script::push(L, img->getCanvas(), ref);
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
                {"blit", blit},
                {"draw", draw},
                {"blitRegion", blitRegion},
                {"scaleBlit", scaleBlit},
                {"scaleBlitRegion", scaleBlitRegion},
                {"rotateBlit", rotateBlit},
                {"rotateBlitRegion", rotateBlitRegion},
                {"rotateScaleBlit", rotateScaleBlit},
                {"rotateScaleBlitRegion", rotateScaleBlitRegion},
                {"refresh", refresh},
                {"getwidth", getwidth},
                {"getheight", getheight},
                {"getcanvas", getcanvas},
                {"gettrueWidth", gettrueWidth},
                {"gettrueHeight", gettrueHeight},
                {nullptr, nullptr}
            };
            luaL_register(L, nullptr, functions);

            lua_pop(L, 1);
                
            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Image = <function create>
            lua_pushstring(L, "Image");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}
