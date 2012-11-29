#pragma once

#include <string>
#include <vector>
#include <cstdint>

extern "C"
{
    // Lua!
    #include <lua/lua.h>
    #include <lua/lualib.h>
    #include <lua/lauxlib.h>
}

namespace plum
{
    class Engine;
    class Script
    {
        private:
            lua_State* L;
            Engine* engine_;
            size_t updateHookRef;

        public:
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

            Script(Engine* engine);
            ~Script();

            Engine& engine()
            {
                return *engine_;
            }

            void run(const std::string& filename);
            void update();
            void stepGarbageCollector();
            void processInputHook(InputHook& hook);
    };

    namespace script
    {
        Script& instance(lua_State* L);

        template<typename T> struct Wrapper
        {
            T* data;
            // The parent, if not NULL, is a registry index to the thing that manages this object's memory
            // (ie. don't delete this, since you didn't allocate the memory.)
            int parentRef;
            // Reference to any 'extra' data that Lua needs to manage.
            int attributeTableRef;

            void pushAttributeTable(lua_State* L)
            {
                // Create attribute table, if not defined.
                if(attributeTableRef == LUA_NOREF)
                {
                    lua_newtable(L);
                    attributeTableRef = luaL_ref(L, LUA_REGISTRYINDEX);
                }
                // Push ext object onto stack.
                lua_rawgeti(L, LUA_REGISTRYINDEX, attributeTableRef);
            }

            void getAttribute(lua_State* L, int key)
            {
                // Push attribute table.
                pushAttributeTable(L);
                // Get value.
                lua_rawgeti(L, -1, key);
                // Remove ext from stack, while keeping value on top of the stack.
                lua_remove(L, -2);
            }

            void setAttribute(lua_State* L, int key)
            {
                // Push attribute table.
                pushAttributeTable(L);
                // Push the value.
                lua_pushvalue(L, -2);
                // Set value.
                lua_rawseti(L, -2, key);
                // Pop ext.
                lua_pop(L, 1);
            }

            int gc(lua_State* L)
            {
                // Discard ext table.
                luaL_unref(L, LUA_REGISTRYINDEX, attributeTableRef);

                // Only delete if it doesn't belong to a parent of some sort.
                if(parentRef == LUA_NOREF)
                {
                    delete data;
                }
                else
                {
                    luaL_unref(L, LUA_REGISTRYINDEX, parentRef);
                }
                return 0;
            }

            int index(lua_State* L)
            {
                std::string fieldName(get<const char*>(L, 2));
                if(luaL_getmetafield(L, 1, std::string("get" + fieldName).c_str()))
                {
                    lua_pushvalue(L, 1);
                    lua_call(L, 1, 1);
                    return 1;
                }
                return luaL_getmetafield(L, 1, fieldName.c_str());
            }

            int newindex(lua_State* L)
            {
                std::string fieldName(get<const char*>(L, 2));
                /* L, 3 is the value to set. */
                if(luaL_getmetafield(L, 1, std::string("set" + fieldName).c_str()))
                {
                    lua_pushvalue(L, 1);
                    lua_pushvalue(L, 3);
                    lua_call(L, 2, 0);
                    return 0;
                }
                return 0;
            }

            int tostring(lua_State* L)
            {
                lua_pushstring(L, meta<T>());
                return 1;
            }
        };

        template<typename T> const char* meta();
        template<typename T> T get(lua_State* L, int index);
        template<typename T> T get(lua_State* L, int index, T fallback);
        template<typename T> T push(lua_State* L, T value);

        template<typename T> bool is(lua_State* L, int index)
        {
            bool result = false;
            // Value is userdata?
            if(auto p = lua_touserdata(L, index))
            {
                // Has a metatable?
                if(lua_getmetatable(L, index))
                {
                    // Get correct metatable
                    lua_getfield(L, LUA_REGISTRYINDEX, meta<T>());
                    if (lua_rawequal(L, -1, -2))
                    {
                        result = true;
                    }
                    lua_pop(L, 2);
                }
            }
            return result;
        }

        template<> inline bool is<int>(lua_State* L, int index)
        {
            return lua_isnumber(L, index) != 0;
        }

        template<> inline bool is<double>(lua_State* L, int index)
        {
            return lua_isnumber(L, index) != 0;
        }

        template<> inline bool is<const char*>(lua_State* L, int index)
        {
            return lua_isstring(L, index) != 0;
        }

        template<> inline bool is<bool>(lua_State* L, int index)
        {
            return lua_isboolean(L, index) != 0;
        }

        template<> inline bool is<nullptr_t>(lua_State* L, int index)
        {
            return lua_isnil(L, index) != 0;
        }


        template<typename T> Wrapper<T>* wrapped(lua_State* L, int index)
        {
            return (Wrapper<T>*) luaL_checkudata(L, index, meta<T>());
        }


        template<typename T> T* ptr(lua_State* L, int index)
        {
            auto w = wrapped<T>(L, index);
            return w ? w->data : nullptr;
        }


        template<> inline int get<int>(lua_State* L, int index)
        {
            return luaL_checkint(L, index);
        }

        template<> inline int get<int>(lua_State* L, int index, int fallback)
        {
            return luaL_optint(L, index, fallback);
        }

        template<> inline const char* get<const char*>(lua_State* L, int index)
        {
            return luaL_checkstring(L, index);
        }

        template<> inline const char* get<const char*>(lua_State* L, int index, const char* fallback)
        {
            return luaL_optstring(L, index, fallback);
        }

        template<> inline double get<double>(lua_State* L, int index)
        {
            return luaL_checknumber(L, index);
        }

        template<> inline double get<double>(lua_State* L, int index, double fallback)
        {
            return luaL_optnumber(L, index, fallback);
        }

        template<> inline bool get<bool>(lua_State* L, int index)
        {
            return lua_toboolean(L, index) != 0;
        }

        template<> inline char push<char>(lua_State* L, char value)
        {
            lua_pushinteger(L, value);
            return value;
        }

        template<> inline int8_t push<int8_t>(lua_State* L, int8_t value)
        {
            lua_pushinteger(L, value);
            return value;
        }

        template<> inline int16_t push<int16_t>(lua_State* L, int16_t value)
        {
            lua_pushinteger(L, value);
            return value;
        }

        template<> inline int32_t push<int32_t>(lua_State* L, int32_t value)
        {
            lua_pushinteger(L, value);
            return value;
        }

        template<> inline uint8_t push<uint8_t>(lua_State* L, uint8_t value)
        {
            lua_pushinteger(L, value);
            return value;
        }

        template<> inline uint16_t push<uint16_t>(lua_State* L, uint16_t value)
        {
            lua_pushinteger(L, value);
            return value;
        }

        template<> inline uint32_t push<uint32_t>(lua_State* L, uint32_t value)
        {
            lua_pushinteger(L, value);
            return value;
        }

        template<> inline const char* push<const char*>(lua_State* L, const char* value)
        {
            lua_pushstring(L, value);
            return value;
        }

        template<> inline float push<float>(lua_State* L, float value)
        {
            lua_pushnumber(L, value);
            return value;
        }

        template<> inline double push<double>(lua_State* L, double value)
        {
            lua_pushnumber(L, value);
            return value;
        }

        template<> inline bool push<bool>(lua_State* L, bool value)
        {
            lua_pushboolean(L, value);
            return value;
        }

        template<> inline nullptr_t push<nullptr_t>(lua_State* L, nullptr_t n)
        {
            lua_pushnil(L);
            return n;
        }

        template<typename T> Wrapper<T>* push(lua_State* L, T* data, int parentRef)
        {
            Wrapper<T>* w = (Wrapper<T>*) lua_newuserdata(L, sizeof(Wrapper<T>));
            luaL_getmetatable(L, meta<T>());
            lua_setmetatable(L, -2);

            w->data = data;
            w->parentRef = parentRef;
            w->attributeTableRef = LUA_NOREF;

            return w;
        }

        void initLibrary(lua_State* L);

        void initVideoModule(lua_State* L);
        void initTimerModule(lua_State* L);

        void initInputObject(lua_State* L);
        void initKeyboardModule(lua_State* L);
        void initMouseModule(lua_State* L);

        void initFontObject(lua_State* L);
        void initSoundObject(lua_State* L);
        void initSongObject(lua_State* L);
        void initFileObject(lua_State* L);
        void initFileObject(lua_State* L);
        void initDirectoryObject(lua_State* L);
        void initCanvasObject(lua_State* L);
        void initPointObject(lua_State* L);
        void initRectObject(lua_State* L);
        void initTransformObject(lua_State* L);
        void initImageObject(lua_State* L);
        void initSpritesheetObject(lua_State* L);
        void initFontObject(lua_State* L);
        void initTilemapObject(lua_State* L);
    }

}
