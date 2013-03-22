
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
    }

    namespace
    {
        typedef Transform Self;
        int create(lua_State* L)
        {
            Self* t = new Self();
            auto w = script::push(L, t, LUA_NOREF);
            return 1;
        }

        int gc(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->gc(L);
        }

        int index(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->index(L);
        }

        int newindex(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->newindex(L);
        }

        int tostring(lua_State* L)
        {
            return script::wrapped<Self>(L, 1)->tostring(L);
        }

        int get_mirror(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            script::push(L, t->mirror);
            return 1;
        }

        int set_mirror(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            auto mirror = script::get<bool>(L, 2);
            t->mirror = mirror;
            return 0;
        }

        int get_angle(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            script::push(L, t->angle);
            return 1;
        }

        int set_angle(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            auto value = script::get<double>(L, 2);
            t->angle = value;
            return 0;
        }

        int get_mode(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            script::push(L, (int) t->mode);
            return 1;
        }

        int set_mode(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            auto value = script::get<int>(L, 2);
            t->mode = (BlendMode) value;
            return 0;
        }

        int get_tint(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            script::push(L, (int) t->tint);
            return 1;
        }

        int set_tint(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            auto value = script::get<int>(L, 2);
            t->tint = value;
            return 0;
        }

        int get_scaleX(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            script::push(L, t->scaleX);
            return 1;
        }

        int set_scaleX(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            auto value = script::get<double>(L, 2);
            t->scaleX = value;
            return 0;
        }

        int get_scaleY(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            script::push(L, t->scaleY);
            return 1;
        }

        int set_scaleY(lua_State* L)
        {
            auto t = script::ptr<Transform>(L, 1);
            auto value = script::get<double>(L, 2);
            t->scaleY = value;
            return 0;
        }
    }

    namespace script
    {
        void initTransformObject(lua_State* L)
        {
            luaL_newmetatable(L, meta<Self>());
            // Duplicate the metatable on the stack.
            lua_pushvalue(L, -1);
            // metatable.__index = metatable
            lua_setfield(L, -2, "__index");

            // Put the members into the metatable.
            const luaL_Reg functions[] = {
                {"__gc", gc},
                {"__index", index},
                {"__newindex", newindex},
                {"__tostring", tostring},
                {"get_mirror", get_mirror},
                {"set_mirror", set_mirror},
                {"get_angle", get_angle},
                {"set_angle", set_angle},
                {"get_mode", get_mode},
                {"set_mode", set_mode},
                {"get_tint", get_tint},
                {"set_tint", set_tint},
                {"get_scaleX", get_scaleX},
                {"set_scaleX", set_scaleX},
                {"get_scaleY", get_scaleY},
                {"set_scaleY", set_scaleY},
                {nullptr, nullptr}
            };
            luaL_setfuncs(L, functions, 0);

            lua_pop(L, 1);
                
            // Push plum namespace.
            lua_getglobal(L, "plum");

            // plum.Transform = <function create>
            script::push(L, "Transform");
            lua_pushcfunction(L, create);
            lua_settable(L, -3);

            // Pop plum namespace.
            lua_pop(L, 1);
        }
    }
}