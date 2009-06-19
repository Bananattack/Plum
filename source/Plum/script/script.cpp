#include "../plum.h"

namespace Plum
{
	namespace Script
	{
		lua_State* lua = NULL;
		Engine* engine = NULL;

		void startup(Engine* eng)
		{
			engine = eng;

			lua = luaL_newstate();
			luaL_openlibs(lua);

			initPlumModule(lua);
			initTextureClass(lua);
			initVideoClass(lua);
			initFontClass(lua);
			initTimerClass(lua);
		}

		void shutdown()
		{
			lua_close(lua);
		}

		void runScript(std::string filename)
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

			if(luaL_loadbuffer(lua, buf, strlen(buf), std::string("@" + filename).c_str()) || lua_pcall(lua, 0, LUA_MULTRET, 0))
			{
				engine->quit("Error found in script:\n" + std::string(lua_tostring(lua, -1)));
			}
		}
	}
}