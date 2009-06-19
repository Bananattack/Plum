#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static Sound** CheckValidSoundObject(lua_State* L, int index)
		{
			return (Sound**) luaL_checkudata(L, index, "plum_sound");
		}

		static int sound_new(lua_State* L)
		{
			const char* filename = lua_tostring(L, 1);

			Sound** s = (Sound**) lua_newuserdata(L, sizeof(Sound*));
			luaL_getmetatable(L, "plum_sound");
			lua_setmetatable(L, -2);

			*s = (*engine).audio.loadSound(filename);
			
			return 1;
		}

		static int sound_gc(lua_State* L)
		{
			Sound** s = CheckValidSoundObject(L, 1);
			delete *s;

			return 0;
		}

		SCRIPT_OBJ_GETTER(sound_getField, Sound**, "plum_sound")
		SCRIPT_OBJ_SETTER(sound_setField, Sound**, "plum_sound")

		static int sound_toString(lua_State* L)
		{
			CheckValidSoundObject(L, 1);
			lua_pushstring(L, "(plum.Sound object)");
			return 1;
		}

		static int sound_play(lua_State* L)
		{
			Sound** s = CheckValidSoundObject(L, 1);
			int volume = luaL_optint(L, 2, 100);
			lua_pushinteger(L, (*engine).audio.playSound(*s, volume));
			return 1;
		}

		const luaL_Reg soundMembers[] = {
			{ "__index", sound_getField },
			{ "__newindex",	sound_setField },
			{ "__tostring",	sound_toString },
			{ "__gc", sound_gc },
			{ "play", sound_play },
			{ NULL, NULL }
		};

		void initSoundClass(lua_State* L)
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

			// plum.sound = <function sound_new>
			lua_pushstring(L, "Sound");
			lua_pushcfunction(L, sound_new);
			lua_settable(L, -3);

			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}