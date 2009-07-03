#include "../plum.h"

namespace Plum
{
	void Config::init(std::string name, std::string blockName, lua_State* state)
	{
		filename = name;

		// Config files will have a general format:
		// config { key = value; ... }
		// Don't include any lua libraries, for good reason.
		if(!state)
		{
			lua = luaL_newstate();
			ref = LUA_NOREF;
			parentThread = NULL;
		}
		else
		{
			lua = lua_newthread(state);
			parentThread = state;
			ref = luaL_ref(state, LUA_REGISTRYINDEX);
			
			// Setup a unique global table for the this thread.
			// This way it is isolated from the main thread, and can't overwrite any variables.
			lua_newtable(lua);  
			lua_newtable(lua);  
			lua_pushliteral(lua, "__index");  
			lua_pushvalue(lua, LUA_GLOBALSINDEX);  
			lua_settable(lua, -3);  
			lua_setmetatable(lua, -2);  
			lua_replace(lua, LUA_GLOBALSINDEX); 
		}
		
		std::string setup = "called = 0;"
			"function " + blockName + "(t)\n"
			"    called = called + 1;\n"
			"    return t\n"
			"end";
		luaL_dostring(lua, setup.c_str());
		
		// Load the file into a string.
		ZZIP_FILE* f = zzip_fopen_plum(filename.c_str(), "rb");
		if(!f)
		{
			throw Engine::Exception("The " + blockName + " file '" + filename + "' was not found.");
		}
		unsigned int length = 0;
		zzip_seek(f, 0, SEEK_END);
		length = zzip_tell(f);
		zzip_seek(f, 0, SEEK_SET);

		int bufsize = length + 8;
		char* buf = new char[bufsize];
		// By going 'return VALUE' there's no chance to inject while/if statements.
		buf[0] = 'r'; buf[1] = 'e'; buf[2] = 't'; buf[3] = 'u'; buf[4] = 'r'; buf[5] = 'n'; buf[6] = ' ';
		zzip_fread(buf + 7, length, 1, f);
		buf[bufsize - 1] = 0;
		zzip_fclose(f);

		// Load the config
		if(luaL_loadbuffer(lua, buf, strlen(buf), std::string("@" + filename).c_str()))
		{
			// Bad stuff occurred, throw an exception.
			std::string s = "Error while loading " + filename + ":\n" + std::string(lua_tostring(lua, -1));
			throw Engine::Exception(s);
		}
		
		// Attempt to call the config (unthreaded).
		if(!parentThread && lua_pcall(lua, 0, LUA_MULTRET, 0))
		{
			// Bad stuff occurred, throw an exception.
			std::string s = "Error while loading " + filename + ":\n" + std::string(lua_tostring(lua, -1));
			throw Engine::Exception(s);
		}
		// Attempt to call the config (threaded).
		else if(parentThread && lua_resume(lua, 0))
		{
			// Bad stuff occurred, throw an exception.
			std::string s = "Error while loading " + filename + ":\n" + std::string(lua_tostring(lua, -1));
			throw Engine::Exception(s);
		}

		// Ensure that the value on stack is a table.
		if(!lua_istable(lua, -1))
		{
			throw Engine::Exception("Error while loading " + filename + ":\nThe " + blockName + " file must be a table.");
		}

		// Make sure they properly put the word "sprite"/"map" or whatever in the header, and don't call it more than once.
		lua_getglobal(lua, "called");
		if(!lua_isnumber(lua, -1) || lua_tonumber(lua, -1) != 1)
		{
			throw Engine::Exception("Error while loading " + filename + ":\nNot a valid " + blockName + " format.");
		}
		lua_pop(lua, 1);
		delete [] buf;

		FILE* fl = fopen("loggy.log", "a");
		fprintf(fl, "CREATING STUFF 0x%x\n", lua);
		fclose(fl);
	}

	Config::~Config()
	{
		if(parentThread)
		{
			luaL_unref(parentThread, LUA_REGISTRYINDEX, ref);
		}
		else
		{
			if(lua)
			{
				FILE* f = fopen("loggy.log", "a");
				fprintf(f, "CLOSING STUFF 0x%x\n", lua);
				fclose(f);
				lua_close(lua);
			}
		}
	}

	void Config::checkInitialized()
	{
		if(!lua)
		{
			throw Engine::Exception("Tried to parse config before it was initialized");
		}
	}

	bool Config::hasValue(std::string key)
	{
		checkInitialized();

		FILE* f = fopen("loggy.log", "a");
		fprintf(f, "USING STUFF 0x%x\n", lua);
		fclose(f);

		lua_pushstring(lua, key.c_str());
		lua_rawget(lua, -2);

		bool result = !lua_isnil(lua, -1);
		lua_pop(lua, 1);

		f = fopen("loggy.log", "a");
		fprintf(f, "I WIN\n");
		fclose(f);

		return result;
	}

	bool Config::getBoolValue(std::string key)
	{
		checkInitialized();

		lua_pushstring(lua, key.c_str());
		lua_rawget(lua, -2);
		if(lua_isnil(lua, -1))
		{
			throw Engine::Exception("Error while parsing " + filename + ":\nAn entry for '" + key + "' was not found.");
		}
		if(!lua_isboolean(lua, -1))
		{
			throw Engine::Exception("Error while parsing " + filename + ":\nEntry '" + key + "' must be a valid number.");
		}
		bool result = lua_toboolean(lua, -1) != 0;
		lua_pop(lua, 1);
		return result;
	}

	int Config::getIntValue(std::string key)
	{
		checkInitialized();

		lua_pushstring(lua, key.c_str());
		lua_rawget(lua, -2);
		if(lua_isnil(lua, -1))
		{
			throw Engine::Exception("Error while parsing " + filename + ":\nAn entry for '" + key + "' was not found.");
		}
		if(!lua_isnumber(lua, -1))
		{
			throw Engine::Exception("Error while parsing " + filename + ":\nEntry '" + key + "' must be a valid number.");
		}
		int result = lua_tointeger(lua, -1);
		lua_pop(lua, 1);
		return result;
	}

	std::string Config::getStringValue(std::string key)
	{
		checkInitialized();

		lua_pushstring(lua, key.c_str());
		lua_rawget(lua, -2);
		if(lua_isnil(lua, -1))
		{
			throw Engine::Exception("Error while parsing " + filename + ":\nAn entry for '" + key + "' was not found.");
		}
		if(!lua_isstring(lua, -1))
		{
			throw Engine::Exception("Error while parsing " + filename + ":\nEntry '" + key + "' must be a valid string.");
		}
		std::string result = lua_tostring(lua, -1);
		lua_pop(lua, 1);
		return result;
	}
}