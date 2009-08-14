#include "../plum.h"

namespace Plum
{
	static Sound** checkValidSoundObject(lua_State* L, int index)
	{
		return (Sound**) luaL_checkudata(L, index, "plum_sound");
	}

	static int soundNew(lua_State* L)
	{
		const char* filename = lua_tostring(L, 1);

		Sound** s = (Sound**) lua_newuserdata(L, sizeof(Sound*));
		luaL_getmetatable(L, "plum_sound");
		lua_setmetatable(L, -2);

		*s = Script::getInstance(L)->engine->audio.loadSound(filename);
		
		return 1;
	}

	static int soundGC(lua_State* L)
	{
		Sound** s = checkValidSoundObject(L, 1);
		if(*s)
		{
			delete *s;
		}

		return 0;
	}

	SCRIPT_OBJ_GETTER(soundGetField, Sound**, "plum_sound")
	SCRIPT_OBJ_SETTER(soundSetField, Sound**, "plum_sound")

	static int soundToString(lua_State* L)
	{
		checkValidSoundObject(L, 1);
		lua_pushstring(L, "(plum.Sound object)");
		return 1;
	}

	static int soundPlay(lua_State* L)
	{
		Sound** s = checkValidSoundObject(L, 1);
		if(!*s)
		{
			lua_pushinteger(L, 0);	
			return 1;
		}

		int volume = luaL_optint(L, 2, 100);
		lua_pushinteger(L, Script::getInstance(L)->engine->audio.playSound(*s, volume));
		return 1;
	}

	static const luaL_Reg soundMembers[] = {
		{ "__index", soundGetField },
		{ "__newindex",	soundSetField },
		{ "__tostring",	soundToString },
		{ "__gc", soundGC },
		{ "play", soundPlay },
		{ NULL, NULL }
	};

	void Script::initSoundClass(lua_State* L)
	{
		luaL_newmetatable(L, "plum_sound");
		// Duplicate the metatable on the stack.
		lua_pushvalue(L, -1);
		// metatable.__index = metatable
		lua_setfield(L, -2, "__index");
		// Put the members into the metatable.
		luaL_register(L, NULL, soundMembers);
		lua_pop(L, 1);

		// Push plum namespace.
		lua_getglobal(L, "plum");

		// plum.sound = <function soundNew>
		lua_pushstring(L, "Sound");
		lua_pushcfunction(L, soundNew);
		lua_settable(L, -3);

		// Pop plum namespace.
		lua_pop(L, 1);
	}
}