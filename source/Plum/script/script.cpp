#include "../plum.h"

namespace Plum
{
	ScriptInstanceMap* scriptInstanceMap;
	Script* Script::getInstance(lua_State* L)
	{
		return (*scriptInstanceMap)[L];
	}

	static int panic(lua_State *L)
	{
	//	__asm int 3;
		return 0;
	}

	void Script::startup(Engine* engine)
	{
		L = luaL_newstate();
		this->engine = engine;
		//lua_atpanic(lua, panic);
		luaL_openlibs(L);

		lua_gc(L, LUA_GCSETSTEPMUL, 400);

		// Allow the static script methods to be able to use instance variables,
		// by looking up with the lua_State.
		(*scriptInstanceMap).insert(ScriptInstanceMap::value_type(L, this));

		initPlumModule(L);
		initTextureClass(L);
		initVideoClass(L);
		initFontClass(L);
		initImageClass(L);
		initTimerClass(L);
		initInputClass(L);
		initKeyboardClass(L);
		initMouseClass(L);
		initSoundClass(L);
		initSongClass(L);
		initFileClass(L);
		//initSpriteClass(L);
	}

	void Script::shutdown()
	{
		(*scriptInstanceMap).erase(L);
		lua_close(L);
	}

	void Script::runScript(std::string filename)
	{
		ZZIP_FILE* f = zzip_fopen_plum(filename.c_str(), "rb");
		if(!f)
		{
			throw Engine::Exception("The script file '" + filename + "' was not found.");
		}
		unsigned int length = 0;
		zzip_seek(f, 0, SEEK_END);
		length = zzip_tell(f);
		zzip_seek(f, 0, SEEK_SET);

		char* buf = new char[length + 1];
		zzip_fread(buf, length, 1, f);
		buf[length] = 0;
		zzip_fclose(f);

		if(luaL_loadbuffer(L, buf, strlen(buf), std::string("@" + filename).c_str()) || lua_pcall(L, 0, LUA_MULTRET, 0))
		{
			throw Engine::Exception("Error found in script:\n" + std::string(lua_tostring(L, -1)));
		}
	}

	void Script::stepGarbageCollector()
	{
		lua_gc(L, LUA_GCSTEP, 1);
	}
}