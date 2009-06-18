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
			int x = lua_tointeger(L, 2);
			int y = lua_tointeger(L, 3);
			BlendMode mode = (BlendMode) luaL_optint(L, 4, BlendUnspecified);
			Color tint = luaL_optint(L, 5, Color::White);

			(*t)->blit(x, y, mode, tint);

			return 0;
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

		const luaL_Reg textureMembers[] = {
			{ "__index", texture_getField },
			{ "__newindex",	texture_setField },
			{ "__tostring",	texture_toString },
			{ "__gc", texture_gc },
			{ "blit", texture_blit },
			{ "getwidth", texture_getWidth },
			{ "getheight", texture_getHeight },
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
			lua_pop(L, -1);
		}
	}
}