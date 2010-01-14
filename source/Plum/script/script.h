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
			luaL_error(L, "Attempt to modify readonly field '%s' on %s instance.", fieldName.c_str(), metaname); \
			lua_pop(L, 1); \
			return 0; \
		} \
		luaL_error(L, "Attempt to modify unknown field '%s' on %s instance.", fieldName.c_str(), metaname); \
		return 0; \
	}

	class Engine;

	struct Script
	{
		typedef std::map<lua_State*, Script*> ScriptInstanceMap;
		static ScriptInstanceMap scriptInstances;

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
	};


#define PLUM_BIND_LIB(library_name) namespace library_name \
	{ \
		const char* const libraryName = "plum_"#library_name; \
		void openLibrary(lua_State* L); \
	}

#define PLUM_BIND_FUNC_BEGIN() static const luaL_Reg functionsToBind[] = {
#define PLUM_BIND_META(name) { "__"#name, name },
#define PLUM_BIND_FUNC(name) { #name, name },
#define PLUM_BIND_FUNC_END() {NULL, NULL}, }; luaL_register(L, libraryName, functionsToBind);
#define PLUM_BIND_FUNC_END_NULL() {NULL, NULL}, }; luaL_register(L, NULL, functionsToBind);


#define PLUM_IS_DATA(L, i, T) ScriptLibrary::_isWrapper<T>(L, i, "plum_"#T"Object")
#define PLUM_CHECK_DATA(L, i, T) ScriptLibrary::_checkWrapper<T>(L, i, "plum_"#T"Object")
#define PLUM_PUSH_DATA(L, T, data, parentRef) ScriptLibrary::_pushWrapperReference<T>(L, "plum_"#T"Object", data, parentRef)

	namespace ScriptLibrary
	{
		const char* const libraryName = "plum";
		void openLibrary(lua_State* L);

		template <typename T> struct Wrapper
		{
			T* data;

			// The parent, if not NULL, is a registry index to the thing that manages this object's memory
			// (ie. don't delete this, since you didn't allocate the memory.)
			int parentRef;
		};

		template <typename T> bool _isWrapper(lua_State* L, int index, const char* metaname)
		{
			void* p = lua_touserdata(L, index);
			bool result = false;
			// Value is userdata?
			if (p != NULL)
			{
				// Has a metatable?
				if(lua_getmetatable(L, 1))
				{
					// Get correct metatable
					lua_getfield(L, LUA_REGISTRYINDEX, metaname);
					if (lua_rawequal(L, -1, -2))
					{
						result = true;
					}
					lua_pop(L, 2);
				}
			}
			return result;
		}

		template <typename T> Wrapper<T>* _checkWrapper(lua_State* L, int index, const char* metaname)
		{
			return (Wrapper<T>*) luaL_checkudata(L, index, metaname);
		}

		template <typename T> void _pushWrapperReference(lua_State* L, const char* metaname, T* data, int parentRef)
		{
			Wrapper<T>* w = (Wrapper<T>*) lua_newuserdata(L, sizeof(Wrapper<T>));
			luaL_getmetatable(L, metaname);
			lua_setmetatable(L, -2);

			w->data = data;
			w->parentRef = parentRef;
		}

		PLUM_BIND_LIB(ImageObject)
		PLUM_BIND_LIB(TextureObject)
		PLUM_BIND_LIB(SpritesheetObject)
		PLUM_BIND_LIB(TilemapObject)
	}

}
