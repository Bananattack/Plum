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
		}

		void shutdown()
		{
			lua_close(lua);
		}

		void runScript()
		{
			std::string code = "plum.setTitle('Giraffes IN SPACE')\n"
				"tex = plum.Texture('resources/sprites/heartsprite.png')\n"
				"while true do\n"
				"	 tex:blit(5, 5, " + integerToString((int) BlendMerge) + ", " + integerToString(Color::White) + ")"
				"    plum.refresh()\n"
				"end\n";

			if(luaL_dostring(lua, code.c_str()))
			{
				engine->quit("Error found in script:\n" + std::string(lua_tostring(lua, -1)));
			}
		}
	}
}