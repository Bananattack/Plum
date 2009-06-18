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

		static int texture_getField(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			std::string fieldName = luaL_checkstring(L, 2);
			
			if(luaL_getmetafield(L, -2, std::string("get" + fieldName).c_str()))
			{
				lua_pushvalue(L, -3);
				lua_call(L, 1, 1);
				return 1;
			}
			return luaL_getmetafield(L, -2, fieldName.c_str());
		}

		static int texture_setField(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			std::string fieldName = luaL_checkstring(L, 2);
			// L, 3 is the value to set.
			
			if(luaL_getmetafield(L, -3, std::string("set" + fieldName).c_str()))
			{
				lua_pushvalue(L, -4);
				lua_pushvalue(L, -2);
				lua_call(L, 2, 0);
				return 0;
			}
			return 0;
		}

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
			int mode = lua_tointeger(L, 4);
			Color tint = (unsigned int) lua_tointeger(L, 5);

			(*t)->blit(x, y, (Plum::BlendMode) mode, tint);

			return 0;
		}

		static int texture_getwidth(lua_State* L)
		{
			Texture** t = CheckValidTextureObject(L, 1);
			lua_pushnumber(L, (*t)->getImageWidth());

			return 1;
		}

		static int texture_getheight(lua_State* L)
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
			{ "getwidth", texture_getwidth },
			{ "getheight", texture_getheight },
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