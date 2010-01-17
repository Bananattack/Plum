#include "../plum.h"

namespace Plum
{
	namespace ScriptLibrary
	{
		namespace CanvasObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<Canvas>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Canvas>*, libraryName)

			int create(lua_State* L)
			{
				if(lua_isnumber(L, 1) && lua_isnumber(L, 2))
				{
					int w = lua_tointeger(L, 1);
					int h = lua_tointeger(L, 2);

					PLUM_PUSH_DATA(L, Canvas, new Canvas(w, h), NULL);
					
					return 1;
				}
				else if(lua_isstring(L, 1))
				{
					const char* filename = lua_tostring(L, 1);

					PLUM_PUSH_DATA(L, Canvas, new Canvas(filename), NULL);
					
					return 1;
				}
				else
				{
					luaL_error(L, "Attempt to call plum.Canvas constructor with invalid argument types.\r\nMust be (string filename) or (int w, int h).");
					return 0;
				}
			}


			int gc(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);

				// Only delete if it doesn't belong to a parent of some sort.
				if(!canvas->parentRef)
				{
					delete canvas->data;
				}
				else
				{
					luaL_unref(L, LUA_REGISTRYINDEX, canvas->parentRef);
				}

				return 0;
			}

			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, Canvas);
				lua_pushstring(L, "(plum.Canvas object)");
				return 1;
			}

			int blit(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				Wrapper<Canvas>* dest = PLUM_CHECK_DATA(L, 4, Canvas);
				BlendMode mode = (BlendMode) luaL_optint(L, 5, getBlendMode());
				
				switch(mode)
				{
					case BlendOpaque:
						canvas->data->blit<SoftOpaqueBlender>(x, y, dest->data, SoftOpaqueBlender());
						break;
					case BlendMerge:
						canvas->data->blit<SoftMergeBlender>(x, y, dest->data, SoftMergeBlender());
						break;
					case BlendPreserve:
						canvas->data->blit<SoftPreserveBlender>(x, y, dest->data, SoftPreserveBlender());
						break;
					case BlendAdd:
						canvas->data->blit<SoftAddBlender>(x, y, dest->data, SoftAddBlender());
						break;
					case BlendSubtract:
						canvas->data->blit<SoftSubtractBlender>(x, y, dest->data, SoftSubtractBlender());
						break;
				}
				return 0;
			}


			int replaceColor(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				int find = luaL_checkint(L, 2);
				int replace = luaL_checkint(L, 3);
				canvas->data->replaceColor(find, replace);
				return 0;
			}

			int flip(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				bool h = lua_toboolean(L, 2) != 0;
				bool v = lua_toboolean(L, 3) != 0;
				canvas->data->flip(h, v);
				return 0;
			}

			int getwidth(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				lua_pushnumber(L, canvas->data->width);

				return 1;
			}

			int getheight(lua_State* L)
			{
				Wrapper<Canvas>* canvas = PLUM_CHECK_DATA(L, 1, Canvas);
				lua_pushnumber(L, canvas->data->height);

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
				PLUM_BIND_FUNC(blit)
				PLUM_BIND_FUNC(flip)
				PLUM_BIND_FUNC(replaceColor)
				PLUM_BIND_FUNC(getwidth)
				PLUM_BIND_FUNC(getheight)
				PLUM_BIND_FUNC_END_NULL()

				lua_pop(L, 1);

				// Push plum namespace.
				lua_getglobal(L, "plum");

				// plum.canvas = <function imageNew>
				lua_pushstring(L, "Canvas");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);

			}
		}
	}
}