#include "../plum.h"

namespace Plum
{
	namespace ScriptLibrary
	{
		namespace FontObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<Font>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Font>*, libraryName)

			int create(lua_State* L)
			{
				 if(lua_isstring(L, 1))
				{
					const char* filename = lua_tostring(L, 1);
					PLUM_PUSH_DATA(L, Font, new Font(filename), LUA_NOREF);

					return 1;
				}
				luaL_error(L, "Attempt to call plum.Font constructor with invalid argument types.\r\nMust be (string filename).");
				return 0;
			}

			int gc(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);

				// Only delete if it doesn't belong to a parent of some sort.
				if(font->parentRef != LUA_NOREF)
				{
					delete font->data;
				}
				else
				{
					luaL_unref(L, LUA_REGISTRYINDEX, font->parentRef);
				}

				return 0;
			}
	
			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, Canvas);
				lua_pushstring(L, "(plum.Font object)");
				return 1;
			}

			int print(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				const char* message = luaL_checkstring(L, 4);
				BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);
				Color tint = luaL_optint(L, 6, Color::White);

				font->data->print(x, y, message, mode, tint);
				return 0;
			}

			int printRight(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				const char* message = luaL_checkstring(L, 4);
				BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);
				Color tint = luaL_optint(L, 6, Color::White);

				font->data->printRight(x, y, message, mode, tint);
				return 0;
			}

			int printCenter(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				const char* message = luaL_checkstring(L, 4);
				BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);
				Color tint = luaL_optint(L, 6, Color::White);

				font->data->printCenter(x, y, message, mode, tint);
				return 0;
			}

			int enableVariableWidth(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);
				font->data->enableVariableWidth();
				return 0;
			}

			int lineWidth(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);
				const char* message = luaL_checkstring(L, 2);
				int lineIndex = luaL_checkint(L, 3);
                if(message)
                {
                    lua_pushinteger(L, font->data->lineWidth(message, lineIndex));
                }
                else
                {
				    lua_pushinteger(L, 0);
                }
				return 1;
			}

			int textWidth(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);
				const char* message = luaL_checkstring(L, 2);
				lua_pushinteger(L, font->data->textWidth(message));
				return 1;
			}

			int textHeight(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);
				const char* message = luaL_checkstring(L, 2);
				lua_pushinteger(L, font->data->textHeight(message));
				return 1;
			}

			int wrapText(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);
				const char* message = luaL_checkstring(L, 2);
				int lineLength = luaL_checkint(L, 3);
				lua_pushstring(L, font->data->wrapText(message, lineLength).c_str());
				return 1;
			}

			int getwidth(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);
				lua_pushinteger(L, font->data->width);
				return 1;
			}

			int getheight(lua_State* L)
			{
				Wrapper<Font>* font = PLUM_CHECK_DATA(L, 1, Font);
				lua_pushinteger(L, font->data->height);
				return 1;
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
				PLUM_BIND_FUNC(enableVariableWidth)
				PLUM_BIND_FUNC(textWidth)
				PLUM_BIND_FUNC(lineWidth)
				PLUM_BIND_FUNC(print)
				PLUM_BIND_FUNC(printRight)
				PLUM_BIND_FUNC(printCenter)
				PLUM_BIND_FUNC(wrapText)
				PLUM_BIND_FUNC(getwidth)
				PLUM_BIND_FUNC(getheight)
				PLUM_BIND_FUNC_END_NULL()

				lua_pop(L, 1);

				// Push plum namespace.
				lua_getglobal(L, "plum");

				// plum[classname] = create
				lua_pushstring(L, "Font");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);

			}
		}
	}
}