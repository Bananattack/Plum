#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static Input** CheckValidInputObject(lua_State* L, int index)
		{
			return (Input**) luaL_checkudata(L, index, "plum_input");
		}

		static int input_new(lua_State* L)
		{
			Input* input = (Input*) lua_touserdata(L, 1);

			Input** inputPointerReference = (Input**) lua_newuserdata(L, sizeof(Input*));
			luaL_getmetatable(L, "plum_input");
			lua_setmetatable(L, -2);

			*inputPointerReference = input;
			
			return 1;
		}

		SCRIPT_OBJ_GETTER(input_getField, Input**, "plum_input")
		SCRIPT_OBJ_SETTER(input_setField, Input**, "plum_input")

		static int input_toString(lua_State* L)
		{
			CheckValidInputObject(L, 1);
			lua_pushstring(L, "(plum.Input object)");
			return 1;
		}

		static int input_getPressed(lua_State* L)
		{
			Input** inp = CheckValidInputObject(L, 1);
			lua_pushboolean(L, (*inp)->isPressed());
			return 1;
		}

		static int input_setPressed(lua_State* L)
		{
			Input** inp = CheckValidInputObject(L, 1);
			(*inp)->setPressed(lua_toboolean(L, 2) != 0);
			return 0;
		}

		const luaL_Reg inputMembers[] = {
			{ "__index", input_getField },
			{ "__newindex",	input_setField },
			{ "__tostring",	input_toString },
			{ "getpressed", input_getPressed },
			{ "setpressed", input_setPressed },
			{ NULL, NULL }
		};

		void initInputClass(lua_State* L)
		{
			luaL_newmetatable(L, "plum_input");
			// Duplicate the metatable on the stack.
			lua_pushvalue(L, -1);
			// metatable.__index = metatable
			lua_setfield(L, -2, "__index");
			// Put the members into the metatable.
			luaL_register(L, NULL, inputMembers);
			lua_pop(L, 1);

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// plum.input = <function input_new>
			lua_pushstring(L, "_Input");
			lua_pushcfunction(L, input_new);
			lua_settable(L, -3);

			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}