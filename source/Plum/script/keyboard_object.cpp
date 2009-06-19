#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		void initKeyboardClass(lua_State* L)
		{
			lua_CFunction inputConstructor = NULL;

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// Store the reference to the plum._Input constructor
			lua_getfield(L, -1, "_Input");
			inputConstructor = lua_tocfunction(L, -1);
			lua_pop(L, 1);

			// t = {}
			lua_newtable(L);

			// plum.key = t
			lua_pushstring(L, "key");
			lua_pushvalue(L, -2);
			lua_settable(L, -4);

			// inp = plum._Input(POINTER);
			lua_pushcfunction(L, inputConstructor);
			lua_pushlightuserdata(L, &(engine->key[KEY_ENTER]));
			lua_call(L, 1, 1);
			// plum.key.Enter = inp
			lua_setfield(L, -2, "Enter");

			// Pop plum.key table. (Whew.)
			lua_pop(L, 1);
		
			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}