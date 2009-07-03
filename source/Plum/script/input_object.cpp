#include "../plum.h"

namespace Plum
{
	static Input** checkValidInputObject(lua_State* L, int index)
	{
		return (Input**) luaL_checkudata(L, index, "plum_input");
	}

	static int inputNew(lua_State* L)
	{
		Input* input = (Input*) lua_touserdata(L, 1);

		Input** inputPointerReference = (Input**) lua_newuserdata(L, sizeof(Input*));
		luaL_getmetatable(L, "plum_input");
		lua_setmetatable(L, -2);

		*inputPointerReference = input;
		
		return 1;
	}

	SCRIPT_OBJ_GETTER(inputGetField, Input**, "plum_input")
	SCRIPT_OBJ_SETTER(inputSetField, Input**, "plum_input")

	static int inputToString(lua_State* L)
	{
		checkValidInputObject(L, 1);
		lua_pushstring(L, "(plum.Input object)");
		return 1;
	}

	static int inputGetPressed(lua_State* L)
	{
		Input** inp = checkValidInputObject(L, 1);
		lua_pushboolean(L, (*inp)->isPressed());
		return 1;
	}

	static int inputSetPressed(lua_State* L)
	{
		Input** inp = checkValidInputObject(L, 1);
		(*inp)->setPressed(lua_toboolean(L, 2) != 0);
		return 0;
	}

	static const luaL_Reg inputMembers[] = {
		{ "__index", inputGetField },
		{ "__newindex",	inputSetField },
		{ "__tostring",	inputToString },
		{ "getpressed", inputGetPressed },
		{ "setpressed", inputSetPressed },
		{ NULL, NULL }
	};

	void Script::initInputClass(lua_State* L)
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

		// plum.input = <function inputNew>
		lua_pushstring(L, "_Input");
		lua_pushcfunction(L, inputNew);
		lua_settable(L, -3);

		// Pop plum namespace.
		lua_pop(L, 1);
	}
}