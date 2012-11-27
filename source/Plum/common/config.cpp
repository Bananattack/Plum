#include "../plum.h"

namespace plum
{
    void Config::init(const std::string& name, const std::string& blockName, lua_State* state)
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
            "function " + blockName + "(t)\r\n"
            "    called = called + 1;\r\n"
            "    return t\r\n"
            "end";
        luaL_dostring(lua, setup.c_str());
        
        // Load the file into a string.
        File* f = new File(filename.c_str(), FileRead);
        if(!f->active())
        {
            delete f;
            throw std::runtime_error("The " + blockName + " file '" + filename + "' was not found.");
        }
        unsigned int length = 0;
        f->seek(0, SeekEnd);
        length = f->tell();
        f->seek(0, SeekStart);

        int bufsize = length + 8;
        char* buf = new char[bufsize];
        // By going 'return VALUE' there's no chance to inject while/if statements.
        buf[0] = 'r'; buf[1] = 'e'; buf[2] = 't'; buf[3] = 'u'; buf[4] = 'r'; buf[5] = 'n'; buf[6] = ' ';
        f->readRaw(buf + 7, length);
        buf[bufsize - 1] = 0;
        delete f;

        // Load the config
        if(luaL_loadbuffer(lua, buf, strlen(buf), std::string("@" + filename).c_str()))
        {
            // Bad stuff occurred, throw an exception.
            std::string s = "Error while loading " + filename + ":\r\n" + std::string(lua_tostring(lua, -1));
            throw std::runtime_error(s);
        }
        
        // Attempt to call the config (unthreaded).
        if(!parentThread && lua_pcall(lua, 0, LUA_MULTRET, 0))
        {
            // Bad stuff occurred, throw an exception.
            std::string s = "Error while loading " + filename + ":\r\n" + std::string(lua_tostring(lua, -1));
            throw std::runtime_error(s);
        }
        // Attempt to call the config (threaded).
        else if(parentThread && lua_resume(lua, 0))
        {
            // Bad stuff occurred, throw an exception.
            std::string s = "Error while loading " + filename + ":\r\n" + std::string(lua_tostring(lua, -1));
            throw std::runtime_error(s);
        }

        // Ensure that the value on stack is a table.
        if(!lua_istable(lua, -1))
        {
            throw std::runtime_error("Error while loading " + filename + ":\r\nThe " + blockName + " file must be a table.");
        }

        // Make sure they properly put the word "sprite"/"map" or whatever in the header, and don't call it more than once.
        lua_getglobal(lua, "called");
        if(!lua_isnumber(lua, -1) || lua_tonumber(lua, -1) != 1)
        {
            throw std::runtime_error("Error while loading " + filename + ":\r\nNot a valid " + blockName + " format.");
        }
        lua_pop(lua, 1);
        delete [] buf;
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
                lua_close(lua);
            }
        }
    }

    void Config::checkInitialized()
    {
        if(!lua)
        {
            throw std::runtime_error("Tried to parse config before it was initialized");
        }
    }

    bool Config::hasValue(const std::string& key)
    {
        checkInitialized();

        lua_pushstring(lua, key.c_str());
        lua_rawget(lua, -2);

        bool result = !lua_isnil(lua, -1);
        lua_pop(lua, 1);

        return result;
    }

    bool Config::getBoolValue(const std::string& key)
    {
        checkInitialized();

        lua_pushstring(lua, key.c_str());
        lua_rawget(lua, -2);
        if(lua_isnil(lua, -1))
        {
            throw std::runtime_error("Error while parsing " + filename + ":\r\nAn entry for '" + key + "' was not found.");
        }
        if(!lua_isboolean(lua, -1))
        {
            throw std::runtime_error("Error while parsing " + filename + ":\r\nEntry '" + key + "' must be a valid number.");
        }
        bool result = lua_toboolean(lua, -1) != 0;
        lua_pop(lua, 1);
        return result;
    }

    int Config::getIntValue(const std::string& key)
    {
        checkInitialized();

        lua_pushstring(lua, key.c_str());
        lua_rawget(lua, -2);
        if(lua_isnil(lua, -1))
        {
            throw std::runtime_error("Error while parsing " + filename + ":\r\nAn entry for '" + key + "' was not found.");
        }
        if(!lua_isnumber(lua, -1))
        {
            throw std::runtime_error("Error while parsing " + filename + ":\r\nEntry '" + key + "' must be a valid number.");
        }
        int result = lua_tointeger(lua, -1);
        lua_pop(lua, 1);
        return result;
    }

    std::string Config::getStringValue(const std::string& key)
    {
        checkInitialized();

        lua_pushstring(lua, key.c_str());
        lua_rawget(lua, -2);
        if(lua_isnil(lua, -1))
        {
            throw std::runtime_error("Error while parsing " + filename + ":\r\nAn entry for '" + key + "' was not found.");
        }
        if(!lua_isstring(lua, -1))
        {
            throw std::runtime_error("Error while parsing " + filename + ":\r\nEntry '" + key + "' must be a valid string.");
        }
        std::string result = lua_tostring(lua, -1);
        lua_pop(lua, 1);
        return result;
    }
}