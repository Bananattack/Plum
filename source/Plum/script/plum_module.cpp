#include "../plum.h"


namespace Plum
{
	namespace Script
	{
		static int plum_exit(lua_State* lua)
		{
			int argumentCount = lua_gettop(lua);

			if(argumentCount >= 1)
			{
				engine->quit(lua_tostring(lua, 1));
			}
			else
			{
				engine->quit();
			}
			
			return 0;
		}

		static int plum_refresh(lua_State* lua)
		{
			engine->refresh();
			return 0;
		}

		static int plum_setTitle(lua_State* lua)
		{
			int argumentCount = lua_gettop(lua);

			if(argumentCount >= 1)
			{
				engine->setTitle(lua_tostring(lua, 1));
			}
			
			return 0;
		}

		const luaL_Reg plumFunctions[] = {
			{ "exit", plum_exit },
			{ "refresh", plum_refresh },
			{ "setTitle", plum_setTitle },
			{ NULL, NULL }
		};

		void initPlumModule(lua_State* lua)
		{
			// Shove a bunch of functions into a global table "plum"
			// It will now serve as a namespace for all other script bindings
			luaL_register(lua, "plum", plumFunctions);
			lua_pop(lua, 1);
		}
	}
}