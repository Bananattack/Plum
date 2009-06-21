#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static Song** checkValidSongObject(lua_State* L, int index)
		{
			return (Song**) luaL_checkudata(L, index, "plum_song");
		}

		static int songNew(lua_State* L)
		{
			const char* filename = lua_tostring(L, 1);

			Song** s = (Song**) lua_newuserdata(L, sizeof(Song*));
			luaL_getmetatable(L, "plum_song");
			lua_setmetatable(L, -2);

			*s = (*engine).audio.loadSong(filename);
			
			return 1;
		}

		static int songGC(lua_State* L)
		{
			Song** s = checkValidSongObject(L, 1);
			delete *s;

			return 0;
		}

		SCRIPT_OBJ_GETTER(songGetField, Song**, "plum_song")
		SCRIPT_OBJ_SETTER(songSetField, Song**, "plum_song")

		static int songToString(lua_State* L)
		{
			checkValidSongObject(L, 1);
			lua_pushstring(L, "(plum.Song object)");
			return 1;
		}

		static int songPlay(lua_State* L)
		{
			Song** s = checkValidSongObject(L, 1);
			int volume = luaL_optint(L, 2, 100);
			(*engine).audio.playSong(*s);
			return 0;
		}

		static int songStop(lua_State* L)
		{
			Song** s = checkValidSongObject(L, 1);
			(*s)->stop();
			return 0;
		}

		static int songIsPlaying(lua_State* L)
		{
			Song** s = checkValidSongObject(L, 1);
			lua_pushboolean(L, (*s)->isPlaying());
			return 1;
		}

		static int songSetPlaying(lua_State* L)
		{
			Song** s = checkValidSongObject(L, 1);
			bool playing = lua_toboolean(L, 2) != 0;
			if(!playing && (*s)->isPlaying())
			{
				(*s)->stop();
			}
			else if(playing && !(*s)->isPlaying())
			{
				(*engine).audio.playSong(*s);
			}
			return 0;
		}

		static int songGetVolume(lua_State* L)
		{
			Song** s = checkValidSongObject(L, 1);
			lua_pushnumber(L, (*s)->getVolume());
			return 1;
		}

		static int songSetVolume(lua_State* L)
		{
			Song** s = checkValidSongObject(L, 1);
			double value = luaL_checknumber(L, 2);
			(*s)->setVolume(value);
			return 0;
		}

		const luaL_Reg songMembers[] = {
			{ "__index", songGetField },
			{ "__newindex",	songSetField },
			{ "__tostring",	songToString },
			{ "__gc", songGC },
			{ "play", songPlay },
			{ "stop", songStop },
			{ "getplaying", songIsPlaying },
			{ "setplaying", songSetPlaying },
			{ "getvolume", songGetVolume },
			{ "setvolume", songSetVolume },
			{ NULL, NULL }
		};

		void initSongClass(lua_State* L)
		{
			luaL_newmetatable(L, "plum_song");
			// Duplicate the metatable on the stack.
			lua_pushvalue(L, -1);
			// metatable.__index = metatable
			lua_setfield(L, -2, "__index");
			// Put the members into the metatable.
			luaL_register(L, NULL, songMembers);
			lua_pop(L, 1);

			// Push plum namespace.
			lua_getglobal(L, "plum");

			// plum.song = <function songNew>
			lua_pushstring(L, "Song");
			lua_pushcfunction(L, songNew);
			lua_settable(L, -3);

			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}