#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static Texture** CheckValidTextureObject(lua_State* L, int index)
		{
			return (Texture**) luaL_checkudata(L, index, "plum_texture");
		}

		static int texture_new(lua_State* L)
		{
			const char* filename = lua_tostring(L, 1);

			Texture** t = (Texture**) lua_newuserdata(L, sizeof(Texture*));
			luaL_getmetatable(L, "plum_texture");
			lua_setmetatable(L, -2);

			*t = new Texture(filename);
			
			return 1;
		}

		static int texture_gc(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			delete *t;

			return 0;
		}

		SCRIPT_OBJ_GETTER(texture_getField, Texture**, "plum_texture")
		SCRIPT_OBJ_SETTER(texture_setField, Texture**, "plum_texture")

		static int texture_toString(lua_State* L)
		{
			CheckValidTextureObject(L, 1);
			lua_pushstring(L, "(plum.Texture object)");
			return 1;
		}

		static int texture_blit(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			int x = luaL_checkint(L, 2);
			int y = luaL_checkint(L, 3);
			BlendMode mode = (BlendMode) luaL_optint(L, 4, BlendUnspecified);
			Color tint = luaL_optint(L, 5, Color::White);

			(*t)->blit(x, y, mode, tint);

			return 0;
		}

		static int texture_blitRegion(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			int sx = luaL_checkint(L, 2);
			int sy = luaL_checkint(L, 3);
			int sx2 = luaL_checkint(L, 4);
			int sy2 = luaL_checkint(L, 5);
			int dx = luaL_checkint(L, 6);
			int dy = luaL_checkint(L, 7);
			BlendMode mode = (BlendMode) luaL_optint(L, 8, BlendUnspecified);
			Color tint = luaL_optint(L, 9, Color::White);

			(*t)->blitRegion(sx, sy, sx2, sy2, dx, dy, mode, tint);

			return 0;
		}

		static int texture_scaleBlit(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			int x = luaL_checkint(L, 2);
			int y = luaL_checkint(L, 3);
			int width = luaL_checkint(L, 4);
			int height = luaL_checkint(L, 5);
			BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);
			Color tint = luaL_optint(L, 7, Color::White);

			(*t)->scaleBlit(x, y, width, height, mode, tint);

			return 0;
		}

		static int texture_scaleBlitRegion(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
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

			(*t)->scaleBlitRegion(sx, sy, sx2, sy2, dx, dy, scaledWidth, scaledHeight, mode, tint);

			return 0;
		}

		static int texture_rotateBlit(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			int x = luaL_checkint(L, 2);
			int y = luaL_checkint(L, 3);
			double angle = luaL_checknumber(L, 4);
			BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);
			Color tint = luaL_optint(L, 6, Color::White);

			(*t)->rotateBlit(x, y, angle, mode, tint);

			return 0;
		}


		static int texture_rotateBlitRegion(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			int sx = luaL_checkint(L, 2);
			int sy = luaL_checkint(L, 3);
			int sx2 = luaL_checkint(L, 4);
			int sy2 = luaL_checkint(L, 5);
			int dx = luaL_checkint(L, 6);
			int dy = luaL_checkint(L, 7);
			double angle = luaL_checknumber(L, 8);
			BlendMode mode = (BlendMode) luaL_optint(L, 9, BlendUnspecified);
			Color tint = luaL_optint(L, 10, Color::White);

			(*t)->rotateBlitRegion(sx, sy, sx2, sy2, dx, dy, angle, mode, tint);
			return 0;
		}

		static int texture_rotateScaleBlit(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			int x = luaL_checkint(L, 2);
			int y = luaL_checkint(L, 3);
			double angle = luaL_checknumber(L, 4);
			double scale = luaL_checknumber(L, 5);
			BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);
			Color tint = luaL_optint(L, 7, Color::White);

			(*t)->rotateScaleBlit(x, y, angle, scale, mode, tint);

			return 0;
		}

		static int texture_rotateScaleBlitRegion(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
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

			(*t)->rotateScaleBlitRegion(sx, sy, sx2, sy2, dx, dy, angle, scale, mode, tint);
			return 0;
		}

		static int texture_refresh(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			(*t)->refresh();

			return 1;
		}

		static int texture_getWidth(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			lua_pushnumber(L, (*t)->getImageWidth());

			return 1;
		}

		static int texture_getHeight(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			lua_pushnumber(L, (*t)->getImageHeight());

			return 1;
		}

		static int texture_getImage(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1); 
			image_pushForTexture(L, *t);
			return 1;
		}

		const luaL_Reg textureMembers[] = {
			{ "__index", texture_getField },
			{ "__newindex",	texture_setField },
			{ "__tostring",	texture_toString },
			{ "__gc", texture_gc },
			{ "blit", texture_blit },
			{ "blitRegion", texture_blitRegion },
			{ "scaleBlit", texture_scaleBlit },
			{ "scaleBlitRegion", texture_scaleBlitRegion },
			{ "rotateBlit", texture_rotateBlit },
			{ "rotateBlitRegion", texture_rotateBlitRegion },
			{ "rotateScaleBlit", texture_rotateScaleBlit },
			{ "rotateScaleBlitRegion", texture_rotateScaleBlitRegion },
			{ "refresh", texture_refresh },
			{ "getwidth", texture_getWidth },
			{ "getheight", texture_getHeight },
			{ "getimage", texture_getImage },
			{ NULL, NULL }
		};

		void initTextureClass(lua_State* L)
		{
			luaL_newmetatable(L, "plum_texture");
			// Duplicate the metatable on the stack.
			lua_pushvalue(L, -1);
			// metatable.__index = metatable
			lua_setfield(L, -2, "__index");
			// Put the members into the metatable.
			luaL_register(L, NULL, textureMembers);
			lua_pop(L, 1);

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// plum.texture = <function texture_new>
			lua_pushstring(L, "Texture");
			lua_pushcfunction(L, texture_new);
			lua_settable(L, -3);

			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}