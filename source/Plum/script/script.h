#ifndef PLUM_SCRIPT_H
#define PLUM_SCRIPT_H

namespace Plum
{
	namespace Script
	{
		extern lua_State* lua;
		extern Engine* engine;

		void startup(Engine* eng);
		void runScript(std::string filename);
		void shutdown();
	}

#define SCRIPT_OBJ_GETTER(funcname, udtype, metaname) \
	static int funcname(lua_State* L) \
	{ \
		udtype t = (udtype) luaL_checkudata(L, 1, metaname); \
		std::string fieldName = luaL_checkstring(L, 2); \
		\
		if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str())) \
		{ \
			lua_pushvalue(L, 1); \
			lua_call(L, 1, 1); \
			return 1; \
		} \
		return luaL_getmetafield(L, 1, fieldName.c_str()); \
	}

#define SCRIPT_OBJ_SETTER(funcname, udtype, metaname) \
	static int funcname(lua_State* L) \
	{ \
		udtype t = (udtype) luaL_checkudata(L, 1, metaname); \
		std::string fieldName = luaL_checkstring(L, 2); \
		/* L, 3 is the value to set. */ \
		if(luaL_getmetafield(L, 1, std::string("set" + fieldName).c_str())) \
		{  \
			lua_pushvalue(L, 1); \
			lua_pushvalue(L, 3); \
			lua_call(L, 2, 0); \
			return 0; \
		} \
		if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str())) \
		{ \
			luaL_error(L, "Attempt to modify readonly field '%s' on " metaname " instance.", fieldName.c_str()); \
			lua_pop(L, 1); \
			return 0; \
		} \
		luaL_error(L, "Attempt to modify unknown field '%s' on " metaname " instance.", fieldName.c_str()); \
		return 0; \
	}
}
#endif