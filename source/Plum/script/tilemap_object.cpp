#include "../plum.h"


namespace Plum
{
	namespace ScriptLibrary
	{
		namespace TilemapObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<Tilemap>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Tilemap>*, libraryName)

			int create(lua_State* L)
			{
				if(lua_isnumber(L, 1) && lua_isnumber(L, 2))
				{
					int w = lua_tointeger(L, 1);
					int h = lua_tointeger(L, 2);
					PLUM_PUSH_DATA(L, Tilemap, new Tilemap(w, h), true);
					return 1;
				}
				luaL_error(L, "Attempt to call plum.Tilemap constructor with invalid argument types.\r\nMust be (int w, int h).");
				return 0;
			}

			int gc(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				// Only delete if it doesn't belong to a texture of some sort.
				if(m->canDelete)
				{
					delete m->data;
				}
				return 0;
			}

			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, Tilemap);
				lua_pushstring(L, "(plum.Tilemap object)");
				return 1;
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
				int tileIndex = luaL_checkint(L, 4);
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
				int tileIndex = luaL_checkint(L, 6);
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
				int tileIndex = luaL_checkint(L, 6);
				m->data->solidRect(tx, ty, tx2, ty2, tileIndex);
				return 0;
			}

			int stamp(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				int tx = luaL_checkint(L, 2);
				int ty = luaL_checkint(L, 3);
				Wrapper<Tilemap>* dest = PLUM_CHECK_DATA(L, 4, Tilemap);
				m->data->stamp(tx, tx, dest->data);
				return 0;
			}

			int blit(lua_State* L)
			{
				Wrapper<Tilemap>* m = PLUM_CHECK_DATA(L, 1, Tilemap);
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 2, Spritesheet);
				int worldX = luaL_checkint(L, 3);
				int worldY = luaL_checkint(L, 4);
				int destX = luaL_checkint(L, 5);
				int destY = luaL_checkint(L, 6);
				int tilesWide = luaL_checkint(L, 7);
				int tilesHigh = luaL_checkint(L, 8);
				BlendMode mode = (BlendMode) luaL_optint(L, 9, BlendUnspecified);
				Color tint = luaL_optint(L, 10, Color::White);

				m->data->blit(spr->data, worldX, worldY, destX, destY, tilesWide, tilesHigh, mode, tint);
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

				// plum.texture = <function textureNew>
				lua_pushstring(L, "Tilemap");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);
			}
		}
	}
}