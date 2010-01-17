#include "../plum.h"


namespace Plum
{
	namespace ScriptLibrary
	{
		namespace SpritesheetObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<Spritesheet>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Spritesheet>*, libraryName)

			int create(lua_State* L)
			{
				if((PLUM_IS_DATA(L, 1, Canvas) || PLUM_IS_DATA(L, 1, Texture))
					&& lua_isnumber(L, 2) && lua_isnumber(L, 3))
				{
					int w = lua_tointeger(L, 2);
					int h = lua_tointeger(L, 3);
					if(PLUM_IS_DATA(L, 1, Canvas))
					{
						Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
						PLUM_PUSH_DATA(L, Spritesheet, new Spritesheet(canvas->data, w, h), NULL);
						return 1;
					}
					else if(PLUM_IS_DATA(L, 1, Texture))
					{
						Wrapper<Texture>* tex = PLUM_CHECK_DATA(L, 1, Texture);
						PLUM_PUSH_DATA(L, Spritesheet, new Spritesheet(tex->data, w, h), NULL);
						return 1;
					}
				}
				luaL_error(L, "Attempt to call plum.Spritesheet constructor with invalid argument types.\r\nMust be (Texture tex, int frameWidth, int frameHeight) or (Canvas canvas, int frameWidth, int frameHeight).");
				return 0;
			}

			int gc(lua_State* L)
			{
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 1, Spritesheet);
				// Only delete if it doesn't belong to a parent of some sort.
				if(!spr->parentRef)
				{
					delete spr->data;
				}
				else
				{
					luaL_unref(L, LUA_REGISTRYINDEX, spr->parentRef);
				}
				return 0;
			}

			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, Spritesheet);
				lua_pushstring(L, "(plum.Spritesheet object)");
				return 1;
			}

			int blitFrame(lua_State* L)
			{
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 1, Spritesheet);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				int f = luaL_checkint(L, 4);
				BlendMode mode = (BlendMode) luaL_optint(L, 7, BlendUnspecified);
				Color tint = luaL_optint(L, 8, Color::White);

				spr->data->blitFrame(x, y, f, mode, tint);
				return 0;
			}

			int getframeWidth(lua_State* L)
			{
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 1, Spritesheet);
				lua_pushinteger(L, spr->data->frameWidth);
				return 1;
			}

			int setframeWidth(lua_State* L)
			{
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 1, Spritesheet);
				spr->data->frameWidth = luaL_checkint(L, 2);
				return 0;
			}

			int getframeHeight(lua_State* L)
			{
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 1, Spritesheet);
				lua_pushinteger(L, spr->data->frameHeight);
				return 1;
			}

			int setframeHeight(lua_State* L)
			{
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 1, Spritesheet);
				spr->data->frameHeight = luaL_checkint(L, 2);
				return 0;
			}

			int getpadding(lua_State* L)
			{
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 1, Spritesheet);
				lua_pushinteger(L, spr->data->padding);
				return 1;
			}

			int setpadding(lua_State* L)
			{
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 1, Spritesheet);
				spr->data->padding = luaL_checkint(L, 2);
				return 0;
			}

			int getcolumns(lua_State* L)
			{
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 1, Spritesheet);
				lua_pushinteger(L, spr->data->columns);
				return 1;
			}

			int setcolumns(lua_State* L)
			{
				Wrapper<Spritesheet>* spr = PLUM_CHECK_DATA(L, 1, Spritesheet);
				spr->data->columns = luaL_checkint(L, 2);
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
				PLUM_BIND_FUNC(blitFrame)
				PLUM_BIND_FUNC(getframeWidth)
				PLUM_BIND_FUNC(setframeWidth)
				PLUM_BIND_FUNC(getframeHeight)
				PLUM_BIND_FUNC(setframeHeight)
				PLUM_BIND_FUNC(getpadding)
				PLUM_BIND_FUNC(setpadding)
				PLUM_BIND_FUNC(getcolumns)
				PLUM_BIND_FUNC(setcolumns)
				PLUM_BIND_FUNC_END_NULL()

				lua_pop(L, 1);

				// Push plum namespace.
				lua_getglobal(L, "plum");

				// plum.texture = <function textureNew>
				lua_pushstring(L, "Spritesheet");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);
			}
		}
	}
}