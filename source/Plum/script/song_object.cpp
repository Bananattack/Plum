#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		static Song** CheckValidSongObject(lua_State* L, int index)
		{
			return (Song**) luaL_checkudata(L, index, "plum_song");
		}

		static int song_new(lua_State* L)
		{
			const char* filename = lua_tostring(L, 1);

			Song** s = (Song**) lua_newuserdata(L, sizeof(Song*));
			luaL_getmetatable(L, "plum_song");
			lua_setmetatable(L, -2);

			*s = (*engine).audio.loadSong(filename);
			
			return 1;
		}

		static int song_gc(lua_State* L)
		{
			Song** s = CheckValidSongObject(L, 1);
			delete *s;

			return 0;
		}

		SCRIPT_OBJ_GETTER(song_getField, Song**, "plum_song")
		SCRIPT_OBJ_SETTER(song_setField, Song**, "plum_song")

		static int song_toString(lua_State* L)
		{
			CheckValidSongObject(L, 1);
			lua_pushstring(L, "(plum.Song object)");
			return 1;
		}

		static int song_play(lua_State* L)
		{
			Song** s = CheckValidSongObject(L, 1);
			int volume = luaL_optint(L, 2, 100);
			(*engine).audio.playSong(*s);
			return 0;
		}

		static int song_stop(lua_State* L)
		{
			Song** s = CheckValidSongObject(L, 1);
			(*s)->stop();
			return 0;
		}

		static int song_isPlaying(lua_State* L)
		{
			Song** s = CheckValidSongObject(L, 1);
			lua_pushboolean(L, (*s)->isPlaying());
			return 1;
		}

		static int song_setPlaying(lua_State* L)
		{
			Song** s = CheckValidSongObject(L, 1);
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

		static int song_getVolume(lua_State* L)
		{
			Song** s = CheckValidSongObject(L, 1);
			lua_pushnumber(L, (*s)->getVolume());
			return 1;
		}

		static int song_setVolume(lua_State* L)
		{
			Song** s = CheckValidSongObject(L, 1);
			double value = luaL_checknumber(L, 2);
			(*s)->setVolume(value);
			return 0;
		}

		const luaL_Reg songMembers[] = {
			{ "__index", song_getField },
			{ "__newindex",	song_setField },
			{ "__tostring",	song_toString },
			{ "__gc", song_gc },
			{ "play", song_play },
			{ "stop", song_stop },
			{ "getplaying", song_isPlaying },
			{ "setplaying", song_setPlaying },
			{ "getvolume", song_getVolume },
			{ "setvolume", song_setVolume },
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

			// plum.song = <function song_new>
			lua_pushstring(L, "Song");
			lua_pushcfunction(L, song_new);
			lua_settable(L, -3);

			// Pop plum namespace.
			lua_pop(L, 1);
		}
	}
}