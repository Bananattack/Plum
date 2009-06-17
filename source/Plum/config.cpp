#include "plum.h"

namespace Plum
{
	Config::Config(std::string name, std::string blockName)
	{
		filename = name;

		// Config files will have a general format:
		// config { key = value; ... }
		// Don't include any lua libraries, for good reason.
		lua = luaL_newstate();
		
		std::string setup = "called = 0;"
			"function " + blockName + "(t)\n"
			"    called = called + 1;\n"
			"    return t\n"
			"end";
		luaL_dostring(lua, setup.c_str());
		
		// Load the file into a string.
		FILE* f = fopen(filename.c_str(), "rb");
		if(!f)
		{
			throw Engine::Exception("The " + blockName + " file '" + filename + "' was not found.");
		}
		unsigned int length = 0;
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);

		char* buf = new char[length + 8];
		// By going 'return VALUE' there's no chance to inject while/if statements.
		buf[0] = 'r'; buf[1] = 'e'; buf[2] = 't'; buf[3] = 'u'; buf[4] = 'r'; buf[5] = 'n'; buf[6] = ' ';
		fread(buf + 7, length, 1, f);
		buf[length + 7] = 0;
		fclose(f);

		// Load the config
		if(luaL_dostring(lua, buf))
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

		lua_pushstring(lua, key.c_str());
		lua_rawget(lua, -2);

		bool result = !lua_isnil(lua, -1);
		lua_pop(lua, 1);
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