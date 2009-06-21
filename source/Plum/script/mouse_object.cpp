#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static int mouseGetField(lua_State* L)
		{
			std::string fieldName = luaL_checkstring(L, 2);
			if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str()))
			{
				lua_pushvalue(L, 1);
				lua_call(L, 1, 1);
				return 1;
			}
			return luaL_getmetafield(L, 1, fieldName.c_str());
		}

		static int mouseSetField(lua_State* L)
		{
			std::string fieldName = luaL_checkstring(L, 2);
			/* L, 3 is the value to set. */
			if(luaL_getmetafield(L, 1, std::string("set" + fieldName).c_str()))
			{
				lua_pushvalue(L, 1);
				lua_pushvalue(L, 3);
				lua_call(L, 2, 0);
				return 0;
			}
			if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str()))
			{
				luaL_error(L, "Attempt to modify readonly field '%s' on plum_mouse.", fieldName.c_str());
				lua_pop(L, 1);
				return 0;
			}
			luaL_error(L, "Attempt to modify unknown field '%s' on plum_mouse.", fieldName.c_str());
			return 0;
		}

		static int mouseToString(lua_State* L)
		{
			lua_pushstring(L, "(plum.mouse singleton)");
			return 1;
		}

		static int mouseGetX(lua_State* L)
		{
			lua_pushnumber(L, (*engine).mouse.x);
			return 1;
		}

		static int mouseGetY(lua_State* L)
		{
			lua_pushnumber(L, (*engine).mouse.y);
			return 1;
		}

		static int mouseGetWheelPosition(lua_State* L)
		{
			lua_pushinteger(L, (*engine).mouse.wheelPosition);
			return 1;
		}

		static int mouseSetWheelPosition(lua_State* L)
		{
			int value = luaL_checkint(L, 2);
			(*engine).mouse.wheelPosition = value;
			return 0;
		}

		static int mouseGetLeft(lua_State* L)
		{
			lua_getglobal(L, "plum");
			lua_getfield(L, -1, "_Input");
			lua_CFunction inputConstructor = lua_tocfunction(L, -1);
			lua_pop(L, 2);

			if(inputConstructor)
			{
				lua_pushcfunction(L, inputConstructor);
				lua_pushlightuserdata(L, &((*engine).mouse.button[MOUSE_LEFT]));
				lua_call(L, 1, 1);
				return 1;
			}

			return 0;
		}

		static int mouseGetMiddle(lua_State* L)
		{
			lua_getglobal(L, "plum");
			lua_getfield(L, -1, "_Input");
			lua_CFunction inputConstructor = lua_tocfunction(L, -1);
			lua_pop(L, 2);

			if(inputConstructor)
			{
				lua_pushcfunction(L, inputConstructor);
				lua_pushlightuserdata(L, &((*engine).mouse.button[MOUSE_MIDDLE]));
				lua_call(L, 1, 1);
				return 1;
			}

			return 0;
		}

		static int mouseGetRight(lua_State* L)
		{
			lua_getglobal(L, "plum");
			lua_getfield(L, -1, "_Input");
			lua_CFunction inputConstructor = lua_tocfunction(L, -1);
			lua_pop(L, 2);

			if(inputConstructor)
			{
				lua_pushcfunction(L, inputConstructor);
				lua_pushlightuserdata(L, &((*engine).mouse.button[MOUSE_RIGHT]));
				lua_call(L, 1, 1);
				return 1;
			}

			return 0;
		}

		const luaL_Reg mouseMembers[] = {
			{ "__index", mouseGetField },
			{ "__newindex",	mouseSetField },
			{ "__tostring",	mouseToString },
			{ "getx", mouseGetX },
			{ "gety", mouseGetY },
			{ "getwheelPosition", mouseGetWheelPosition },
			{ "setwheelPosition", mouseSetWheelPosition },
			{ "getleft", mouseGetLeft },
			{ "getmiddle", mouseGetMiddle },
			{ "getright", mouseGetRight },
			{ NULL, NULL }
		};

		void initMouseClass(lua_State* L)
		{
			luaL_newmetatable(L, "plum_mouse");
			// Duplicate the metatable on the stack.
			lua_pushvalue(L, -1);
			// metatable.__index = metatable
			lua_setfield(L, -2, "__index");
			// Put the members into the metatable.
			luaL_register(L, NULL, mouseMembers);
			lua_pop(L, 1);

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// Create mouse
			lua_newtable(L);
			lua_pushvalue(L, -1);
			lua_setfield(L, -3, "mouse");

			luaL_getmetatable(L, "plum_mouse");
			lua_setmetatable(L, -2);

			// Pop mouse.
			lua_pop(L, 1);

			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}