#include "../plum.h"


namespace Plum
{
	namespace ScriptLibrary
	{
		namespace TilemapObject
		{
			const int TILEMAP_SPRITESHEET_REF = 1;

			SCRIPT_OBJ_GETTER(index, Wrapper<Tilemap>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Tilemap>*, libraryName)

			int create(lua_State* L)
			{
				if(lua_isnumber(L, 1) && lua_isnumber(L, 2))
				{
					int w = lua_tointeger(L, 1);
					int h = lua_tointeger(L, 2);
					PLUM_PUSH_DATA(L, Tilemap, new Tilemap(w, h), LUA_NOREF);

					return 1;
				}
				luaL_error(L, "Attempt to call plum.Tilemap constructor with invalid argument types.\r\nMust be (int w, int h).");
				return 0;
			}

			int gc(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);

				// Discard ext table.
				luaL_unref(L, LUA_REGISTRYINDEX, m->extRef);

				// Only delete if it doesn't belong to a parent of some sort.
				if(m->parentRef == LUA_NOREF)
				{
					delete m->data;
				}
				else
				{
					luaL_unref(L, LUA_REGISTRYINDEX, m->parentRef);
				}
				return 0;
			}

			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, Tilemap);
				lua_pushstring(L, "(plum.Tilemap object)");
				return 1;
			}

			int getspritesheet(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);

				// Push ext table.
				PLUM_GET_EXT(L, m, Tilemap);
				// Push value.
				lua_rawgeti(L, -1, TILEMAP_SPRITESHEET_REF);
				// Remove ext from stack, while keeping value on top of the stack.
				lua_remove(L, -2);

				return 1;
			}

			int setspritesheet(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				Wrapper<Spritesheet>* spritesheet = PLUM_CHECK_DATA(L, 2, Spritesheet);

				// Change the spritesheet
				m->data->spr = spritesheet->data;

				// Update reference table, so the value will remain in memory as long as it's required.
				// Push ext table.
				PLUM_GET_EXT(L, m, Tilemap);
				// Push the value.
				lua_pushvalue(L, 2);
				// Set value.
				lua_rawseti(L, -2, TILEMAP_SPRITESHEET_REF);
				// Pop ext from stack.
				lua_pop(L, 1);

				return 0;
			}

			int getwidth(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				lua_pushinteger(L, m->data->getWidth());
				return 1;
			}

			int getheight(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				lua_pushinteger(L, m->data->getHeight());
				return 1;
			}

			int getTile(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				int tx = luaL_checkint(L, 2);
				int ty = luaL_checkint(L, 3);
				lua_pushinteger(L, m->data->getTile(tx, ty));
				return 1;
			}

			int setTile(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				int tx = luaL_checkint(L, 2);
				int ty = luaL_checkint(L, 3);
				Tile tileIndex = (Tile) luaL_checkint(L, 4);
				m->data->setTile(tx, ty, tileIndex);
				return 0;
			}

			int rect(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				int tx = luaL_checkint(L, 2);
				int ty = luaL_checkint(L, 3);
				int tx2 = luaL_checkint(L, 4);
				int ty2 = luaL_checkint(L, 5);
				Tile tileIndex = (Tile) luaL_checkint(L, 6);
				m->data->rect(tx, ty, tx2, ty2, tileIndex);
				return 0;
			}

			int solidRect(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				int tx = luaL_checkint(L, 2);
				int ty = luaL_checkint(L, 3);
				int tx2 = luaL_checkint(L, 4);
				int ty2 = luaL_checkint(L, 5);
				Tile tileIndex = (Tile) luaL_checkint(L, 6);
				m->data->solidRect(tx, ty, tx2, ty2, tileIndex);
				return 0;
			}

			int stamp(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				int tx = luaL_checkint(L, 2);
				int ty = luaL_checkint(L, 3);
				Wrapper<Tilemap>* dest = PLUM_CHECK_DATA(L, 4, Tilemap);
				m->data->stamp(tx, ty, dest->data);
				return 0;
			}

			int blit(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				int worldX = luaL_checkint(L, 2);
				int worldY = luaL_checkint(L, 3);
				int destX = luaL_checkint(L, 4);
				int destY = luaL_checkint(L, 5);
				int tilesWide = luaL_checkint(L, 6);
				int tilesHigh = luaL_checkint(L, 7);
				BlendMode mode = (BlendMode) luaL_optint(L, 8, BlendUnspecified);
				Color tint = luaL_optint(L, 9, Color::White);

				m->data->blit(worldX, worldY, destX, destY, tilesWide, tilesHigh, mode, tint);
				return 0;
			}

			void openLibrary(lua_State* L)
			{
				luaL_newmetatable(L, libraryName);
				// Duplicate the metatable on the stack.
				lua_pushvalue(L, -1);
				// metatable.__index = metatable
				lua_setfield(L, -2, "__index");

				// Put the members into the metatable.
				PLUM_BIND_FUNC_BEGIN()
				PLUM_BIND_META(gc)
				PLUM_BIND_META(index)
				PLUM_BIND_META(newindex)
				PLUM_BIND_META(tostring)
				PLUM_BIND_FUNC(getspritesheet)
				PLUM_BIND_FUNC(setspritesheet)
				PLUM_BIND_FUNC(getwidth)
				PLUM_BIND_FUNC(getheight)
				PLUM_BIND_FUNC(getTile)
				PLUM_BIND_FUNC(setTile)
				PLUM_BIND_FUNC(rect)
				PLUM_BIND_FUNC(solidRect)
				PLUM_BIND_FUNC(stamp)
				PLUM_BIND_FUNC(blit)
				PLUM_BIND_FUNC_END_NULL()

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
}
