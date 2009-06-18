#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static Font** CheckValidFontObject(lua_State* L, int index)
		{
			return (Font**) luaL_checkudata(L, index, "plum_font");
		}

		static int font_new(lua_State* L)
		{
			const char* filename = lua_tostring(L, 1);

			Font** f = (Font**) lua_newuserdata(L, sizeof(Font*));
			luaL_getmetatable(L, "plum_font");
			lua_setmetatable(L, -2);

			*f = new Font(filename);
			
			return 1;
		}

		static int font_gc(lua_State* L)
		{
			Font** f = CheckValidFontObject(L, 1);
			delete *f;

			return 0;
		}

		SCRIPT_OBJ_GETTER(font_getField, Font**, "plum_font")
		SCRIPT_OBJ_SETTER(font_setField, Font**, "plum_font")

		static int font_toString(lua_State* L)
		{
			CheckValidFontObject(L, 1);
			lua_pushstring(L, "(plum.Font object)");
			return 1;
		}

		static int font_print(lua_State* L)
		{
			Font** f = CheckValidFontObject(L, 1);
			int x = lua_tointeger(L, 2);
			int y = lua_tointeger(L, 3);
			const char* message = lua_tostring(L, 4);
			BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);
			Color tint = luaL_optint(L, 6, Color::White);

			(*f)->print(x, y, message, mode, tint);
			return 0;
		}

		static int font_enableVariableWidth(lua_State* L)
		{
			Font** f = CheckValidFontObject(L, 1);
			(*f)->enableVariableWidth();
			return 1;
		}

		static int font_getWidth(lua_State* L)
		{
			Font** f = CheckValidFontObject(L, 1);
			lua_pushnumber(L, (*f)->width);
			return 1;
		}

		static int font_getHeight(lua_State* L)
		{
			Font** f = CheckValidFontObject(L, 1);
			lua_pushnumber(L, (*f)->height);
			return 1;
		}

		const luaL_Reg fontMembers[] = {
			{ "__index", font_getField },
			{ "__newindex",	font_setField },
			{ "__tostring",	font_toString },
			{ "__gc", font_gc },
			{ "print", font_print },
			{ "enableVariableWidth", font_enableVariableWidth },
			{ "getwidth", font_getWidth },
			{ "getheight", font_getHeight },
			{ NULL, NULL }
		};

		void initFontClass(lua_State* L)
		{
			luaL_newmetatable(L, "plum_font");
			// Duplicate the metatable on the stack.
			lua_pushvalue(L, -1);
			// metatable.__index = metatable
			lua_setfield(L, -2, "__index");
			// Put the members into the metatable.
			luaL_register(L, NULL, fontMembers);
			lua_pop(L, 1);

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// plum.texture = <function font_new>
			lua_pushstring(L, "Font");
			lua_pushcfunction(L, font_new);
			lua_settable(L, -3);

			// Pop plum namespace.
			lua_pop(L, -1);
		}
	}
}