#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static Font** checkValidFontObject(lua_State* L, int index)
		{
			return (Font**) luaL_checkudata(L, index, "plum_font");
		}

		static int fontNew(lua_State* L)
		{
			const char* filename = lua_tostring(L, 1);

			Font** f = (Font**) lua_newuserdata(L, sizeof(Font*));
			luaL_getmetatable(L, "plum_font");
			lua_setmetatable(L, -2);

			*f = new Font(filename);
			
			return 1;
		}

		static int fontGC(lua_State* L)
		{
			Font** f = checkValidFontObject(L, 1);
			delete *f;

			return 0;
		}

		SCRIPT_OBJ_GETTER(fontGetField, Font**, "plum_font")
		SCRIPT_OBJ_SETTER(fontSetField, Font**, "plum_font")

		static int fontToString(lua_State* L)
		{
			checkValidFontObject(L, 1);
			lua_pushstring(L, "(plum.Font object)");
			return 1;
		}

		static int fontPrint(lua_State* L)
		{
			Font** f = checkValidFontObject(L, 1);
			int x = luaL_checkint(L, 2);
			int y = luaL_checkint(L, 3);
			const char* message = lua_tostring(L, 4);
			BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);
			Color tint = luaL_optint(L, 6, Color::White);

			(*f)->print(x, y, message, mode, tint);
			return 0;
		}

		static int fontPrintCenter(lua_State* L)
		{
			Font** f = checkValidFontObject(L, 1);
			int x = luaL_checkint(L, 2);
			int y = luaL_checkint(L, 3);
			const char* message = lua_tostring(L, 4);
			BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);
			Color tint = luaL_optint(L, 6, Color::White);

			(*f)->printCenter(x, y, message, mode, tint);
			return 0;
		}

		static int fontPrintRight(lua_State* L)
		{
			Font** f = checkValidFontObject(L, 1);
			int x = luaL_checkint(L, 2);
			int y = luaL_checkint(L, 3);
			const char* message = lua_tostring(L, 4);
			BlendMode mode = (BlendMode) luaL_optint(L, 5, BlendUnspecified);
			Color tint = luaL_optint(L, 6, Color::White);

			(*f)->printRight(x, y, message, mode, tint);
			return 0;
		}


		static int fontEnableVariableWidth(lua_State* L)
		{
			Font** f = checkValidFontObject(L, 1);
			(*f)->enableVariableWidth();
			return 0;
		}

		static int fontLineWidth(lua_State* L)
		{
			Font** f = checkValidFontObject(L, 1);
			const char* message = luaL_checkstring(L, 2);
			int lineIndex = luaL_checkstring(L, 3);
			lua_pushinteger(L, (*f)->lineWidth(message, lineIndex));
			return 1;
		}

		static int fontTextWidth(lua_State* L)
		{
			Font** f = checkValidFontObject(L, 1);
			const char* message = luaL_checkstring(L, 2);
			lua_pushinteger(L, (*f)->textWidth(message));
			return 1;
		}

		static int fontTextHeight(lua_State* L)
		{
			Font** f = checkValidFontObject(L, 1);
			const char* message = luaL_checkstring(L, 2);
			lua_pushinteger(L, (*f)->textHeight(message));
			return 1;
		}

		static int fontGetWidth(lua_State* L)
		{
			Font** f = checkValidFontObject(L, 1);
			lua_pushnumber(L, (*f)->width);
			return 1;
		}

		static int fontGetHeight(lua_State* L)
		{
			Font** f = checkValidFontObject(L, 1);
			lua_pushnumber(L, (*f)->height);
			return 1;
		}

		const luaL_Reg fontMembers[] = {
			{ "__index", fontGetField },
			{ "__newindex",	fontSetField },
			{ "__tostring",	fontToString },
			{ "__gc", fontGC },
			{ "print", fontPrint },
			{ "printCenter", fontPrintCenter },
			{ "printRight", fontPrintRight },
			{ "enableVariableWidth", fontEnableVariableWidth },
			{ "lineWidth", fontLineWidth },
			{ "textWidth", fontTextWidth },
			{ "textHeight", fontTextHeight },
			{ "getwidth", fontGetWidth },
			{ "getheight", fontGetHeight },
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

			// plum.font = <function fontNew>
			lua_pushstring(L, "Font");
			lua_pushcfunction(L, fontNew);
			lua_settable(L, -3);

			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}