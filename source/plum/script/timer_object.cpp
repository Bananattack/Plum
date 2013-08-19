
#include "../core/timer.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        namespace
        {
            const char* const Meta = "plum.Timer";
        }

        void initTimerModule(lua_State* L)
        {
            // Load timer metatable
            luaL_newmetatable(L, Meta);
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
                    /* L, 3 is the value to set. */
                    if(luaL_getmetafield(L, 1, std::string("set_" + fieldName).c_str()))
                    {
                        lua_pushvalue(L, 1);
                        lua_pushvalue(L, 3);
                        lua_call(L, 2, 0);
                    }
                    return 0;
                }},
                {"__tostring", [](lua_State* L)
                {
                    script::push(L, Meta);
                    return 1;
                }},
                {"__pairs", [](lua_State* L)
                {
                    lua_getglobal(L, "next");
                    luaL_getmetatable(L, Meta);
                    lua_pushnil(L);
                    return 3;
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
                {"get_speed", [](lua_State* L)
                {
                    const char* speed = "n";
                    switch(script::instance(L).timer().getSpeed())
                    {
                        case plum::TimerSpeedSlowMotion: speed = "s";
                        case plum::TimerSpeedFastForward: speed = "f";
                        default: break;
                    }
                    script::push(L, speed);
                    return 1;
                }},
                {"set_speed", [](lua_State* L)
                {
                    const char* speed = script::get<const char*>(L, 2);
                    auto timerSpeed = plum::TimerSpeedNormal;
                    switch(speed[0])
                    {
                        case 's': timerSpeed = plum::TimerSpeedSlowMotion; break;
                        case 'f': timerSpeed = plum::TimerSpeedFastForward; break;
                        default: break;
                    }
                    script::instance(L).timer().setSpeed(timerSpeed);
                    return 0;
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

            luaL_getmetatable(L, Meta);
            lua_setmetatable(L, -2);

            // Pop timer namespace.
            lua_pop(L, 1);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}