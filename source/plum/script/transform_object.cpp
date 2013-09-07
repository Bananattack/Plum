
#include "../core/transform.h"
#include "script.h"

namespace plum
{
    namespace script
    {
        template<> const char* meta<Transform>()
        {
            return "plum.Transform";
        }

        void initTransformObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Transform>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", [](lua_State* L) { return script::wrapped<Transform>(L, 1)->gc(L); }},
                {"__index", [](lua_State* L) { return script::wrapped<Transform>(L, 1)->index(L); }},
                {"__newindex", [](lua_State* L) { return script::wrapped<Transform>(L, 1)->newindex(L); }},
                {"__tostring", [](lua_State* L) { return script::wrapped<Transform>(L, 1)->tostring(L); }},
                {"__pairs", [](lua_State* L) { return script::wrapped<Transform>(L, 1)->pairs(L); }},
                {"get_mirror", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    script::push(L, t->mirror);
                    return 1;
                }},
                {"set_mirror", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    auto mirror = script::get<bool>(L, 2);
                    t->mirror = mirror;
                    return 0;
                }},
                {"get_angle", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    script::push(L, t->angle);
                    return 1;
                }},
                {"set_angle", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    auto value = script::get<double>(L, 2);
                    t->angle = value;
                    return 0;
                }},
                {"get_blend", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    script::push(L, (int) t->blend);
                    return 1;
                }},
                {"set_blend", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    auto value = script::get<int>(L, 2);
                    t->blend = (BlendMode) value;
                    return 0;
                }},
                {"get_tint", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    script::push(L, (int) t->tint);
                    return 1;
                }},
                {"set_tint", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    auto value = script::get<int>(L, 2);
                    t->tint = value;
                    return 0;
                }},
                {"get_scaleX", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    script::push(L, t->scaleX);
                    return 1;
                }},
                {"set_scaleX", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    auto value = script::get<double>(L, 2);
                    t->scaleX = value;
                    return 0;
                }},
                {"get_scaleY", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    script::push(L, t->scaleY);
                    return 1;
                }},
                {"set_scaleY", [](lua_State* L)
                {
                    auto t = script::ptr<Transform>(L, 1);
                    auto value = script::get<double>(L, 2);
                    t->scaleY = value;
                    return 0;
                }},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);
                
            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Transform = <function create>
            script::push(L, "Transform");
            lua_pushcfunction(L, [](lua_State* L)
            {
                auto w = script::push(L, new Transform(), LUA_NOREF);
                return 1;
            });
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}