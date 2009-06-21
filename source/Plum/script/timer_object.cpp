#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static int timerGetField(lua_State* L)
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

		static int timerSetField(lua_State* L)
		{
			std::string fieldName = luaL_checkstring(L, 2);
			if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str()))
			{
				luaL_error(L, "Attempt to modify readonly field '%s' on plum_timer.", fieldName.c_str());
				lua_pop(L, 1);
				return 0;
			}
			luaL_error(L, "Attempt to modify unknown field '%s' on plum_timer.", fieldName.c_str());
			return 0;
		}

		static int timerToString(lua_State* L)
		{
			lua_pushstring(L, "(plum.timer singleton)");
			return 1;
		}

		static int timerGetTime(lua_State* L)
		{
			lua_pushinteger(L, (*engine).timer.time / 10);
			return 1;
		}

		static int timerGetGap(lua_State* L)
		{
			lua_pushinteger(L, (*engine).timer.gap);
			return 1;
		}

		static int timerGetFPS(lua_State* L)
		{
			lua_pushinteger(L, (*engine).timer.fps);
			return 1;
		}

		const luaL_Reg timerMembers[] = {
			{ "__index", timerGetField },
			{ "__newindex", timerSetField },
			{ "__tostring",	timerToString },
			{ "gettime", timerGetTime },
			{ "getgap", timerGetGap },
			{ "getfps", timerGetFPS },
			{ NULL, NULL }
		};

		void initTimerClass(lua_State* L)
		{
			// Load timer metatable
			luaL_newmetatable(L, "plum_timer");
			// Duplicate the metatable on the stack.
			lua_pushvalue(L, -1);
			// metatable.__index = metatable
			lua_setfield(L, -2, "__index");
			// Put the members into the metatable.
			luaL_register(L, NULL, timerMembers);
			lua_pop(L, 1);

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// Create timer namespace
			lua_newtable(L);
			lua_pushvalue(L, -1);
			lua_setfield(L, -3, "timer");

			luaL_getmetatable(L, "plum_timer");
			lua_setmetatable(L, -2);

			// Pop timer namespace.
			lua_pop(L, 1);

			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}