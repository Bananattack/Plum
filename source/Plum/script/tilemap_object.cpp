
#include "../core/video.h"
#include "../core/tilemap.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Tilemap>()
        {
            return "plum.Tilemap";
        }
    }

    namespace
    {
        typedef Tilemap Self;

        int create(lua_State* L)
        {
            if(script::is<int>(L, 1) && script::is<int>(L, 2))
            {
                int w = script::get<int>(L, 1);
                int h = script::get<int>(L, 2);
                script::push(L, new Tilemap(w, h), LUA_NOREF);

                return 1;
            }
            luaL_error(L, "Attempt to call plum.Tilemap constructor with invalid argument types.\r\nMust be (int w, int h).");
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

        int getwidth(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            script::push(L, m->getWidth());
            return 1;
        }

        int getheight(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            script::push(L, m->getHeight());
            return 1;
        }

        int getTile(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            int tx = script::get<int>(L, 2);
            int ty = script::get<int>(L, 3);
            unsigned int t = m->getTile(tx, ty);

            if(t != Tilemap::InvalidTile)
            {
                script::push(L, t);
            }
            else
            {
                script::push(L, nullptr);
            }
                
            return 1;
        }

        int setTile(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            int tx = script::get<int>(L, 2);
            int ty = script::get<int>(L, 3);
            unsigned int tileIndex = script::get<int>(L, 4);
            m->setTile(tx, ty, tileIndex);
            return 0;
        }

        int rect(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            int tx = script::get<int>(L, 2);
            int ty = script::get<int>(L, 3);
            int tx2 = script::get<int>(L, 4);
            int ty2 = script::get<int>(L, 5);
            unsigned int tileIndex = script::get<int>(L, 6);
            m->rect(tx, ty, tx2, ty2, tileIndex);
            return 0;
        }

        int solidRect(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            int tx = script::get<int>(L, 2);
            int ty = script::get<int>(L, 3);
            int tx2 = script::get<int>(L, 4);
            int ty2 = script::get<int>(L, 5);
            unsigned int tileIndex = script::get<int>(L, 6);
            m->solidRect(tx, ty, tx2, ty2, tileIndex);
            return 0;
        }

        int stamp(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            int tx = script::get<int>(L, 2);
            int ty = script::get<int>(L, 3);
            auto dest = script::ptr<Tilemap>(L, 4);
            m->stamp(tx, ty, dest);
            return 0;
        }

        int blit(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            auto spr = script::ptr<Sprite>(L, 2);
            int worldX = script::get<int>(L, 3);
            int worldY = script::get<int>(L, 4);
            int destX = script::get<int>(L, 5);
            int destY = script::get<int>(L, 6);
            int tilesWide = script::get<int>(L, 7);
            int tilesHigh = script::get<int>(L, 8);
            BlendMode mode = (BlendMode) script::get<int>(L, 9, BlendPreserve);
            Color tint = script::get<int>(L, 10, Color::White);

            m->blit(script::instance(L).video(), *spr, worldX, worldY, destX, destY, tilesWide, tilesHigh, mode, tint);
            return 0;
        }
    }

    namespace script
    {
        void initTilemapObject(lua_State* L)
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
                {"getwidth", getwidth},
                {"getheight", getheight},
                {"getTile", getTile},
                {"setTile", setTile},
                {"rect", rect},
                {"solidRect", solidRect},
                {"stamp", stamp},
                {"blit", blit},
                {nullptr, nullptr}
            };
            luaL_register(L, nullptr, functions);

            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum[classname] = create
            script::push(L, "Tilemap");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}
