
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

        void initTilemapObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Tilemap>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", [](lua_State* L)
                {
                    return script::wrapped<Tilemap>(L, 1)->gc(L);
                }},
                {"__index", [](lua_State* L)
                {
                    return script::wrapped<Tilemap>(L, 1)->index(L);
                }},
                {"__newindex", [](lua_State* L)
                {
                    return script::wrapped<Tilemap>(L, 1)->newindex(L);
                }},
                {"__tostring", [](lua_State* L)
                {
                    return script::wrapped<Tilemap>(L, 1)->tostring(L);
                }},
                {"get_width", [](lua_State* L)
                {
                    auto m = script::ptr<Tilemap>(L, 1);
                    script::push(L, m->getWidth());
                    return 1;
                }},
                {"get_height", [](lua_State* L)
                {
                    auto m = script::ptr<Tilemap>(L, 1);
                    script::push(L, m->getHeight());
                    return 1;
                }},
                {"getTile", [](lua_State* L)
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
                }},
                {"setTile", [](lua_State* L)
                {
                    auto m = script::ptr<Tilemap>(L, 1);
                    int tx = script::get<int>(L, 2);
                    int ty = script::get<int>(L, 3);
                    unsigned int tileIndex = script::get<int>(L, 4);
                    m->setTile(tx, ty, tileIndex);
                    return 0;
                }},
                {"rect", [](lua_State* L)
                {
                    auto m = script::ptr<Tilemap>(L, 1);
                    int tx = script::get<int>(L, 2);
                    int ty = script::get<int>(L, 3);
                    int tx2 = script::get<int>(L, 4);
                    int ty2 = script::get<int>(L, 5);
                    unsigned int tileIndex = script::get<int>(L, 6);
                    m->rect(tx, ty, tx2, ty2, tileIndex);
                    return 0;
                }},
                {"solidRect", [](lua_State* L)
                {
                    auto m = script::ptr<Tilemap>(L, 1);
                    int tx = script::get<int>(L, 2);
                    int ty = script::get<int>(L, 3);
                    int tx2 = script::get<int>(L, 4);
                    int ty2 = script::get<int>(L, 5);
                    unsigned int tileIndex = script::get<int>(L, 6);
                    m->solidRect(tx, ty, tx2, ty2, tileIndex);
                    return 0;
                }},
                {"stamp", [](lua_State* L)
                {
                    auto m = script::ptr<Tilemap>(L, 1);
                    int tx = script::get<int>(L, 2);
                    int ty = script::get<int>(L, 3);
                    auto dest = script::ptr<Tilemap>(L, 4);
                    m->stamp(tx, ty, dest);
                    return 0;
                }},
                {"draw", [](lua_State* L)
                {
                    auto m = script::ptr<Tilemap>(L, 1);
                    auto img = script::ptr<Image>(L, 2);
                    auto spr = script::ptr<Sheet>(L, 3);
                    int worldX = script::get<int>(L, 4);
                    int worldY = script::get<int>(L, 5);
                    int destX = script::get<int>(L, 6);
                    int destY = script::get<int>(L, 7);
                    int tilesWide = script::get<int>(L, 8);
                    int tilesHigh = script::get<int>(L, 9);
                    auto screen = script::ptr<Screen>(L, 10);

                    m->draw(*img, *spr, worldX, worldY, destX, destY, tilesWide, tilesHigh, *screen);
                    return 0;
                }},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum[classname] = create
            script::push(L, "Tilemap");
            lua_pushcfunction(L, [](lua_State* L)
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
            });
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}
