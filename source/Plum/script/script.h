#pragma once
namespace Plum
{

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

	class Engine;

	struct Script
	{
		static Script* getInstance(lua_State* L);

		lua_State* L;
		Engine* engine;

		// A mapping of type: input reference -> function index
		// Oh, and the function needs to be of form:
		// void f(keyIndex);
		// This way callbacks can be potentially used across several keys
		// and determine which is pressed by the name passed
		struct InputHook
		{
			int inputRef;
			int callbackRef;
		};
		std::vector<InputHook> inputHooks;

		Script()
			: L(0), engine(0)
		{
		}

		void startup(Engine* engine);
		void runScript(std::string filename);
		void shutdown();
		void stepGarbageCollector();
		void processInputHook(InputHook& hook);

		static void initPlumModule(lua_State* L);

		static void initTextureClass(lua_State* L);
		static void initImageClass(lua_State* L);
		static void initSpriteClass(lua_State* L);
		static void initVideoClass(lua_State* L);
		static void initFontClass(lua_State* L);
		static void initTimerClass(lua_State* L);
		static void initInputClass(lua_State* L);
		static void initKeyboardClass(lua_State* L);
		static void initMouseClass(lua_State* L);
		static void initSoundClass(lua_State* L);
		static void initSongClass(lua_State* L);
		static void initFileClass(lua_State* L);
		static void initTilesetClass(lua_State* L);

		struct ImageWrapper
		{
			Image* image;
			bool canDelete;
		};

		struct TextureWrapper
		{
			bool canDelete;
			Texture* texture;	
		};
		
		static Script::TextureWrapper* checkValidTextureObject(lua_State* L, int index);

		static void pushImageRef(lua_State* L, Image* image);
		static void pushTextureRef(lua_State* L, Texture* texture);
	};

	typedef std::map<lua_State*, Script*> ScriptInstanceMap;
	extern ScriptInstanceMap* scriptInstances;
	void initScriptInstances(ScriptInstanceMap* map);
}
