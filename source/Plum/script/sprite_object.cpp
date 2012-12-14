
#include "../core/image.h"
#include "../core/canvas.h"
#include "../core/sprite.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Sprite>()
        {
            return "plum.Sprite";
        }
    }

    namespace
    {
        typedef Sprite Self;

        int create(lua_State* L)
        {
            if((script::is<Image>(L, 1) || script::is<Image>(L, 1))
                && script::is<int>(L, 2) && script::is<int>(L, 3))
            {
                int w = script::get<int>(L, 2);
                int h = script::get<int>(L, 3);
                if(script::is<Canvas>(L, 1))
                {
                    auto canvas = script::ptr<Canvas>(L, 1);
                    script::push(L, new Sprite(*canvas, w, h), LUA_NOREF);
                    return 1;
                }
                else if(script::is<Image>(L, 1))
                {
                    auto img = script::ptr<Image>(L, 1);
                    script::push(L, new Sprite(img->canvas(), w, h), LUA_NOREF);
                    return 1;
                }
            }
            luaL_error(L, "Attempt to call plum.Sprite constructor with invalid argument types.\r\nMust be (Image img, int frameWidth, int frameHeight) or (Canvas canvas, int frameWidth, int frameHeight).");
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

        int getimage(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);

            // Push reference to this, so the spritesheet stays around
            // as long as it's required for the child.
            lua_pushvalue(L, 1);
            int ref = luaL_ref(L, LUA_REGISTRYINDEX);

            script::push(L, &spr->image(), ref);
            return 1;
        }

        int blitFrame(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);
            int x = script::get<int>(L, 2);
            int y = script::get<int>(L, 3);
            int f = script::get<int>(L, 4);
            BlendMode mode = (BlendMode) script::get<int>(L, 5, BlendPreserve);
            Color tint = script::get<int>(L, 6, Color::White);

            spr->blitFrame(x, y, f, mode, tint);
            return 0;
        }

        int getFramePixel(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);
            int f = script::get<int>(L, 2);
            int x = script::get<int>(L, 3);
            int y = script::get<int>(L, 4);
            int pixel = spr->getFramePixel(f, x, y);

            script::push(L, pixel);
            return 1;
        }

        int getframeWidth(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);
            script::push(L, spr->getFrameWidth());
            return 1;
        }

        int setframeWidth(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);
            spr->setFrameWidth(script::get<int>(L, 2));
            return 0;
        }

        int getframeHeight(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);
            script::push(L, spr->getFrameHeight());
            return 1;
        }

        int setframeHeight(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);
            spr->setFrameHeight(script::get<int>(L, 2));
            return 0;
        }

        int getpadding(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);
            script::push(L, spr->getPadding());
            return 1;
        }

        int setpadding(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);
            spr->setPadding(script::get<int>(L, 2));
            return 0;
        }

        int getcolumns(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);
            script::push(L, spr->getColumns());
            return 1;
        }

        int setcolumns(lua_State* L)
        {
            auto spr = script::ptr<Sprite>(L, 1);
            spr->setColumns(script::get<int>(L, 2));
            return 0;
        }
    }

    namespace script
    {
        void initSpriteObject(lua_State* L)
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
                {"blitFrame", blitFrame},
                {"getFramePixel", getFramePixel},
                {"getframeWidth", getframeWidth},
                {"setframeWidth", setframeWidth},
                {"getframeHeight", getframeHeight},
                {"setframeHeight", setframeHeight},
                {"getpadding", getpadding},
                {"setpadding", setpadding},
                {"getcolumns", getcolumns},
                {"setcolumns", setcolumns},
                {"getimage", getimage},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Sprite = <function create>
            script::push(L, "Sprite");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}
