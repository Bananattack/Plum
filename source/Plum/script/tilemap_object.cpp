#include "../plum.h"


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
        const int TILEMAP_SPRITESHEET_REF = 1;

        int create(lua_State* L)
        {
            if(lua_isnumber(L, 1) && lua_isnumber(L, 2))
            {
                int w = lua_tointeger(L, 1);
                int h = lua_tointeger(L, 2);
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

        int getspritesheet(lua_State* L)
        {
            auto wrapper = script::wrapped<Tilemap>(L, 1);
            wrapper->getAttribute(L, TILEMAP_SPRITESHEET_REF);
            return 1;
        }

        int setspritesheet(lua_State* L)
        {
            auto wrapper = script::wrapped<Tilemap>(L, 1);
            auto spritesheet = script::ptr<Spritesheet>(L, 2);

            // Change the spritesheet
            wrapper->data->spr = spritesheet;
            // Update reference table, so the value will remain in memory as long as it's required.
            wrapper->setAttribute(L, TILEMAP_SPRITESHEET_REF);
            return 0;
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
            int tx = luaL_checkint(L, 2);
            int ty = luaL_checkint(L, 3);
            Tile t = m->getTile(tx, ty);

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
            int tx = luaL_checkint(L, 2);
            int ty = luaL_checkint(L, 3);
            Tile tileIndex = (Tile) luaL_checkint(L, 4);
            m->setTile(tx, ty, tileIndex);
            return 0;
        }

        int rect(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            int tx = luaL_checkint(L, 2);
            int ty = luaL_checkint(L, 3);
            int tx2 = luaL_checkint(L, 4);
            int ty2 = luaL_checkint(L, 5);
            Tile tileIndex = (Tile) luaL_checkint(L, 6);
            m->rect(tx, ty, tx2, ty2, tileIndex);
            return 0;
        }

        int solidRect(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            int tx = luaL_checkint(L, 2);
            int ty = luaL_checkint(L, 3);
            int tx2 = luaL_checkint(L, 4);
            int ty2 = luaL_checkint(L, 5);
            Tile tileIndex = (Tile) luaL_checkint(L, 6);
            m->solidRect(tx, ty, tx2, ty2, tileIndex);
            return 0;
        }

        int stamp(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            int tx = luaL_checkint(L, 2);
            int ty = luaL_checkint(L, 3);
            auto dest = script::ptr<Tilemap>(L, 4);
            m->stamp(tx, ty, dest);
            return 0;
        }

        int blit(lua_State* L)
        {
            auto m = script::ptr<Tilemap>(L, 1);
            int worldX = luaL_checkint(L, 2);
            int worldY = luaL_checkint(L, 3);
            int destX = luaL_checkint(L, 4);
            int destY = luaL_checkint(L, 5);
            int tilesWide = luaL_checkint(L, 6);
            int tilesHigh = luaL_checkint(L, 7);
            BlendMode mode = (BlendMode) luaL_optint(L, 8, BlendUnspecified);
            Color tint = luaL_optint(L, 9, Color::White);

            m->blit(worldX, worldY, destX, destY, tilesWide, tilesHigh, mode, tint);
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
                {"getspritesheet", getspritesheet},
                {"setspritesheet", setspritesheet},
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
            lua_pushstring(L, "Tilemap");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}
