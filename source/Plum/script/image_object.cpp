#include "../plum.h"

namespace Plum
{
	namespace ScriptLibrary
	{
		namespace ImageObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<Image>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Image>*, libraryName)

			int create(lua_State* L)
			{
				if(lua_isstring(L, 1))
				{
					const char* filename = lua_tostring(L, 1);

					PLUM_PUSH_DATA(L, Image, new Image(filename), true);
					
					return 1;
				}
				else if(lua_isnumber(L, 1) && lua_isnumber(L, 2))
				{
					int w = lua_tointeger(L, 1);
					int h = lua_tointeger(L, 2);

					PLUM_PUSH_DATA(L, Image, new Image(w, h), true);
					
					return 1;
				}
				else
				{
					luaL_error(L, "Attempt to call plum.Image constructor with invalid argument types.\r\nMust be (string filename) or (int w, int h).");
					return 0;
				}
			}


			int gc(lua_State* L)
			{
				Wrapper<Image>* img = PLUM_CHECK_DATA(L, 1, Image);

				// Only delete if it doesn't belong to a texture of some sort.
				if(img->canDelete)
				{
					delete img->data;
				}

				return 0;
			}

			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, Image);
				lua_pushstring(L, "(plum.Image object)");
				return 1;
			}

			int blit(lua_State* L)
			{
				Wrapper<Image>* img = PLUM_CHECK_DATA(L, 1, Image);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				Wrapper<Image>* dest = PLUM_CHECK_DATA(L, 4, Image);
				BlendMode mode = (BlendMode) luaL_optint(L, 5, getBlendMode());
				
				switch(mode)
				{
					case BlendOpaque:
						img->data->blit<SoftOpaqueBlender>(x, y, dest->data, SoftOpaqueBlender());
						break;
					case BlendMerge:
						img->data->blit<SoftMergeBlender>(x, y, dest->data, SoftMergeBlender());
						break;
					case BlendPreserve:
						img->data->blit<SoftPreserveBlender>(x, y, dest->data, SoftPreserveBlender());
						break;
					case BlendAdd:
						img->data->blit<SoftAddBlender>(x, y, dest->data, SoftAddBlender());
						break;
					case BlendSubtract:
						img->data->blit<SoftSubtractBlender>(x, y, dest->data, SoftSubtractBlender());
						break;
				}
				return 0;
			}


			int replaceColor(lua_State* L)
			{
				Wrapper<Image>* img = PLUM_CHECK_DATA(L, 1, Image);
				int find = luaL_checkint(L, 2);
				int replace = luaL_checkint(L, 3);
				img->data->replaceColor(find, replace);
				return 0;
			}

			int flip(lua_State* L)
			{
				Wrapper<Image>* img = PLUM_CHECK_DATA(L, 1, Image);
				bool h = lua_toboolean(L, 2) != 0;
				bool v = lua_toboolean(L, 3) != 0;
				img->data->flip(h, v);
				return 0;
			}

			int getwidth(lua_State* L)
			{
				Wrapper<Image>* img = PLUM_CHECK_DATA(L, 1, Image);
				lua_pushnumber(L, img->data->width);

				return 1;
			}

			int getheight(lua_State* L)
			{
				Wrapper<Image>* img = PLUM_CHECK_DATA(L, 1, Image);
				lua_pushnumber(L, img->data->height);

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

				// plum.image = <function imageNew>
				lua_pushstring(L, "Image");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);

			}
		}
	}
}