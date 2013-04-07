
#include "../core/timer.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        void initTimerModule(lua_State* L)
        {
            // Load timer metatable
            luaL_newmetatable(L, "plum_timer");
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");
            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__index", [](lua_State* L)
                {
                    std::string fieldName(script::get<const char*>(L, 2));
                    if(luaL_getmetafield(L, 1, std::string("get_" + fieldName).c_str()))
                    {
                        lua_pushvalue(L, 1);
                        lua_call(L, 1, 1);
                        return 1;
                    }
                    return luaL_getmetafield(L, 1, fieldName.c_str());
                }},
                {"__newindex", [](lua_State* L)
                {
                    std::string fieldName(script::get<const char*>(L, 2));
                    if(luaL_getmetafield(L, 1, std::string("get_" + fieldName).c_str()))
                    {
                        luaL_error(L, "Attempt to modify readonly field '%s' on plum_timer.", fieldName.c_str());
                        lua_pop(L, 1);
                        return 0;
                    }
                    luaL_error(L, "Attempt to modify unknown field '%s' on plum_timer.", fieldName.c_str());
                    return 0;
                }},
                {"__tostring", [](lua_State* L)
                {
                    script::push(L, "(plum.timer singleton)");
                    return 1;
                }},
                {"get_time", [](lua_State* L)
                {
                    script::push(L, script::instance(L).timer().getTime());
                    return 1;
                }},
                {"get_gap", [](lua_State* L)
                {
                    script::push(L, script::instance(L).timer().getDelta());
                    return 1;
                }},
                {"get_fps", [](lua_State* L)
                {
                    script::push(L, script::instance(L).timer().getFPS());
                    return 1;
                }},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);
            lua_pop(L, 1);

            // Push plum namespace.
            lua_getglobal(L, "plum");

            // Create timer namespace
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setfield(L, -3, "timer");

            luaL_getmetatable(L, "plum_timer");
            lua_setmetatable(L, -2);

            // Pop timer namespace.
            lua_pop(L, 1);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}