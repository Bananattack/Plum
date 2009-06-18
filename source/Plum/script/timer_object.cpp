#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static int timer_getField(lua_State* L)
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

		static int timer_toString(lua_State* L)
		{
			lua_pushstring(L, "(plum.timer singleton)");
			return 1;
		}

		static int timer_getTime(lua_State* L)
		{
			lua_pushinteger(L, (*engine).timer.time / 10);
			return 1;
		}

		static int timer_getGap(lua_State* L)
		{
			lua_pushinteger(L, (*engine).timer.gap);
			return 1;
		}

		static int timer_getFPS(lua_State* L)
		{
			lua_pushinteger(L, (*engine).timer.fps);
			return 1;
		}

		const luaL_Reg timerMembers[] = {
			{ "__index", timer_getField },
			{ "__tostring",	timer_toString },
			{ "gettime", timer_getTime },
			{ "getgap", timer_getGap },
			{ "getfps", timer_getFPS },
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
			lua_pop(L, -1);

			// Pop plum namespace.
			lua_pop(L, -1);
		}
	}
}