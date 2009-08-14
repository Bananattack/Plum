#include "../plum.h"

namespace Plum
{
	Script::TextureWrapper* Script::checkValidTextureObject(lua_State* L, int index)
	{
		return (Script::TextureWrapper*) luaL_checkudata(L, index, "plum_texture");
	}

	static int textureNew(lua_State* L)
	{
		if(lua_isstring(L, 1))
		{
			const char* filename = lua_tostring(L, 1);

			Script::TextureWrapper* t = (Script::TextureWrapper*) lua_newuserdata(L, sizeof(Script::TextureWrapper));
			luaL_getmetatable(L, "plum_texture");
			lua_setmetatable(L, -2);

			t->canDelete = true;
			t->texture = new Texture(filename);

			return 1;
		}
		else if (lua_isuserdata(L, 1))
		{
			void* data = lua_touserdata(L, 1);
			if(lua_getmetatable(L, 1))
			{
				lua_getfield(L, LUA_REGISTRYINDEX, "plum_image");
				// Creating a texture from an image?
				if (lua_rawequal(L, -1, -2))
				{
					lua_pop(L, 2);

					Script::TextureWrapper* t = (Script::TextureWrapper*) lua_newuserdata(L, sizeof(Script::TextureWrapper));
					luaL_getmetatable(L, "plum_texture");
					lua_setmetatable(L, -2);

					t->canDelete = true;
					t->texture = new Texture(((Script::ImageWrapper*) data)->image);

					return 1;
				}
			}
		}
		luaL_error(L, "Attempt to call plum.Texture constructor with invalid argument types.\r\nMust be (string filename) or (plum.Image image).");
		return 0;
	}

	void Script::pushTextureRef(lua_State* L, Texture* tex)
	{
		Script::TextureWrapper* t = (Script::TextureWrapper*) lua_newuserdata(L, sizeof(Script::TextureWrapper));
		luaL_getmetatable(L, "plum_texture");
		lua_setmetatable(L, -2);

		t->texture = tex;
		t->canDelete = false;
	}

	static int textureGC(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);

		if(t->canDelete)
		{
			delete t->texture;
			t->texture = NULL;
			t->canDelete = false;
		}

		return 0;
	}

	SCRIPT_OBJ_GETTER(textureGetField, Script::TextureWrapper*, "plum_texture")
	SCRIPT_OBJ_SETTER(textureSetField, Script::TextureWrapper*, "plum_texture")

	static int textureToString(lua_State* L)
	{
		Script::checkValidTextureObject(L, 1);
		lua_pushstring(L, "(plum.Texture object)");
		return 1;
	}

	static int textureBlit(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
		int x = luaL_checkint(L, 2);
		int y = luaL_checkint(L, 3);
		BlendMode mode = (BlendMode) luaL_optint(L, 4, BlendUnspecified);
		Color tint = luaL_optint(L, 5, Color::White);

		t->texture->blit(x, y, mode, tint);

		return 0;
	}

	static int textureBlitRegion(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
		int sx = luaL_checkint(L, 2);
		int sy = luaL_checkint(L, 3);
		int sx2 = luaL_checkint(L, 4);
		int sy2 = luaL_checkint(L, 5);
		int dx = luaL_checkint(L, 6);
		int dy = luaL_checkint(L, 7);
		BlendMode mode = (BlendMode) luaL_optint(L, 8, BlendUnspecified);
		Color tint = luaL_optint(L, 9, Color::White);

		t->texture->blitRegion(sx, sy, sx2, sy2, dx, dy, mode, tint);

		return 0;
	}

	static int textureScaleBlit(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
		int x = luaL_checkint(L, 2);
		int y = luaL_checkint(L, 3);
		int width = luaL_checkint(L, 4);
		int height = luaL_checkint(L, 5);
		BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);
		Color tint = luaL_optint(L, 7, Color::White);

		t->texture->scaleBlit(x, y, width, height, mode, tint);

		return 0;
	}

	static int textureScaleBlitRegion(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
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

		t->texture->scaleBlitRegion(sx, sy, sx2, sy2, dx, dy, scaledWidth, scaledHeight, mode, tint);

		return 0;
	}

	static int textureRotateBlit(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
		int x = luaL_checkint(L, 2);
		int y = luaL_checkint(L, 3);
		double angle = luaL_checknumber(L, 4);
		BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);
		Color tint = luaL_optint(L, 6, Color::White);

		t->texture->rotateBlit(x, y, angle, mode, tint);

		return 0;
	}


	static int textureRotateBlitRegion(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
		int sx = luaL_checkint(L, 2);
		int sy = luaL_checkint(L, 3);
		int sx2 = luaL_checkint(L, 4);
		int sy2 = luaL_checkint(L, 5);
		int dx = luaL_checkint(L, 6);
		int dy = luaL_checkint(L, 7);
		double angle = luaL_checknumber(L, 8);
		BlendMode mode = (BlendMode) luaL_optint(L, 9, BlendUnspecified);
		Color tint = luaL_optint(L, 10, Color::White);

		t->texture->rotateBlitRegion(sx, sy, sx2, sy2, dx, dy, angle, mode, tint);
		return 0;
	}

	static int textureRotateScaleBlit(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
		int x = luaL_checkint(L, 2);
		int y = luaL_checkint(L, 3);
		double angle = luaL_checknumber(L, 4);
		double scale = luaL_checknumber(L, 5);
		BlendMode mode = (BlendMode) luaL_optint(L, 6, BlendUnspecified);
		Color tint = luaL_optint(L, 7, Color::White);

		t->texture->rotateScaleBlit(x, y, angle, scale, mode, tint);

		return 0;
	}

	static int textureRotateScaleBlitRegion(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
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

		t->texture->rotateScaleBlitRegion(sx, sy, sx2, sy2, dx, dy, angle, scale, mode, tint);
		return 0;
	}

	static int textureRefresh(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
		t->texture->refresh();

		return 1;
	}

	static int textureGetWidth(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
		lua_pushnumber(L, t->texture->getImageWidth());

		return 1;
	}

	static int textureGetHeight(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1);
		lua_pushnumber(L, t->texture->getImageHeight());

		return 1;
	}

	static int textureGetImage(lua_State* L)
	{
		Script::TextureWrapper* t = Script::checkValidTextureObject(L, 1); 
		Script::pushImageRef(L, t->texture->getImage());
		return 1;
	}

	static const luaL_Reg textureMembers[] = {
		{ "__index", textureGetField },
		{ "__newindex",	textureSetField },
		{ "__tostring",	textureToString },
		{ "__gc", textureGC },
		{ "blit", textureBlit },
		{ "blitRegion", textureBlitRegion },
		{ "scaleBlit", textureScaleBlit },
		{ "scaleBlitRegion", textureScaleBlitRegion },
		{ "rotateBlit", textureRotateBlit },
		{ "rotateBlitRegion", textureRotateBlitRegion },
		{ "rotateScaleBlit", textureRotateScaleBlit },
		{ "rotateScaleBlitRegion", textureRotateScaleBlitRegion },
		{ "refresh", textureRefresh },
		{ "getwidth", textureGetWidth },
		{ "getheight", textureGetHeight },
		{ "getimage", textureGetImage },
		{ NULL, NULL }
	};

	void Script::initTextureClass(lua_State* L)
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

		// plum.texture = <function textureNew>
		lua_pushstring(L, "Texture");
		lua_pushcfunction(L, textureNew);
		lua_settable(L, -3);

		// Pop plum namespace.
		lua_pop(L, 1);
	}
}