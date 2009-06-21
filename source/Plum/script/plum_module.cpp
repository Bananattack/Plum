#include "../plum.h"


namespace Plum
{
	namespace Script
	{
		static int plumExit(lua_State* lua)
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

		static int plumRefresh(lua_State* lua)
		{
			engine->refresh();
			return 0;
		}

		static int plumSetTitle(lua_State* lua)
		{
			int argumentCount = lua_gettop(lua);

			if(argumentCount >= 1)
			{
				engine->setTitle(lua_tostring(lua, 1));
			}
			
			return 0;
		}

		static int plumRGB(lua_State* lua)
		{
			int r = luaL_checkint(lua, 1);
			int g = luaL_checkint(lua, 2);
			int b = luaL_checkint(lua, 3);
			int a = luaL_optint(lua, 4, 255);
			lua_pushinteger(lua, rgba(r, g, b, a).value);
			return 1;
		}

		const luaL_Reg plumFunctions[] = {
			{ "exit", plumExit },
			{ "refresh", plumRefresh },
			{ "setTitle", plumSetTitle },
			{ NULL, NULL }
		};

		void initPlumModule(lua_State* lua)
		{
			// Shove a bunch of functions into a global table "plum"
			// It will now serve as a namespace for all other script bindings
			luaL_register(lua, "plum", plumFunctions);

			// Create the 'color' table.
			lua_newtable(lua);
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -3, "color");

			lua_pushinteger(lua, Color::White);
			lua_setfield(lua, -2, "White");
			lua_pushinteger(lua, Color::Red);
			lua_setfield(lua, -2, "Red");
			lua_pushinteger(lua, Color::Green);
			lua_setfield(lua, -2, "Green");
			lua_pushinteger(lua, Color::Magenta);
			lua_setfield(lua, -2, "Magenta");
			lua_pushinteger(lua, Color::Cyan);
			lua_setfield(lua, -2, "Cyan");
			lua_pushinteger(lua, Color::Yellow);
			lua_setfield(lua, -2, "Yellow");
			lua_pushinteger(lua, Color::Black);
			lua_setfield(lua, -2, "Black");

			lua_pushcfunction(lua, plumRGB);
			lua_setfield(lua, -2, "rgb");

			// Done with 'color' now.
			lua_pop(lua, 1);

			// Create the 'blend' table.
			lua_newtable(lua);
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -3, "blend");

			lua_pushinteger(lua, BlendOpaque);
			lua_setfield(lua, -2, "Opaque");
			lua_pushinteger(lua, BlendMerge);
			lua_setfield(lua, -2, "Merge");
			lua_pushinteger(lua, BlendMerge);
			lua_setfield(lua, -2, "Alpha");
			lua_pushinteger(lua, BlendPreserve);
			lua_setfield(lua, -2, "Preserve");
			lua_pushinteger(lua, BlendAdd);
			lua_setfield(lua, -2, "Add");
			lua_pushinteger(lua, BlendSubtract);
			lua_setfield(lua, -2, "Subtract");

			// Done with 'blend' now.
			lua_pop(lua, 1);



			// Pop the plum namespace.
			lua_pop(lua, 1);
		}
	}
}