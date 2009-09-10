#include "../plum.h"

namespace Plum
{
	namespace ScriptLibrary
	{
		namespace TextureObject
		{
			SCRIPT_OBJ_GETTER(index, Wrapper<Texture>*, libraryName)
			SCRIPT_OBJ_SETTER(newindex, Wrapper<Texture>*, libraryName)

			int create(lua_State* L)
			{
				if(lua_isstring(L, 1))
				{
					const char* filename = lua_tostring(L, 1);

					PLUM_PUSH_DATA(L, Texture, new Texture(filename), true);

					return 1;
				}
				else if(PLUM_IS_DATA(L, 1, Image))
				{
					Wrapper<Image>* img = PLUM_CHECK_DATA(L, 1, Image);
					PLUM_PUSH_DATA(L, Texture, new Texture(img->data), true);

					return 1;
				}
				luaL_error(L, "Attempt to call plum.Texture constructor with invalid argument types.\r\nMust be (string filename) or (plum.Image image).");
				return 0;
			}

			int gc(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);

				if(t->canDelete)
				{
					delete t->data;
					t->data = NULL;
					t->canDelete = false;
				}

				return 0;
			}

			int tostring(lua_State* L)
			{
				PLUM_CHECK_DATA(L, 1, Texture);
				lua_pushstring(L, "(plum.Texture object)");
				return 1;
			}

			int blit(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				BlendMode mode = (BlendMode) luaL_optint(L, 4, BlendUnspecified);
				Color tint = luaL_optint(L, 5, Color::White);

				t->data->blit(x, y, mode, tint);

				return 0;
			}

			int blitRegion(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				int sx = luaL_checkint(L, 2);
				int sy = luaL_checkint(L, 3);
				int sx2 = luaL_checkint(L, 4);
				int sy2 = luaL_checkint(L, 5);
				int dx = luaL_checkint(L, 6);
				int dy = luaL_checkint(L, 7);
				BlendMode mode = (BlendMode) luaL_optint(L, 8, BlendUnspecified);
				Color tint = luaL_optint(L, 9, Color::White);

				t->data->blitRegion(sx, sy, sx2, sy2, dx, dy, mode, tint);

				return 0;
			}

			int scaleBlit(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				int width = luaL_checkint(L, 4);
				int height = luaL_checkint(L, 5);
				BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);
				Color tint = luaL_optint(L, 7, Color::White);

				t->data->scaleBlit(x, y, width, height, mode, tint);

				return 0;
			}

			int scaleBlitRegion(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				int sx = luaL_checkint(L, 2);
				int sy = luaL_checkint(L, 3);
				int sx2 = luaL_checkint(L, 4);
				int sy2 = luaL_checkint(L, 5);
				int dx = luaL_checkint(L, 6);
				int dy = luaL_checkint(L, 7);
				int scaledWidth = luaL_checkint(L, 8);
				int scaledHeight = luaL_checkint(L, 9);
				BlendMode mode = (BlendMode) luaL_optint(L, 10, BlendUnspecified);
				Color tint = luaL_optint(L, 11, Color::White);

				t->data->scaleBlitRegion(sx, sy, sx2, sy2, dx, dy, scaledWidth, scaledHeight, mode, tint);

				return 0;
			}

			int rotateBlit(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				double angle = luaL_checknumber(L, 4);
				BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);
				Color tint = luaL_optint(L, 6, Color::White);

				t->data->rotateBlit(x, y, angle, mode, tint);

				return 0;
			}

			int rotateBlitRegion(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				int sx = luaL_checkint(L, 2);
				int sy = luaL_checkint(L, 3);
				int sx2 = luaL_checkint(L, 4);
				int sy2 = luaL_checkint(L, 5);
				int dx = luaL_checkint(L, 6);
				int dy = luaL_checkint(L, 7);
				double angle = luaL_checknumber(L, 8);
				BlendMode mode = (BlendMode) luaL_optint(L, 9, BlendUnspecified);
				Color tint = luaL_optint(L, 10, Color::White);

				t->data->rotateBlitRegion(sx, sy, sx2, sy2, dx, dy, angle, mode, tint);
				return 0;
			}

			int rotateScaleBlit(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				int x = luaL_checkint(L, 2);
				int y = luaL_checkint(L, 3);
				double angle = luaL_checknumber(L, 4);
				double scale = luaL_checknumber(L, 5);
				BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);
				Color tint = luaL_optint(L, 7, Color::White);

				t->data->rotateScaleBlit(x, y, angle, scale, mode, tint);

				return 0;
			}

			int rotateScaleBlitRegion(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				int sx = luaL_checkint(L, 2);
				int sy = luaL_checkint(L, 3);
				int sx2 = luaL_checkint(L, 4);
				int sy2 = luaL_checkint(L, 5);
				int dx = luaL_checkint(L, 6);
				int dy = luaL_checkint(L, 7);
				double angle = luaL_checknumber(L, 8);
				double scale = luaL_checknumber(L, 9);
				BlendMode mode = (BlendMode) luaL_optint(L, 10, BlendUnspecified);
				Color tint = luaL_optint(L, 11, Color::White);

				t->data->rotateScaleBlitRegion(sx, sy, sx2, sy2, dx, dy, angle, scale, mode, tint);
				return 0;
			}

			int refresh(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				t->data->refresh();

				return 1;
			}

			int getwidth(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				lua_pushnumber(L, t->data->getImageWidth());

				return 1;
			}

			int getheight(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture);
				lua_pushnumber(L, t->data->getImageHeight());

				return 1;
			}

			int getimage(lua_State* L)
			{
				Wrapper<Texture>* t = PLUM_CHECK_DATA(L, 1, Texture); 
				PLUM_PUSH_DATA(L, Image, t->data->getImage(), false);
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
				PLUM_BIND_FUNC(blitRegion)
				PLUM_BIND_FUNC(scaleBlit)
				PLUM_BIND_FUNC(scaleBlitRegion)
				PLUM_BIND_FUNC(rotateBlit)
				PLUM_BIND_FUNC(rotateBlitRegion)
				PLUM_BIND_FUNC(rotateScaleBlit)
				PLUM_BIND_FUNC(rotateScaleBlitRegion)
				PLUM_BIND_FUNC(refresh)
				PLUM_BIND_FUNC(getwidth)
				PLUM_BIND_FUNC(getheight)
				PLUM_BIND_FUNC(getimage)
				PLUM_BIND_FUNC_END_NULL()

				lua_pop(L, 1);
				
				// Push plum namespace.
				lua_getglobal(L, "plum");

				// plum.texture = <function textureNew>
				lua_pushstring(L, "Texture");
				lua_pushcfunction(L, create);
				lua_settable(L, -3);

				// Pop plum namespace.
				lua_pop(L, 1);
			}
		}
	}
}