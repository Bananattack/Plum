#include "../plum.h"


namespace Plum
{
	static int plumExit(lua_State* L)
	{
		int argumentCount = lua_gettop(L);

		if(argumentCount >= 1)
		{
			Script::getInstance(L)->engine->quit(lua_tostring(L, 1));
		}
		else
		{
			Script::getInstance(L)->engine->quit();
		}
		
		return 0;
	}

	static int plumRefresh(lua_State* L)
	{
		Script::getInstance(L)->engine->refresh();
		return 0;
	}

	static int plumSetTitle(lua_State* L)
	{
		int argumentCount = lua_gettop(L);

		if(argumentCount >= 1)
		{
			Script::getInstance(L)->engine->setTitle(lua_tostring(L, 1));
		}
		
		return 0;
	}

	static int plumRGB(lua_State* L)
	{
		int r = luaL_checkint(L, 1);
		int g = luaL_checkint(L, 2);
		int b = luaL_checkint(L, 3);
		int a = luaL_optint(L, 4, 255);
		lua_pushinteger(L, rgba(r, g, b, a).value);
		return 1;
	}
	
	static int plumHSV(lua_State* L)
	{
		int h = luaL_checkint(L, 1);
		int s = luaL_checkint(L, 2);
		int v = luaL_checkint(L, 3);
		int a = luaL_optint(L, 4, 255);
		lua_pushinteger(L, hsv(h, s, v, a).value);
		return 1;
	}

	static int plumLoadConfig(lua_State* L)
	{
		Config cfg;
		cfg.init("plum.cfg", "config", L);
		lua_pushvalue(cfg.lua, -1);
		lua_xmove(cfg.lua, L, 1);
		return 1;
	}

	static int plumHookInput(lua_State* L)
	{
		Script* script = Script::getInstance(L);

		// Check that first argument is an input object.
		// If so, happily grab the pointer.
		Input** inp = (Input**) luaL_checkudata(L, 1, "plum_input");

		// Get that function on the stack!
		if(!lua_isfunction(L, 2) && !lua_isnil(L, 2))
		{
			luaL_error(L, "Invalid argument #2 to plum.hookInput. Must be a valid function, or nil.");
			return 0;
		}

		// Nuke old callback reference (if any)
		{
			std::vector<Script::InputHook>& hooks = script->inputHooks;
			int ref = LUA_NOREF;
			for(u32 i = 0; i < hooks.size(); i++)
			{
				// Okay, convert our reference number input to userdata.
				Script::InputHook& hook = hooks[i];
				lua_rawgeti(L, LUA_REGISTRYINDEX, hook.inputRef);
				Input** p = (Input**) luaL_checkudata(L, -1, "plum_input");

				// If the passed argument is the same input object,
				// Then discard that old references.
				if(*p == *inp)
				{
					hooks.erase(hooks.begin() + i);

					luaL_unref(L, LUA_REGISTRYINDEX, hook.inputRef);
					luaL_unref(L, LUA_REGISTRYINDEX, hook.callbackRef);
					break;
				}
			}
		}

		// Store new callback reference stuff.
		// Unless nil, in which case we just remove old entries.
		// argh.
		if(!lua_isnil(L, 2))
		{
			Script::InputHook hook;

			lua_pushvalue(L, 1);
			hook.inputRef = luaL_ref(L, LUA_REGISTRYINDEX);

			lua_pushvalue(L, 2);
			hook.callbackRef = luaL_ref(L, LUA_REGISTRYINDEX);

			script->inputHooks.push_back(hook);
		}
		return 0;
	}

	static int plumUnhookAllInput(lua_State* L)
	{
		Script* script = Script::getInstance(L);
		std::vector<Script::InputHook>& hooks = script->inputHooks;
		for(u32 i = 0; i < hooks.size(); i++)
		{
			Script::InputHook& hook = hooks[i];
			luaL_unref(L, LUA_REGISTRYINDEX, hook.inputRef);
			luaL_unref(L, LUA_REGISTRYINDEX, hook.callbackRef);
		}
		hooks.clear();
		return 0;
	}
	
	static const luaL_Reg plumFunctions[] = {
		{ "exit", plumExit },
		{ "refresh", plumRefresh },
		{ "setTitle", plumSetTitle },
		{ "loadConfig", plumLoadConfig },
		{ "hookInput", plumHookInput },
		{ "unhookAllInput", plumUnhookAllInput },
		{ NULL, NULL }
	};

	void Script::initPlumModule(lua_State* L)
	{
		// Shove a bunch of functions into a global table "plum"
		// It will now serve as a namespace for all other script bindings
		luaL_register(L, "plum", plumFunctions);

		// Create the 'color' table.
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setfield(L, -3, "color");

		lua_pushinteger(L, Color::White);
		lua_setfield(L, -2, "White");
		lua_pushinteger(L, Color::Red);
		lua_setfield(L, -2, "Red");
		lua_pushinteger(L, Color::Green);
		lua_setfield(L, -2, "Green");
		lua_pushinteger(L, Color::Blue);
		lua_setfield(L, -2, "Blue");
		lua_pushinteger(L, Color::Magenta);
		lua_setfield(L, -2, "Magenta");
		lua_pushinteger(L, Color::Cyan);
		lua_setfield(L, -2, "Cyan");
		lua_pushinteger(L, Color::Yellow);
		lua_setfield(L, -2, "Yellow");
		lua_pushinteger(L, Color::Black);
		lua_setfield(L, -2, "Black");

		lua_pushcfunction(L, plumRGB);
		lua_setfield(L, -2, "rgb");

		lua_pushcfunction(L, plumHSV);
		lua_setfield(L, -2, "hsv");

		// Done with 'color' now.
		lua_pop(L, 1);

		// Create the 'blend' table.
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setfield(L, -3, "blend");

		lua_pushinteger(L, BlendOpaque);
		lua_setfield(L, -2, "Opaque");
		lua_pushinteger(L, BlendMerge);
		lua_setfield(L, -2, "Merge");
		lua_pushinteger(L, BlendMerge);
		lua_setfield(L, -2, "Alpha");
		lua_pushinteger(L, BlendPreserve);
		lua_setfield(L, -2, "Preserve");
		lua_pushinteger(L, BlendAdd);
		lua_setfield(L, -2, "Add");
		lua_pushinteger(L, BlendSubtract);
		lua_setfield(L, -2, "Subtract");

		// Done with 'blend' now.
		lua_pop(L, 1);

		// Pop the plum namespace.
		lua_pop(L, 1);
	}
}