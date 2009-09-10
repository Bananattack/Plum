#include "../plum.h"

namespace Plum
{
	Script::ScriptInstanceMap Script::scriptInstances;
	Script* Script::getInstance(lua_State* L)
	{
		return scriptInstances[L];
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
		scriptInstances.insert(ScriptInstanceMap::value_type(L, this));

		// Load library functions.
		ScriptLibrary::openLibrary(L);
		initVideoClass(L);
		initFontClass(L);
		initTimerClass(L);
		initInputClass(L);
		initKeyboardClass(L);
		initMouseClass(L);
		initSoundClass(L);
		initSongClass(L);
		initFileClass(L);
		initSpriteClass(L);
		initTilesetClass(L);

		luaL_dostring(L,
			"function plum.RequireModuleFromPit(modulename)\r\n"
				// Find the source in a .pit file.
			"	local modulepath = string.gsub(modulename, '%.', '/')\r\n"
			"	for path in string.gmatch(package.path, '([^;]+)') do\r\n"
			"		local filename = string.gsub(path, '%?', modulepath)\r\n"
			"		local f = plum.File(filename, 'r')\r\n"
			"		if f then\r\n"
						// Read and compile the chunk.
			"			local chunk = assert(loadstring(f:readFully(), '@' .. filename))\r\n"
			"			f:close()\r\n"
						// Success!
			"			return chunk\r\n"
			"		end\n"
			"	end\n"
				// Failed to open it.
			"	return '\\r\\n\\tno pit module \\'' .. modulename .. '\\''\r\n"
			"end\r\n"
			"\r\n"

			// Add a few more package path rules that agree with our loader a lot more.
			"package.path = package.path .. ';?.lua;?/init.lua;?\\\\init.lua'\r\n"
			// Install the loader so that it's called just before the DLL loader
			"table.insert(package.loaders, 3, plum.RequireModuleFromPit)\r\n"
		);
	}

	void Script::shutdown()
	{
		lua_close(L);
		scriptInstances.erase(L);
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
			throw Engine::Exception("Error found in script:\r\n" + std::string(lua_tostring(L, -1)));
		}
	}

	void Script::stepGarbageCollector()
	{
		lua_gc(L, LUA_GCSTEP, 1);
	}

	void Script::processInputHook(Script::InputHook& hook)
	{
		// Okay, convert our reference number input to userdata.
		lua_rawgeti(L, LUA_REGISTRYINDEX, hook.inputRef);
		Input** inp = (Input**) luaL_checkudata(L, -1, "plum_input");

		// If the button's presed, we can call the hook callback.
		if((*inp)->isPressed())
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, hook.callbackRef);
			lua_rawgeti(L, LUA_REGISTRYINDEX, hook.inputRef);
			lua_call(L, 1, 0);
		}
	}
}